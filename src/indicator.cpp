#include "indicator.h"

#include "vanetza/geonet/mib.hpp"
#include "vanetza/geonet/indication_context.hpp"
#include "vanetza/geonet/tests/security_context.hpp"
#include "vanetza/common/manual_runtime.hpp"

#include "vanetza/btp/data_indication.hpp"

UpPacketPtr indicate_basic(vanetza::geonet::IndicationContextBasic& ctx, vanetza::geonet::ManagementInformationBase& mib);
UpPacketPtr indicate_common(vanetza::geonet::IndicationContext& ctx, const vanetza::geonet::BasicHeader& basic, vanetza::geonet::ManagementInformationBase& mib);
UpPacketPtr indicate_secured(vanetza::geonet::IndicationContextBasic& ctx, const vanetza::geonet::BasicHeader& basic, vanetza::geonet::ManagementInformationBase& mib);
UpPacketPtr indicate_extended(vanetza::geonet::IndicationContext& ctx, const vanetza::geonet::CommonHeader& common, vanetza::geonet::ManagementInformationBase& mib);

UpPacketPtr indicate(UpPacketPtr packet, const vanetza::MacAddress& sender, const vanetza::MacAddress& destination)
{
    assert(packet);

    vanetza::geonet::ManagementInformationBase mib;

    const auto size_limit = mib.itsGnMaxSduSize + mib.itsGnMaxGeoNetworkingHeaderSize;
    if (size(*packet) <= size_limit) {
        vanetza::geonet::IndicationContext::LinkLayer link_layer;
        link_layer.sender = sender;
        link_layer.destination = destination;

        if (auto cohesive = boost::get<vanetza::CohesivePacket>(packet.get())) {
            vanetza::geonet::IndicationContextDeserialize ctx(std::move(packet), *cohesive, link_layer);
            return indicate_basic(ctx, mib);

        } else if (auto chunk = boost::get<vanetza::ChunkPacket>(packet.get())) {
            vanetza::geonet::IndicationContextCast ctx(std::move(packet), *chunk, link_layer);
            return indicate_basic(ctx, mib);
        } else {
            //packet_dropped(PacketDropReason::Internal_Error);
            return nullptr;
        }
    } else {
        //packet_dropped(PacketDropReason::Packet_Size);
        return nullptr;
    }
}

UpPacketPtr indicate_basic(vanetza::geonet::IndicationContextBasic& ctx, vanetza::geonet::ManagementInformationBase& mib)
{
    const vanetza::geonet::BasicHeader* basic = ctx.parse_basic();
    if (!basic) {
        //packet_dropped(PacketDropReason::Parse_Basic_Header);
        return nullptr;
    } else if (basic->version.raw() != mib.itsGnProtocolVersion) {
        //packet_dropped(PacketDropReason::ITS_Protocol_Version);
        return nullptr;
    } else {
        vanetza::geonet::DataIndication& indication = ctx.service_primitive();
        indication.remaining_packet_lifetime = basic->lifetime;
        indication.remaining_hop_limit = basic->hop_limit;

        if (basic->next_header == vanetza::geonet::NextHeaderBasic::Secured) {
            indication.security_report = vanetza::security::DecapReport::Incompatible_Protocol;
            return indicate_secured(ctx, *basic, mib);
        } else if (basic->next_header == vanetza::geonet::NextHeaderBasic::Common) {
            if (!mib.itsGnSecurity || vanetza::geonet::SecurityDecapHandling::Non_Strict == mib.itsGnSnDecapResultHandling) {
                indication.security_report = vanetza::security::DecapReport::Unsigned_Message;
                return indicate_common(ctx, *basic, mib);
            } else {
                // packet_dropped(PacketDropReason::Decap_Unsuccessful_Strict);
                return nullptr;
            }
        }
    }
    return nullptr;
}

UpPacketPtr indicate_common(vanetza::geonet::IndicationContext& ctx, const vanetza::geonet::BasicHeader& basic, vanetza::geonet::ManagementInformationBase& mib)
{
    const vanetza::geonet::CommonHeader* common = ctx.parse_common();
    if (!common) {
        // packet_dropped(PacketDropReason::Parse_Common_Header);
        return nullptr;
    } else if (common->maximum_hop_limit < basic.hop_limit) {
        // step 1) check the MHL field
        // packet_dropped(PacketDropReason::Hop_Limit);
        return nullptr;
    } else {
        vanetza::geonet::DataIndication& indication = ctx.service_primitive();
        indication.traffic_class = common->traffic_class;
        switch (common->next_header)
        {
            case vanetza::geonet::NextHeaderCommon::BTP_A:
                indication.upper_protocol = vanetza::geonet::UpperProtocol::BTP_A;
                break;
            case vanetza::geonet::NextHeaderCommon::BTP_B:
                indication.upper_protocol = vanetza::geonet::UpperProtocol::BTP_B;
                break;
            case vanetza::geonet::NextHeaderCommon::IPv6:
                indication.upper_protocol = vanetza::geonet::UpperProtocol::IPv6;
                break;
            default:
                indication.upper_protocol = vanetza::geonet::UpperProtocol::Unknown;
                break;
        }

        // step 3) execute steps depending on extended header type
        return indicate_extended(ctx, *common, mib);
    }
}

UpPacketPtr indicate_secured(vanetza::geonet::IndicationContextBasic& ctx, const vanetza::geonet::BasicHeader& basic, vanetza::geonet::ManagementInformationBase& mib)
{
    auto secured_message = ctx.parse_secured();

    vanetza::ManualRuntime runtime;
    vanetza::SecurityContext securityContext(runtime);
    auto securityEntity = &securityContext.entity();

    if (!secured_message) {
        //packet_dropped(PacketDropReason::Parse_Secured_Header);
        return nullptr;
    } else if (securityEntity) {
        // Decap packet
        using namespace vanetza::security;
        DecapConfirm decap_confirm = securityEntity->decapsulate_packet(DecapRequest { *secured_message });
        ctx.service_primitive().security_report = decap_confirm.report;
        ctx.service_primitive().its_aid = decap_confirm.its_aid;
        ctx.service_primitive().permissions = decap_confirm.permissions;

        // check whether the received packet is valid
        if (DecapReport::Success == decap_confirm.report) {
            if (vanetza::ChunkPacket* pChunk = boost::get<vanetza::ChunkPacket>(&decap_confirm.plaintext_payload)) {
                vanetza::geonet::IndicationContextSecuredCast ctx2(ctx, *pChunk);
                return indicate_common(ctx2, basic, mib);
            } else if (vanetza::CohesivePacket* pCohe = boost::get<vanetza::CohesivePacket>(&decap_confirm.plaintext_payload)) {
                vanetza::geonet::IndicationContextSecuredDeserialize ctx2(ctx, *pCohe);
                return indicate_common(ctx2, basic, mib);
            }

        } else if (vanetza::geonet::SecurityDecapHandling::Non_Strict == mib.itsGnSnDecapResultHandling) {
            // according to ETSI EN 302 636-4-1 v1.2.1 section 9.3.3 Note 2
            // handle the packet anyway, when itsGnDecapResultHandling is set to NON-Strict (1)
            switch (decap_confirm.report) {
                case DecapReport::False_Signature:
                case DecapReport::Invalid_Certificate:
                case DecapReport::Revoked_Certificate:
                case DecapReport::Inconsistant_Chain:
                case DecapReport::Invalid_Timestamp:
                case DecapReport::Invalid_Mobility_Data:
                case DecapReport::Unsigned_Message:
                case DecapReport::Signer_Certificate_Not_Found:
                case DecapReport::Unsupported_Signer_Identifier_Type:
                case DecapReport::Unencrypted_Message:
                    // ok, continue
                    if (vanetza::ChunkPacket* pChunk = boost::get<vanetza::ChunkPacket>(&decap_confirm.plaintext_payload)) {
                        vanetza::geonet::IndicationContextSecuredCast ctx2(ctx, *pChunk);
                        return indicate_common(ctx2, basic, mib);
                    } else if (vanetza::CohesivePacket* pCohe = boost::get<vanetza::CohesivePacket>(&decap_confirm.plaintext_payload)) {
                        vanetza::geonet::IndicationContextSecuredDeserialize ctx2(ctx, *pCohe);
                        return indicate_common(ctx2, basic, mib);
                    }
                    break;
                case DecapReport::Duplicate_Message:
                case DecapReport::Incompatible_Protocol:
                case DecapReport::Decryption_Error:
                default:
                    //packet_dropped(PacketDropReason::Decap_Unsuccessful_Non_Strict);
                    return nullptr;
                    break;
            }
        } else {
            // discard packet
            //packet_dropped(PacketDropReason::Decap_Unsuccessful_Strict);
            return nullptr;
        }
    } else {
        // packet_dropped(PacketDropReason::Security_Entity_Missing);
        return nullptr;
    }
    return nullptr;
}

UpPacketPtr indicate_extended(vanetza::geonet::IndicationContext& ctx, const vanetza::geonet::CommonHeader& common, vanetza::geonet::ManagementInformationBase& mib)
{
    auto extended = ctx.parse_extended(common.header_type);
    auto packet =  ctx.finish();



    if (common.next_header == vanetza::geonet::NextHeaderCommon::BTP_A) {
        vanetza::btp::parse_btp_a(*packet);
    } else if (common.next_header == vanetza::geonet::NextHeaderCommon::BTP_B) {
        vanetza::btp::parse_btp_b(*packet);
    }

    return packet;
}