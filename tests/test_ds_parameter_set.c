#include <stdio.h>
#include <stdint.h>
#include "frame_parser.h"

int main(void)
{
    // DS Parameter Set: Element ID 3, length 1, channel 6.
    const uint8_t channel_element[] = {0x03, 0x01, 0x06};

    ds_parameter_set_t result;

    if (parse_ds_parameter_set(
            channel_element,
            sizeof(channel_element),
            &result) != 0)
    {
        printf("Channel parsing failed.\n");
        return 1;
    }

    printf("Wi-Fi Channel: %u\n", (unsigned int)result.channel);

    // Reject an incorrect Element ID.
    const uint8_t invalid_id[] = {0x01, 0x01, 0x06};

    if (parse_ds_parameter_set(invalid_id, sizeof(invalid_id), &result) == -1)
    {
        printf("Invalid Element ID rejected.\n");
    }

    // Reject truncated channel information.
    const uint8_t truncated[] = {0x03, 0x01};

    if (parse_ds_parameter_set(truncated, sizeof(truncated), &result) == -1)
    {
        printf("Truncated element rejected.\n");
    }

    // Reject an incorrect declared length.
    const uint8_t invalid_length[] = {0x03, 0x02, 0x06, 0x07};

    if (parse_ds_parameter_set(invalid_length, sizeof(invalid_length), &result) == -1)
    {
        printf("Invalid element length rejected.\n");
    }

    return 0;
}