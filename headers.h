#ifndef ETSI_ITS_PARSER_HEADERS_H
#define ETSI_ITS_PARSER_HEADERS_H

#include <cinttypes>

typedef enum __btpb_port
{
    BTP_B_PORT_CAM = 2001,
    BTP_B_PORT_DENM = 2002,
    BTP_B_PORT_MAPEM = 2003,
    BTP_B_PORT_SPATEM = 2004,
    BTP_B_PORT_SAEM = 2005,
    BTP_B_PORT_IVIM = 2006,
    BTP_B_PORT_SREM = 2007,
    BTP_B_PORT_SSEM = 2008,
    BTP_B_PORT_EVCSN_POI = 2010,
    BTP_B_PORT_TPG = 2011,
    BTP_B_PORT_TRM = BTP_B_PORT_TPG,
    BTP_B_PORT_TCM = BTP_B_PORT_TPG,
    BTP_B_PORT_VDRM = BTP_B_PORT_TPG,
    BTP_B_PORT_VDPM = BTP_B_PORT_TPG,
    BTP_B_PORT_EOFM = BTP_B_PORT_TPG,
    BTP_B_PORT_EVRSR = 2012,
    BTP_B_PORT_RTCMEM = 2013,
    BTP_B_PORT_CTLM = 2014,
    BTP_B_PORT_CRLM = 2015,
    BTP_B_PORT_EC = 2016,
    BTP_B_PORT_AT = BTP_B_PORT_EC
} btpb_port_t;

typedef struct __radiotap_header
{
    uint8_t revision;
    uint8_t pad;
    uint16_t length;
    union
    {
        uint32_t raw;
        struct
        {
            uint8_t TSFT:1;
            uint8_t flags:1;
            uint8_t	rate:1;
            uint8_t channel:1;
            uint8_t FHSS:1;
            uint8_t dBm_antenna_signal:1;
            uint8_t dBm_antenna_noise:1;
            uint8_t lock_quality:1;
            uint8_t tx_attenuation:1;
            uint8_t dB_tx_attenuation:1;
            uint8_t dBm_tx_power:1;
            uint8_t antenna:1;
            uint8_t dB_antenna_signal:1;
            uint8_t dBantenna_noise:1;
            uint8_t rx_flags:1;
            uint8_t tx_flags:1;
            uint8_t reserved:1;
            uint8_t data_retries:1;
            uint8_t channel_plus:1;
            uint8_t MSC_info:1;
            uint8_t AMPDU_status:1;
            uint8_t VHT_info:1;
            uint8_t frame_timestamp:1;
            uint8_t HE_info:1;
            uint8_t HEMU_info:1;
            uint8_t length_PSDU:1;
            uint8_t LSIG:1;
            uint8_t reserved2:1;
            uint8_t TLVs:1;
            uint8_t radiotap_NS_next:1;
            uint8_t vendor_NS_next:1;
            uint8_t ext:1;
        }fields;
    }present_flags;
    union
    {
        uint8_t raw;
        struct
        {
            uint8_t CFP:1;
            uint8_t preamble:1;
            uint8_t WEP:1;
            uint8_t fragmentation:1;
            uint8_t FCS_at_end:1;
            uint8_t data_pad:1;
            uint8_t bad_FCS:1;
            uint8_t short_GI:1;
        }fields;
    }flags;
    uint8_t vendor_OUI[3];
    uint8_t vendor_sub_namespace;
    uint16_t vendor_data_length;
    // uint8_t vendor_data[32];
    // uint8_t vendor_data[56];
    uint8_t vendor_data[];
}radiotap_header_t;

typedef struct __IEEE_QoS
{
    struct
    {
        union
        {
            uint8_t raw;
            struct
            {
                uint8_t version:2;
                uint8_t type:2;
                uint8_t subtype:4;
            }fields;
        }info;
        union
        {
            uint8_t raw;
            struct
            {
                uint8_t DS_status:1;
                uint8_t more_fragments:1;
                uint8_t retry:1;
                uint8_t PWR_MGT:1;
                uint8_t more_data:1;
                uint8_t protected_flag:1;
                uint8_t HTC_order_flag:1;
            }fields;
        }flags;
    }frame_control_field;
    uint16_t duration_ms;
    uint8_t destination_mac[6];
    uint8_t source_mac[6];
    uint8_t BSS_id[6];
    union
    {
        uint16_t raw;
        struct
        {
            uint8_t fragment_number:4;
            uint16_t sequence_number:12;
        }fields;
    }sequence_fragment_number;
    union
    {
        uint16_t raw;
        struct
        {
            uint8_t priority:3;
            uint8_t reserved:1;
            uint8_t QoS_bit:1;
            uint8_t ack_policy:2;
            uint8_t payload_type:1;
            uint8_t TXOP_duration_request;
        }fields;
    }QoS_control;
}__attribute((packed))IEEE_QoS_t;

typedef struct __LLC
{
    union
    {
        uint8_t raw;
        struct
        {
            uint8_t SAP:7;
            uint8_t ig_bit:1;
        }fields;
    }DSAP;
    union
    {
        uint8_t raw;
        struct
        {
            uint8_t SAP:7;
            uint8_t cr_bit:1;
        }fields;
    }SSAP;
    union
    {
        uint8_t raw;
        struct
        {
            uint8_t frame_type:1;
            uint8_t cmd:7;
        }fields;
    }control_field;
    uint8_t organization_code[3];
    uint16_t type;
}logical_link_control_t;

typedef struct __ethernet
{
    uint8_t destination_mac[6];
    uint8_t source_mac[6];
    uint16_t type;
    uint8_t data[];
} __attribute__((packed)) ethernet_t;

typedef enum
{
    GEONET_BASIC_HEADER_NEXT_ANY = 0,
    GEONET_BASIC_HEADER_NEXT_COMMON = 1,
    GEONET_BASIC_HEADER_NEXT_SECURED = 2,
} geonet_basic_header_next_t;

typedef enum
{
    GEONET_BASIC_HEADER_LIFETIME_BASE_50MS = 0,
    GEONET_BASIC_HEADER_LIFETIME_BASE_1S = 1,
    GEONET_BASIC_HEADER_LIFETIME_BASE_10S = 2,
    GEONET_BASIC_HEADER_LIFETIME_BASE_100S = 3,
} geonet_basic_header_lifetime_base_t;

typedef enum
{
    GEONET_COMMON_HEADER_NEXT_ANY = 0,
    GEONET_COMMON_HEADER_NEXT_BTP_A = 1,
    GEONET_COMMON_HEADER_NEXT_BTP_B = 2,
    GEONET_COMMON_HEADER_NEXT_IPV6 = 3,
} geonet_common_header_next_t;

typedef enum __geonet_type
{
    GEONET_TYPE_ANY = 0x00,
    GEONET_TYPE_BEACON = 0x10,
    GEONET_TYPE_GUC = 0x20,
    GEONET_TYPE_GAC_CIRCLE = 0x30,
    GEONET_TYPE_GAC_RECT = 0x31,
    GEONET_TYPE_GAC_ELLIPSE = 0x32,
    GEONET_TYPE_GBC_CIRCLE = 0x40,
    GEONET_TYPE_GBC_RECT = 0x41,
    GEONET_TYPE_GBC_ELLIPSE = 0x42,
    GEONET_TYPE_TSB_SHB = 0x50,
    GEONET_TYPE_TSB_MHB = 0x51,
    GEONET_TYPE_LS_REQUEST = 0x60,
    GEONET_TYPE_LS_REPLY = 0x61,
} geonet_type_t;

typedef struct __geonetworking
{
    struct {
        uint8_t next_header:4;
        uint8_t version:4;
        uint8_t reserved;
        union {
            uint8_t raw;
            struct {
                uint8_t base:2;
                uint8_t mult:6;
            } fields;
        } life_time;
        uint8_t remaining_hop_limit;
    } basic_header;
    uint8_t data[];
} __attribute__((packed)) geonetworking_t;

typedef struct __genonetworking_common_header
{
    uint8_t reserved:4;
    uint8_t next_header:4;
    union {
        uint8_t raw;
        struct {
            uint8_t sub_type:4;
            uint8_t type:4;
        } fields;
    } type;
    union {
        uint8_t raw;
        struct {
            uint8_t id:6;
            uint8_t offload:1;
            uint8_t scf:1;
        } fields;
    } traffic_class;
    union {
        uint8_t raw;
        struct {
            uint8_t todo;
        } fields;
    } flags;
    uint16_t payload_length;
    uint8_t max_hop_limit;
    uint8_t reserved2;
    uint8_t data[];
} __attribute__((packed)) geonetworking_common_header_t;

typedef struct __genonetworking_secured_packet
{
    uint8_t todo[7];
    uint8_t data[];
} __attribute__((packed)) geonetworking_secured_packet_t;

typedef struct __geonet_long_position_vector
{
    uint64_t address;
    uint32_t timestamp;
    uint32_t latitude;
    uint32_t longitude;
    struct {
        uint8_t accuracy:1;
        uint16_t speed:15;
    } accuracy_speed;
    uint16_t heading;
} __attribute__((packed)) geonet_long_position_vector_t;

typedef struct __geonet_short_position_vector
{
    uint64_t address;
    uint32_t timestamp;
    uint32_t latitude;
    uint32_t longitude;
} __attribute__((packed)) geonet_short_position_vector_t;

typedef struct
{
    geonet_long_position_vector_t source_position;
    uint8_t data[];
} __attribute__((packed)) geonet_beacon_t;

typedef struct
{
    uint16_t sequence_number;
    uint16_t reserved;
    geonet_long_position_vector_t source_position;
    geonet_short_position_vector_t destination;
    uint8_t data[];
} __attribute__((packed)) geonet_guc_t;

typedef struct
{
    uint16_t sequence_number;
    uint16_t reserved;
    geonet_long_position_vector_t source_position;
    uint32_t latitude;
    uint32_t longitude;
    uint16_t distance_a;
    uint16_t distance_b;
    uint16_t angle;
    uint16_t reserved2;
    uint8_t data[];
} __attribute__((packed)) geonet_gac_t;

typedef struct
{
    uint16_t sequence_number;
    uint16_t reserved;
    geonet_long_position_vector_t source_position;
    uint8_t data[];
} __attribute__((packed)) geonet_tsb_mhb_t;

typedef struct
{
    geonet_long_position_vector_t source_position;
    uint32_t media_dependent_data;
    uint8_t data[];
} __attribute__((packed)) geonet_tsb_shb_t;

typedef struct __btp_b
{
    uint16_t port;
    uint16_t port_info;
    uint8_t data[];
} btp_b_t;


#endif //ETSI_ITS_PARSER_HEADERS_H
