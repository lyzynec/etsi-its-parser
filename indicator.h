#ifndef ETSI_ITS_PARSER_INDICATOR_H
#define ETSI_ITS_PARSER_INDICATOR_H

#include <vanetza/geonet/packet.hpp>
#include <vanetza/net/mac_address.hpp>

typedef std::unique_ptr<vanetza::UpPacket> UpPacketPtr;

UpPacketPtr indicate(UpPacketPtr packet, const vanetza::MacAddress& sender, const vanetza::MacAddress& destination);

#endif //ETSI_ITS_PARSER_INDICATOR_H
