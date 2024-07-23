#include "build_json.h"

Document buildJSON(DENM_t message, json_context_t context) {

    Document document(kObjectType);
    Document::AllocatorType &allocator = document.GetAllocator();

    document.AddMember("timeStamp", context.time_reception, allocator)
            .AddMember("fields", to_json(message, allocator), allocator);

    return document;
}

Document buildJSON(SREM_t message, json_context_t context) {

    Document document(kObjectType);
    Document::AllocatorType &allocator = document.GetAllocator();

    document.AddMember("timeStamp", context.time_reception, allocator)
            .AddMember("fields", to_json(message, allocator), allocator);

    return document;
}

Document buildJSON(SSEM_t message, json_context_t context) {

    Document document(kObjectType);
    Document::AllocatorType &allocator = document.GetAllocator();

    document.AddMember("timeStamp", context.time_reception, allocator)
            .AddMember("fields", to_json(message, allocator), allocator);

    return document;
}

Document buildJSON(CAM_t message, json_context_t context) {

    Document document(kObjectType);
    Document::AllocatorType &allocator = document.GetAllocator();

    document.AddMember("timeStamp", context.time_reception, allocator)
        .AddMember("fields", to_json(message, allocator), allocator);

    return document;
}

Document buildJSON(SPATEM_t message, json_context_t context) {

    Document document(kObjectType);
    Document::AllocatorType &allocator = document.GetAllocator();

    document.AddMember("timeStamp", context.time_reception, allocator)
            .AddMember("fields", to_json(message, allocator), allocator);

    return document;
}

Document buildJSON(MAPEM_t message, json_context_t context) {

    Document document(kObjectType);
    Document::AllocatorType &allocator = document.GetAllocator();

    document.AddMember("timeStamp", context.time_reception, allocator)
            .AddMember("fields", to_json(message, allocator), allocator);

    return document;
}




