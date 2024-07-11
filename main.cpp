/* Standard Library Includes */
#include <iostream>
#include <string>

/* Boost Includes */
#include <boost/optional.hpp>

/* PcapPlusPlus Includes */
// TODO

/* RapidJSON Includes */
// TODO

/* Vanetza Includes */
#include <vanetza/asn1/packet_visitor.hpp>
#include <vanetza/asn1/denm.hpp>
#include <vanetza/asn1/cam.hpp>
#include <vanetza/asn1/spatem.hpp>
#include <vanetza/asn1/mapem.hpp>
#include <vanetza/asn1/ivim.hpp>
#include <vanetza/asn1/srem.hpp>
#include <vanetza/asn1/ssem.hpp>
#include <vanetza/asn1/cpm.hpp>



#include "headers.h"
#include "indicator.h"
#include "message_id.h"


boost::optional<std::string> parse_msg_radiotap(vanetza::ByteBuffer byteBuffer) {

    size_t header_length = 0;
    auto radiotap = *(radiotap_header_t*)&(byteBuffer[header_length]);
    header_length += sizeof(radiotap_header_t) + (size_t)radiotap.vendor_data_length;

    auto ieee_qos = *(IEEE_QoS_t*)&(byteBuffer[header_length]);
    header_length += sizeof(IEEE_QoS_t);

    auto logical_link_control = *(logical_link_control_t *)&(byteBuffer[header_length]);
    header_length += sizeof(logical_link_control_t);

    byteBuffer.erase(byteBuffer.begin(), byteBuffer.begin() + (long)header_length);

    auto packet = std::make_unique<vanetza::geonet::UpPacket>(vanetza::CohesivePacket { std::move(byteBuffer), vanetza::OsiLayer::Network });
    std::initializer_list<uint8_t> sender = {
            ieee_qos.source_mac[0],
            ieee_qos.source_mac[1],
            ieee_qos.source_mac[2],
            ieee_qos.source_mac[3],
            ieee_qos.source_mac[4],
            ieee_qos.source_mac[5],
    };
    std::initializer_list<uint8_t> destination = {
            ieee_qos.destination_mac[0],
            ieee_qos.destination_mac[1],
            ieee_qos.destination_mac[2],
            ieee_qos.destination_mac[3],
            ieee_qos.destination_mac[4],
            ieee_qos.destination_mac[5],
    };
    auto finishedPacketPtr = indicate(std::move(packet), sender, destination);

    if (finishedPacketPtr == nullptr) {
        return boost::none;
    }

    auto finishedPacket = *finishedPacketPtr;

    uint8_t messageID;
    if (vanetza::ChunkPacket* pChunk = boost::get<vanetza::ChunkPacket>(&finishedPacket)) {
        // there is something really wrong if this happens
        return boost::none;
    } else if (vanetza::CohesivePacket* pCohes = boost::get<vanetza::CohesivePacket>(&finishedPacket)) {
        auto applicationBuffer = (*pCohes)[vanetza::OsiLayer::Application];
        messageID = applicationBuffer[1];
    } else {
        // this time its wrong like if the universe ended bro
        return boost::none;
    }

    std::string result;

    if (messageID == MESSAGE_ID_DENM) {
        vanetza::asn1::PacketVisitor<vanetza::asn1::Denm> visitor;
        std::shared_ptr<const vanetza::asn1::Denm> denm = boost::apply_visitor(visitor, finishedPacket);
        if (denm == nullptr) {
            return boost::none;
        }

        DENM_t cDenm = {(*denm)->header, (*denm)->denm};

    } else if (messageID == MESSAGE_ID_CAM) {
        vanetza::asn1::PacketVisitor<vanetza::asn1::Cam> visitor;
        std::shared_ptr<const vanetza::asn1::Cam> cam = boost::apply_visitor(visitor, finishedPacket);
        if (cam == nullptr) {
            return boost::none;
        }

        CAM_t cCam = {(*cam)->header, (*cam)->cam};

    } else if (messageID == MESSAGE_ID_SPATEM) {
        vanetza::asn1::PacketVisitor<vanetza::asn1::Spatem> visitor;
        std::shared_ptr<const vanetza::asn1::Spatem> spatem = boost::apply_visitor(visitor, finishedPacket);
        if (spatem == nullptr) {
            return boost::none;
        }

        SPATEM_t cSpatem = {(*spatem)->header, (*spatem)->spat};

    } else if (messageID == MESSAGE_ID_MAPEM) {
        vanetza::asn1::PacketVisitor<vanetza::asn1::Mapem> visitor;
        std::shared_ptr<const vanetza::asn1::Mapem> mapem = boost::apply_visitor(visitor, finishedPacket);
        if (mapem == nullptr) {
            return boost::none;
        }

        MAPEM_t cMapem = {(*mapem)->header, (*mapem)->map};

    } else if (messageID == MESSAGE_ID_IVIM) {
        vanetza::asn1::PacketVisitor<vanetza::asn1::Ivim> visitor;
        std::shared_ptr<const vanetza::asn1::Ivim> ivim = boost::apply_visitor(visitor, finishedPacket);
        if (ivim == nullptr) {
            return boost::none;
        }

        IVIM_t cIvim = {(*ivim)->header, (*ivim)->ivi};

    } else if (messageID == MESSAGE_ID_SREM) {
        vanetza::asn1::PacketVisitor<vanetza::asn1::Srem> visitor;
        std::shared_ptr<const vanetza::asn1::Srem> srem = boost::apply_visitor(visitor, finishedPacket);
        if (srem == nullptr) {
            return boost::none;
        }

        SREM_t cSrem = {(*srem)->header, (*srem)->srm};

    } else if (messageID == MESSAGE_ID_SSEM) {
        vanetza::asn1::PacketVisitor<vanetza::asn1::Ssem> visitor;
        std::shared_ptr<const vanetza::asn1::Ssem> ssem = boost::apply_visitor(visitor, finishedPacket);
        if (ssem == nullptr) {
            return boost::none;
        }

        SSEM_t cSsem = {(*ssem)->header, (*ssem)->ssm};

    } else if (messageID == MESSAGE_ID_CPM) {
        vanetza::asn1::PacketVisitor<vanetza::asn1::Cpm> visitor;
        std::shared_ptr<const vanetza::asn1::Cpm> cpm = boost::apply_visitor(visitor, finishedPacket);
        if (cpm == nullptr) {
            return boost::none;
        }

        CPM_t cCpm = {(*cpm)->header, (*cpm)->cpm};

    }


    return result;
}

int main() {
    return 0;
}