
#include <stdio.h>
#include "frame_parser.h"

// Print a MAC address in standard hexadecimal format
void print_mac_address(const char *label, const uint8_t mac[6])
{
    printf("%s: ", label);

    for (int i = 0; i < 6; i++)
    {
        printf("%02X", mac[i]);

        if (i < 5)
        {
            printf(":");
        }
    }

    printf("\n");
}

int main(void)
{
    // Sample 24-byte Beacon management header
    const uint8_t beacon_frame[24] = {
        0x80, 0x00,             // Frame Control
        0x00, 0x00,             // Duration
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Receiver
        0xAA, 0xBB, 0xCC, 0x11, 0x22, 0x33, // Transmitter
        0xAA, 0xBB, 0xCC, 0x11, 0x22, 0x33, // BSSID
        0x10, 0x00              // Sequence Control
    };

    management_header_t header;

    // Parse the complete management header
    int status = parse_management_header(
        beacon_frame,
        sizeof(beacon_frame),
        &header
    );

    if (status != 0)
    {
        printf("Management header parsing failed.\n");
        return 1;
    }

    printf("Frame Type: %u\n", header.control.type);
    printf("Frame Subtype: %u\n", header.control.subtype);

    print_mac_address("Receiver", header.receiver);
    print_mac_address("Transmitter", header.transmitter);
    print_mac_address("BSSID", header.bssid);

    printf("Management header parsing successful.\n");

    // Test rejection of a Control frame (Type = 1)
const uint8_t control_frame[24] = {
    0x04, 0x00
};

status = parse_management_header(
    control_frame,
    sizeof(control_frame),
    &header
);

if (status == -1)
{
    printf("Control frame rejected successfully.\n");
}
else
{
    printf("ERROR: Control frame was accepted!\n");
    return 1;
}

// Test rejection of a Data frame (Type = 2)
const uint8_t data_frame[24] = {
    0x08, 0x00
};

status = parse_management_header(
    data_frame,
    sizeof(data_frame),
    &header
);

if (status == -1)
{
    printf("Data frame rejected successfully.\n");
}
else
{
    printf("ERROR: Data frame was accepted!\n");
    return 1;
}


    // Test a management header that is only 23 bytes long
const uint8_t short_frame[23] = {0};

status = parse_management_header(
    short_frame,
    sizeof(short_frame),
    &header
);

if (status == -1)
{
    printf("Incomplete management header rejected successfully.\n");
}
else
{
    printf("ERROR: Incomplete header was accepted!\n");
    return 1;
}
    return 0;
}
