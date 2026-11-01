#include <stdio.h>
#include <stdint.h>
#include "frame_parser.h"

int main(void)
{
    // Valid Supported Rates element: 3 rates.
    const uint8_t rates_element[] = {
        0x01, 0x03, 0x82, 0x84, 0x8B
    };

    supported_rates_t result;

    // Test valid Supported Rates parsing.
    if (parse_supported_rates(
            rates_element,
            sizeof(rates_element),
            &result) != 0)
    {
        printf("Supported Rates parsing failed.\n");
        return 1;
    }

    printf("Number of rates: %u\n", (unsigned int)result.count);

    for (uint8_t i = 0; i < result.count; i++)
    {
        printf("Rate byte %u: 0x%02X\n",
               (unsigned int)i,
               (unsigned int)result.values[i]);
    }

    // Test invalid Element ID.
    const uint8_t invalid_id[] = {
        0x00, 0x03, 0x82, 0x84, 0x8B
    };

    if (parse_supported_rates(invalid_id, sizeof(invalid_id), &result) == -1)
    {
        printf("Invalid Element ID rejected successfully.\n");
    }

    // Test truncated data.
    const uint8_t truncated[] = {
        0x01, 0x03, 0x82, 0x84
    };

    if (parse_supported_rates(truncated, sizeof(truncated), &result) == -1)
    {
        printf("Truncated data rejected successfully.\n");
    }

    // Test rate count greater than 8.
    const uint8_t oversized[] = {
        0x01, 0x09
    };

    if (parse_supported_rates(oversized, sizeof(oversized), &result) == -1)
    {
        printf("Oversized rate count rejected successfully.\n");
    }

        printf("\nDecoded rates:\n");

    for (uint8_t i = 0; i < result.count; i++)
    {
        uint8_t rate_units;

        int is_basic = decode_supported_rate(
            result.values[i],
            &rate_units
        );

        if (is_basic < 0)
        {
            printf("Rate decoding failed.\n");
            return 1;
        }

        printf("Rate: %u.%u Mbps | Basic: %s\n",
               (unsigned int)(rate_units / 2),
               (unsigned int)((rate_units % 2) * 5),
               is_basic ? "Yes" : "No");
    }

        // Test a rate without the Basic Rate flag.
    uint8_t rate_units;

    int is_basic = decode_supported_rate(0x04, &rate_units);

    if (is_basic == 0 && rate_units == 4)
    {
        printf("Non-Basic rate decoded successfully.\n");
    }
    else
    {
        printf("Non-Basic rate test failed.\n");
        return 1;
    }

    return 0;
}