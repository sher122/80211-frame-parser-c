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