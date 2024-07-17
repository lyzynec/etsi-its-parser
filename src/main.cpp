/* Standard Library Includes */
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

/* Boost Includes */
#include <boost/optional.hpp>

/* PcapPlusPlus Includes */
#include <pcap.h>

/* Vanetza Includes */
#include "vanetza/asn1/packet_visitor.hpp"
#include "vanetza/asn1/denm.hpp"
#include "vanetza/asn1/cam.hpp"
#include "vanetza/asn1/spatem.hpp"
#include "vanetza/asn1/mapem.hpp"
#include "vanetza/asn1/ssem.hpp"

/* Local Includes */
#include "headers.h"
#include "indicator.h"
#include "message_id.h"

#include "build_json.h"

boost::optional<std::string> parse_msg_radiotap(vanetza::ByteBuffer byteBuffer, double time_reception) {

    json_context_t context {
            .time_reception = time_reception,
    };

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

    Document json_document;

    bool found = false;
    if (!found) {
        vanetza::asn1::PacketVisitor<vanetza::asn1::Denm> visitor;
        std::shared_ptr<const vanetza::asn1::Denm> denm = boost::apply_visitor(visitor, finishedPacket);
        if (denm != nullptr) {
            DENM_t cDenm = {(*denm)->header, (*denm)->denm};

            json_document = buildJSON(cDenm, context);

            found = true;
        }
    }

    if (!found) {
        vanetza::asn1::PacketVisitor<vanetza::asn1::Srem> visitor;
        std::shared_ptr<const vanetza::asn1::Srem> srem = boost::apply_visitor(visitor, finishedPacket);
        if (srem != nullptr) {
            SREM_t cSrem = {(*srem)->header, (*srem)->srm};

            json_document = buildJSON(cSrem, context);

            found = true;
        }
    }

    if (!found) {
        vanetza::asn1::PacketVisitor<vanetza::asn1::Cam> visitor;
        std::shared_ptr<const vanetza::asn1::Cam> cam = boost::apply_visitor(visitor, finishedPacket);
        if (cam != nullptr) {
            CAM_t cCam = {(*cam)->header, (*cam)->cam};

            json_document = buildJSON(cCam, context);

            found = true;
        }
    }

    if (!found) {
        vanetza::asn1::PacketVisitor<vanetza::asn1::Spatem> visitor;
        std::shared_ptr<const vanetza::asn1::Spatem> spatem = boost::apply_visitor(visitor, finishedPacket);
        if (spatem != nullptr) {
            SPATEM_t cSpatem = {(*spatem)->header, (*spatem)->spat};

            json_document = buildJSON(cSpatem, context);

            found = true;
        }
    }



    if (!found) {
        vanetza::asn1::PacketVisitor<vanetza::asn1::Mapem> visitor;
        std::shared_ptr<const vanetza::asn1::Mapem> mapem = boost::apply_visitor(visitor, finishedPacket);
        if (mapem != nullptr) {
            MAPEM_t cMapem = {(*mapem)->header, (*mapem)->map};

            json_document = buildJSON(cMapem, context);

            found = true;
        }
    }

    if (!found) {
        return boost::none;
    }

    // This is just conversion to string to simplify my life.
    StringBuffer strbuf;
    strbuf.Clear();

    Writer<StringBuffer> writer(strbuf);
    json_document.Accept(writer);

    std::string out = strbuf.GetString();

    return out;
}

typedef struct user_data {
    std::ofstream* output_stream;
    size_t message_counter;
} user_data_t;

void pcap_loop_callback(u_char *pUserData, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    auto userData = (user_data_t *) (pUserData);


    vanetza::ByteBuffer byteBuffer(packet, packet + pkthdr->len);

    double timestamp = (double)(pkthdr->ts.tv_sec) + (double)(pkthdr->ts.tv_usec) / 1e6;
    auto res = parse_msg_radiotap(byteBuffer, timestamp);

    if ( res.has_value() ) {
        if (userData->message_counter > 0) {
            *userData->output_stream << ",";
        }
        *userData->output_stream << res.value();
        userData->message_counter += 1;
    }

}

// etsi-its-parser [input file] [output file]
int main(int argc, char **argv) {

    if (argc != 3) {
        std::cerr
                << "ERROR: Not enough arguments provided, provide them in format etsi-its-parser [input file] [output file]."
                << std::endl;

        return EXIT_FAILURE;
    }

    {
        std::filesystem::path inputFilePath(argv[1]);

        if (!std::filesystem::exists(inputFilePath)) {
            std::cerr << "ERROR: Input file does not exist." << std::endl;

            return EXIT_FAILURE;
        }
    }

    std::ofstream outputFile(argv[2]);
    if (!outputFile.is_open()) {
        std::cerr << "ERROR: Could not open output file." << std::endl;
    }

    user_data_t userData {
        .output_stream = &outputFile,
        .message_counter = 0,
    };

    char pcap_errbuf[PCAP_ERRBUF_SIZE];
    auto pcap_fp = pcap_open_offline(argv[1], pcap_errbuf);

    if (pcap_fp == nullptr) {
        std::cerr << "ERROR: Failure to open pcap file: " << pcap_errbuf << std::endl;

        return EXIT_FAILURE;
    }

    outputFile << "[";
    if (pcap_loop(pcap_fp, 0, pcap_loop_callback, (u_char *) &userData) < 0) {
        std::cerr << "ERROR: There was error in pcap_loop: " << pcap_errbuf << std::endl;
    }
    outputFile << "]";

    std::cout << "Parsed " << userData.message_counter << " messages." << std::endl;
    
    return EXIT_SUCCESS;
}