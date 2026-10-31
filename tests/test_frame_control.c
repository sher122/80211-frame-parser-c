
#include <stdio.h>
#include "frame_parser.h"

// Convert numeric frame type into a readable name
const char *get_frame_type_name(uint8_t type)
{
    switch (type)
    {
        case 0:
            return "Management";
        case 1:
            return "Control";
        case 2:
            return "Data";
        default:
            return "Reserved";
    }
}

// Convert Management subtype into a readable name
const char *get_management_subtype_name(uint8_t subtype)
{
    switch (subtype)
    {
        case 0:
            return "Association Request";
        case 1:
            return "Association Response";
        case 4:
            return "Probe Request";
        case 5:
            return "Probe Response";
        case 8:
            return "Beacon";
        case 11:
            return "Authentication";
        default:
            return "Other Management Frame";
    }
}

int main(void)
{
    const uint8_t beacon_frame[] = {0x80, 0x00};
    frame_control_t result;

    int status = parse_frame_control(
        beacon_frame,
        sizeof(beacon_frame),
        &result
    );

    if (status != 0)
    {
        printf("Frame parsing failed.\n");
        return 1;
    }

    printf("Protocol Version: %u\n", result.protocol_version);
    printf("Frame Type: %s\n", get_frame_type_name(result.type));

    if (result.type == 0)
    {
        printf("Frame Subtype: %s\n",
               get_management_subtype_name(result.subtype));
    }
        // Test an incomplete frame containing only one byte
    const uint8_t truncated_frame[] = {0x80};

    status = parse_frame_control(
        truncated_frame,
        sizeof(truncated_frame),
        &result
    );

    if (status == -1)
    {
        printf("Truncated frame rejected successfully.\n");
    }
    else
    {
        printf("ERROR: Truncated frame was accepted!\n");
        return 1;
    }

        // Test a NULL frame pointer
    status = parse_frame_control(NULL, 2, &result);

    if (status == -1)
    {
        printf("NULL frame pointer rejected successfully.\n");
    }
    else
    {
        printf("ERROR: NULL frame pointer accepted!\n");
        return 1;
    }

    // Test a NULL result pointer
    status = parse_frame_control(beacon_frame, sizeof(beacon_frame), NULL);

    if (status == -1)
    {
        printf("NULL result pointer rejected successfully.\n");
    }
    else
    {
        printf("ERROR: NULL result pointer accepted!\n");
        return 1;
    }
    return 0;
    
}
