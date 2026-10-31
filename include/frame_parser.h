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

// Decodes the two Frame Control bytes
int parse_frame_control(
    const uint8_t *frame,
    uint32_t frame_length,
    frame_control_t *result
);

#endif