
#include <stdio.h>
#include "frame_parser.h"

int main(void)
{
    // SSID Information Element for the network name "TEST"
    const uint8_t ssid_element[] = {
        0x00,                   // Element ID: SSID
        0x04,                   // Length: 4 bytes
        0x54, 0x45, 0x53, 0x54  // ASCII: TEST
    };

    ssid_info_t ssid;

    // Parse the SSID Information Element
    int status = parse_ssid_element(
        ssid_element,
        sizeof(ssid_element),
        &ssid
    );

    if (status != 0)
    {
        printf("SSID parsing failed.\n");
        return 1;
    }

    // Print the extracted network name
    printf("SSID Length: %u\n", (unsigned int)ssid.length);

    printf("SSID: ");

    for (uint8_t i = 0; i < ssid.length; i++)
    {
        printf("%c", ssid.value[i]);
    }

    printf("\nSSID parsing successful.\n");

        // Test 1: Incorrect Element ID
    const uint8_t invalid_id[] = {0x01, 0x04, 0x54, 0x45, 0x53, 0x54};

    status = parse_ssid_element(invalid_id, sizeof(invalid_id), &ssid);

    if (status == -1)
    {
        printf("Invalid Element ID rejected successfully.\n");
    }
    else
    {
        printf("ERROR: Invalid Element ID accepted!\n");
        return 1;
    }

    // Test 2: Declared length exceeds available data
    const uint8_t truncated_ssid[] = {0x00, 0x06, 0x54, 0x45};

    status = parse_ssid_element(
        truncated_ssid, sizeof(truncated_ssid), &ssid
    );

    if (status == -1)
    {
        printf("Truncated SSID rejected successfully.\n");
    }
    else
    {
        printf("ERROR: Truncated SSID accepted!\n");
        return 1;
    }

    // Test 3: SSID length exceeds the 32-byte limit
    const uint8_t oversized_ssid[] = {0x00, 0x21};

    status = parse_ssid_element(
        oversized_ssid, sizeof(oversized_ssid), &ssid
    );

    if (status == -1)
    {
        printf("Oversized SSID rejected successfully.\n");
    }
    else
    {
        printf("ERROR: Oversized SSID accepted!\n");
        return 1;
    }

    return 0;
}
