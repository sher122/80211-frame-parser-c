#ifndef FRAME_PARSER_H
#define FRAME_PARSER_H

#include <stdint.h>

// Stores the decoded Frame Control information
typedef struct
{
    uint8_t protocol_version;
    uint8_t type;
    uint8_t subtype;
} frame_control_t;

// Stores the important fields of a management frame header
typedef struct
{
    frame_control_t control;

    uint16_t duration;

    uint8_t receiver[6];     // Address 1
    uint8_t transmitter[6];  // Address 2
    uint8_t bssid[6];        // Address 3

    uint16_t sequence_control;

} management_header_t;

// Stores the decoded SSID information
typedef struct
{
    uint8_t length;
    uint8_t value[32];
} ssid_info_t;

// Decodes the two Frame Control bytes
int parse_frame_control(
    const uint8_t *frame,
    uint32_t frame_length,
    frame_control_t *result
);

// Parses a complete 24-byte management frame header
int parse_management_header(
    const uint8_t *frame,
    uint32_t frame_length,
    management_header_t *result
);

// Parses an SSID Information Element
int parse_ssid_element(
    const uint8_t *element,
    uint32_t element_length,
    ssid_info_t *result
);

#endif