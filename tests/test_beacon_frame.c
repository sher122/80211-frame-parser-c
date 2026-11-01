#include <stdio.h>
#include <stdint.h>
#include "frame_parser.h"

int main(void)
{
    // Complete synthetic Beacon frame:
    // 24-byte header + 12-byte fixed parameters + 14-byte IEs.
    const uint8_t beacon[] = {
        // Management Header (24 bytes)
        0x80, 0x00,             // Beacon Frame Control
        0x00, 0x00,             // Duration
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xAA, 0xBB, 0xCC, 0x11, 0x22, 0x33,
        0xAA, 0xBB, 0xCC, 0x11, 0x22, 0x33,
        0x10, 0x00,             // Sequence Control

        // Beacon Fixed Parameters (12 bytes)
        0xE8, 0x03, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, // Timestamp = 1000 us
        0x64, 0x00,             // Interval = 100 TU
        0x31, 0x04,             // Capability = 0x0431

        // Information Elements (14 bytes)
        0x00, 0x04, 'T', 'E', 'S', 'T',  // SSID
        0x01, 0x03, 0x82, 0x84, 0x8B,   // Rates
        0x03, 0x01, 0x06                // Channel 6
    };

    beacon_frame_t result;

    // Parse the complete Beacon frame.
    if (parse_beacon_frame(beacon, sizeof(beacon), &result) != 0)
    {
        printf("Beacon frame parsing failed.\n");
        return 1;
    }

    printf("Beacon frame parsing successful!\n");

    printf("Frame Type: %u\n",
           (unsigned int)result.header.control.type);

    printf("Frame Subtype: %u\n",
           (unsigned int)result.header.control.subtype);

    printf("Timestamp: %llu microseconds\n",
           (unsigned long long)result.fixed_parameters.timestamp);

    printf("Beacon Interval: %u TU\n",
           (unsigned int)result.fixed_parameters.beacon_interval);

    printf("Capability: 0x%04X\n",
           (unsigned int)result.fixed_parameters.capability_information);

    // Display SSID.
    if (result.information_elements.has_ssid)
    {
        printf("SSID: ");

        for (uint8_t i = 0;
             i < result.information_elements.ssid.length; i++)
        {
            printf("%c", result.information_elements.ssid.value[i]);
        }

        printf("\n");
    }

    // Display Supported Rates count.
    if (result.information_elements.has_supported_rates)
    {
        printf("Supported Rates Count: %u\n",
               (unsigned int)result.information_elements.supported_rates.count);
    }

    // Display Wi-Fi channel.
    if (result.information_elements.has_ds_parameter_set)
    {
        printf("Wi-Fi Channel: %u\n",
               (unsigned int)result.information_elements.ds_parameter_set.channel);
    }

    // Test rejection of a truncated Beacon.
    if (parse_beacon_frame(beacon, 35, &result) == -1)
    {
        printf("Truncated Beacon rejected successfully.\n");
    }

    // Test rejection of a non-Beacon management frame.
    uint8_t non_beacon[sizeof(beacon)];

    for (uint32_t i = 0; i < sizeof(beacon); i++)
    {
        non_beacon[i] = beacon[i];
    }

    non_beacon[0] = 0x40; // Probe Request subtype

    if (parse_beacon_frame(non_beacon, sizeof(non_beacon), &result) == -1)
    {
        printf("Non-Beacon frame rejected successfully.\n");
    }

        // Test rejection of an Information Element with an invalid length.
    uint8_t invalid_ie[sizeof(beacon)];

    for (uint32_t i = 0; i < sizeof(beacon); i++)
    {
        invalid_ie[i] = beacon[i];
    }

    // SSID length is stored at byte offset 37.
    invalid_ie[37] = 0x20; // Claims 32 bytes, but only 4 are available.

    if (parse_beacon_frame(invalid_ie, sizeof(invalid_ie), &result) == -1)
    {
        printf("Invalid IE length rejected successfully.\n");
    }
    else
    {
        printf("ERROR: Invalid IE length was accepted!\n");
        return 1;
    }

        // Test rejection of a NULL frame pointer.
    if (parse_beacon_frame(NULL, sizeof(beacon), &result) == -1)
    {
        printf("NULL frame pointer rejected successfully.\n");
    }
    else
    {
        printf("ERROR: NULL frame pointer was accepted!\n");
        return 1;
    }

    // Test rejection of a NULL output pointer.
    if (parse_beacon_frame(beacon, sizeof(beacon), NULL) == -1)
    {
        printf("NULL output pointer rejected successfully.\n");
    }
    else
    {
        printf("ERROR: NULL output pointer was accepted!\n");
        return 1;
    }

        // Test that failed parsing does not leave partial output.
    uint8_t partial_failure[sizeof(beacon)];

    for (uint32_t i = 0; i < sizeof(beacon); i++)
    {
        partial_failure[i] = beacon[i];
    }

    

    // Claim 32 SSID bytes when only 4 are available.
    partial_failure[37] = 0x20;

    // Test that failed parsing clears all partial output.
    if (parse_beacon_frame(
            partial_failure,
            sizeof(partial_failure),
            &result) == -1)
    {
        if (result.header.control.type != 0 ||
            result.fixed_parameters.beacon_interval != 0)
        {
            printf("ERROR: Partial output was not cleared!\n");
            return 1;
        }

        printf("Partial output cleared successfully.\n");
    }
    else
    {
        printf("ERROR: Invalid frame was accepted!\n");
        return 1;
    }
}