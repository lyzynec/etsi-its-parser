#ifndef ETSI_ITS_PARSER_BUILD_JSON_H
#define ETSI_ITS_PARSER_BUILD_JSON_H

#include "asn1json.hpp"

typedef struct json_context {
    double time_reception;
    int rssi;
    int packet_size;
    int receiver_id;
    int receiver_type;
} json_context_t;

Document buildJSON(DENM_t message, json_context_t context);
Document buildJSON(CAM_t message, json_context_t context);
Document buildJSON(SPATEM_t message, json_context_t context);
Document buildJSON(MAPEM_t message, json_context_t context);

#endif //ETSI_ITS_PARSER_BUILD_JSON_H
