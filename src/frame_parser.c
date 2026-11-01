#include "frame_parser.h"

int parse_frame_control(
    const uint8_t *frame,
    uint32_t frame_length,
    frame_control_t *result
)
{
    // Validate pointers and ensure at least 2 bytes are available
    if (frame == NULL || result == NULL || frame_length < 2)
    {
        return -1;
    }

    // Combine the two Frame Control bytes (little-endian)
    uint16_t frame_control =
        (uint16_t)frame[0] |
        ((uint16_t)frame[1] << 8);

    // Extract protocol version, type, and subtype
    result->protocol_version = frame_control & 0x03;
    result->type = (frame_control >> 2) & 0x03;
    result->subtype = (frame_control >> 4) & 0x0F;

    return 0;
}
int parse_management_header(
    const uint8_t *frame,
    uint32_t frame_length,
    management_header_t *result
)
{
    // Reject NULL pointers and incomplete management headers
    if (frame == NULL || result == NULL || frame_length < 24)
    {
        return -1;
    }

    // Decode the Frame Control field
    if (parse_frame_control(frame, frame_length, &result->control) != 0)
    {
        return -1;
    }
    // Accept only Management frames (Type = 0)
    if (result->control.type != 0)
    {
        return -1;
    }

    // Read the Duration field (bytes 2 and 3)
    result->duration =
        (uint16_t)frame[2] |
        ((uint16_t)frame[3] << 8);

    // Copy Address 1: Receiver (bytes 4-9)
    for (int i = 0; i < 6; i++)
    {
        result->receiver[i] = frame[4 + i];
    }

    // Copy Address 2: Transmitter (bytes 10-15)
    for (int i = 0; i < 6; i++)
    {
        result->transmitter[i] = frame[10 + i];
    }

    // Copy Address 3: BSSID (bytes 16-21)
    for (int i = 0; i < 6; i++)
    {
        result->bssid[i] = frame[16 + i];
    }

    // Read Sequence Control (bytes 22 and 23)
    result->sequence_control =
        (uint16_t)frame[22] |
        ((uint16_t)frame[23] << 8);

    return 0;
}

int parse_ssid_element(
    const uint8_t *element,
    uint32_t element_length,
    ssid_info_t *result
)
{
    // Reject NULL pointers and elements shorter than the ID and Length fields
    if (element == NULL || result == NULL || element_length < 2)
    {
        return -1;
    }

    // Element ID 0 identifies an SSID
    if (element[0] != 0)
    {
        return -1;
    }

    // Read the SSID value length
    uint8_t ssid_length = element[1];

    // Ensure the declared value is completely available
    if (ssid_length > 32 || element_length < (uint32_t)(2 + ssid_length))
    {
        return -1;
    }

    // Store the SSID length
    result->length = ssid_length;

    // Copy the SSID bytes into our result structure
    for (uint8_t i = 0; i < ssid_length; i++)
    {
        result->value[i] = element[2 + i];
    }

    return 0;
}

// Parses an 802.11 Supported Rates Information Element.
int parse_supported_rates(
    const uint8_t *element,
    uint32_t element_length,
    supported_rates_t *result
)
{
    // Reject NULL pointers and elements shorter than 2 bytes.
    if (element == NULL || result == NULL || element_length < 2)
    {
        return -1;
    }

    // Element ID 1 identifies the Supported Rates element.
    if (element[0] != 1)
    {
        return -1;
    }

    // Read the number of rate values.
    uint8_t rate_count = element[1];

    // Supported Rates can contain at most 8 values.
    if (rate_count > 8)
    {
        return -1;
    }

    // Check that all declared rate bytes are available.
    if (element_length < (uint32_t)(2 + rate_count))
    {
        return -1;
    }

    // Store the number of rates.
    result->count = rate_count;

    // Copy each encoded rate byte into the result.
    for (uint8_t i = 0; i < rate_count; i++)
    {
        result->values[i] = element[2 + i];
    }

    return 0;
}

// Decodes a rate byte into 500 kbps units.
int decode_supported_rate(uint8_t rate_byte, uint8_t *rate_units)
{
    // Reject a NULL output pointer.
    if (rate_units == NULL)
    {
        return -1;
    }

    // Keep only the lower 7 bits; bit 7 is the Basic Rate flag.
    *rate_units = rate_byte & 0x7F;

    // Return 1 for Basic Rate, 0 otherwise.
    return (rate_byte & 0x80) ? 1 : 0;
}