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

// Stores the Supported Rates Information Element.
typedef struct
{
    uint8_t count;       // Number of rate values found
    uint8_t values[8];   // Storage for up to 8 rate values
} supported_rates_t;

// Stores the DS Parameter Set channel information.
typedef struct
{
    uint8_t channel;
} ds_parameter_set_t;

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

// Parses an 802.11 Supported Rates Information Element.
int parse_supported_rates(
    const uint8_t *element,
    uint32_t element_length,
    supported_rates_t *result
);

// Decodes an encoded rate into units of 500 kbps.
// Returns 1 if the rate is marked as basic, otherwise 0.
int decode_supported_rate(
    uint8_t rate_byte,
    uint8_t *rate_units
);

// Parses an 802.11 DS Parameter Set Information Element.
int parse_ds_parameter_set(
    const uint8_t *element,
    uint32_t element_length,
    ds_parameter_set_t *result
);

#endif