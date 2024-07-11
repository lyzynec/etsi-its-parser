#include "build_json.h"

Document buildJSON(DENM_t message, json_context_t context) {

    ITS_Container_ItsPduHeader_t &header = message.header;

    Document document(kObjectType);
    Document::AllocatorType &allocator = document.GetAllocator();

    document.AddMember("timestamp", context.time_reception, allocator)
            .AddMember("rssi", context.rssi, allocator)
            .AddMember("stationID", Value(static_cast<int64_t>(header.stationID)), allocator)
            .AddMember("receiverID", context.receiver_id, allocator)
            .AddMember("receiverType", context.receiver_type, allocator)
            .AddMember("packet_size", context.packet_size, allocator)
            .AddMember("fields", to_json(message, allocator), allocator);

    return document;
}

Document buildJSON(CAM_t message, json_context_t context) {

    ITS_Container_ItsPduHeader_t &header = message.header;

    Document document(kObjectType);
    Document::AllocatorType &fullAllocator = document.GetAllocator();

    //.AddMember("newInfo", new_info, fullAllocator)
    document.AddMember("timestamp", context.time_reception, fullAllocator)
            .AddMember("rssi", context.rssi, fullAllocator)
            .AddMember("stationID", Value(static_cast<int64_t>(header.stationID)), fullAllocator)
            .AddMember("receiverID", context.receiver_id, fullAllocator)
            .AddMember("receiverType", context.receiver_type, fullAllocator)
            .AddMember("packet_size", context.packet_size, fullAllocator)
            .AddMember("fields", to_json(message, fullAllocator), fullAllocator);

    return document;
}

Document buildJSON(SPATEM_t message, json_context_t context) {

    ITS_Container_ItsPduHeader_t &header = message.header;

    Document document(kObjectType);
    Document::AllocatorType &allocator = document.GetAllocator();

    document.AddMember("timestamp", context.time_reception, allocator)
            .AddMember("rssi", context.rssi, allocator)
            .AddMember("stationID", Value(static_cast<int64_t>(header.stationID)), allocator)
            .AddMember("receiverID", context.receiver_id, allocator)
            .AddMember("receiverType", context.receiver_type, allocator)
            .AddMember("packet_size", context.packet_size, allocator)
            .AddMember("fields", to_json(message, allocator), allocator);

    return document;
}

Document buildJSON(MAPEM_t message, json_context_t context) {

    ITS_Container_ItsPduHeader_t &header = message.header;

    Document document(kObjectType);
    Document::AllocatorType &allocator = document.GetAllocator();


    document.AddMember("timestamp", context.time_reception, allocator)
            .AddMember("rssi", context.rssi, allocator)
            .AddMember("stationID", Value(static_cast<int64_t>(header.stationID)), allocator)
            .AddMember("receiverID", context.receiver_id, allocator)
            .AddMember("receiverType", context.receiver_type, allocator)
            .AddMember("packet_size", context.packet_size, allocator)
            .AddMember("fields", to_json(message, allocator), allocator);

    return document;
}




