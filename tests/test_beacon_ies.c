#include <stdio.h>
#include <stdint.h>
#include "frame_parser.h"

int main(void)
{
    // Three consecutive Information Elements:
    // SSID = TEST, Supported Rates = 3 values, Channel = 6.
    const uint8_t beacon_ies[] = {
        0x00, 0x04, 'T', 'E', 'S', 'T',
        0x01, 0x03, 0x82, 0x84, 0x8B,
        0x03, 0x01, 0x06
    };

    beacon_info_t result;

    // Parse all Information Elements together.
    if (parse_beacon_information_elements(
            beacon_ies,
            sizeof(beacon_ies),
            &result) != 0)
    {
        printf("Beacon IE parsing failed.\n");
        return 1;
    }

    // Display SSID.
    if (result.has_ssid)
    {
        printf("SSID: ");

        for (uint8_t i = 0; i < result.ssid.length; i++)
        {
            printf("%c", result.ssid.value[i]);
        }

        printf("\n");
    }

    // Display Supported Rates count.
    if (result.has_supported_rates)
    {
        printf("Supported Rates Count: %u\n",
               (unsigned int)result.supported_rates.count);
    }

    // Display channel.
    if (result.has_ds_parameter_set)
    {
        printf("Wi-Fi Channel: %u\n",
               (unsigned int)result.ds_parameter_set.channel);
    }

    printf("Beacon IE parsing successful.\n");


        // Test truncated IE header (only Element ID is present).
    const uint8_t truncated_header[] = {0x00};

    if (parse_beacon_information_elements(
            truncated_header,
            sizeof(truncated_header),
            &result) == -1)
    {
        printf("Truncated IE header rejected.\n");
    }

    // Test IE whose declared length exceeds available data.
    const uint8_t truncated_data[] = {
        0x00, 0x05, 'T', 'E'
    };

    if (parse_beacon_information_elements(
            truncated_data,
            sizeof(truncated_data),
            &result) == -1)
    {
        printf("Truncated IE data rejected.\n");
    }

    // Test unknown IE: it should be safely skipped.
    const uint8_t unknown_ie[] = {
        0xFF, 0x02, 0xAA, 0xBB
    };

    if (parse_beacon_information_elements(
            unknown_ie,
            sizeof(unknown_ie),
            &result) == 0)
    {
        printf("Unknown IE skipped successfully.\n");
    }
    return 0;
}