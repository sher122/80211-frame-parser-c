#include <stdio.h>
#include <stdint.h>
#include "frame_parser.h"

int main(void)
{
    // Synthetic Beacon: 24-byte header + 12 fixed bytes.
    const uint8_t beacon[36] = {
        0x80, 0x00,             // Beacon Frame Control
        0x00, 0x00,             // Duration
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xAA, 0xBB, 0xCC, 0x11, 0x22, 0x33,
        0xAA, 0xBB, 0xCC, 0x11, 0x22, 0x33,
        0x10, 0x00,             // Sequence Control

        // Timestamp = 1000 microseconds
        0xE8, 0x03, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,

        // Beacon Interval = 100 TU
        0x64, 0x00,

        // Capability Information = 0x0431
        0x31, 0x04
    };

    beacon_fixed_parameters_t result;

    if (parse_beacon_fixed_parameters(
            beacon, sizeof(beacon), &result) != 0)
    {
        printf("Beacon fixed parameter parsing failed.\n");
        return 1;
    }

    printf("Timestamp: %llu microseconds\n",
           (unsigned long long)result.timestamp);

    printf("Beacon Interval: %u TU\n",
           (unsigned int)result.beacon_interval);

    printf("Capability Information: 0x%04X\n",
           (unsigned int)result.capability_information);

    // Reject a truncated Beacon.
    if (parse_beacon_fixed_parameters(beacon, 35, &result) == -1)
    {
        printf("Truncated Beacon rejected successfully.\n");
    }

    return 0;
}