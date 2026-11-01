#include <stdio.h>
#include <stdint.h>
#include "frame_parser.h"

#define MAX_FRAME_SIZE 4096

// Prints a MAC address in standard hexadecimal format.
static void print_mac(const uint8_t address[6])
{
    for (int i = 0; i < 6; i++)
    {
        printf("%02X", (unsigned int)address[i]);

        if (i < 5)
            printf(":");
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    // Require the user to provide a frame file.
    if (argc != 2)
    {
        printf("Usage: frame_parser <frame_file>\n");
        return 1;
    }

    // Open the input file in binary read mode.
    FILE *file = fopen(argv[1], "rb");

    if (file == NULL)
    {
        printf("Error: Could not open file.\n");
        return 1;
    }

    // Read the file size.
    if (fseek(file, 0, SEEK_END) != 0)
    {
        printf("Error: Could not seek file.\n");
        fclose(file);
        return 1;
    }

    long file_size = ftell(file);

    if (file_size < 36 || file_size > MAX_FRAME_SIZE)
    {
        printf("Error: Invalid frame file size.\n");
        fclose(file);
        return 1;
    }

    rewind(file);

    // Read the frame bytes into memory.
    uint8_t frame[MAX_FRAME_SIZE];

    size_t bytes_read = fread(frame, 1, (size_t)file_size, file);

    fclose(file);

    if (bytes_read != (size_t)file_size)
    {
        printf("Error: Could not read complete frame.\n");
        return 1;
    }

    // Parse the complete Beacon frame.
    beacon_frame_t result;

    if (parse_beacon_frame(frame, (uint32_t)bytes_read, &result) != 0)
    {
        printf("Error: Invalid or unsupported Beacon frame.\n");
        return 1;
    }

    // Display the extracted information.
    printf("\n===== 802.11 Beacon Analysis =====\n");

    printf("Receiver: ");
    print_mac(result.header.receiver);

    printf("Transmitter: ");
    print_mac(result.header.transmitter);

    printf("BSSID: ");
    print_mac(result.header.bssid);

    printf("Timestamp: %llu microseconds\n",
           (unsigned long long)result.fixed_parameters.timestamp);

    printf("Beacon Interval: %u TU\n",
           (unsigned int)result.fixed_parameters.beacon_interval);

    printf("Capability: 0x%04X\n",
           (unsigned int)result.fixed_parameters.capability_information);

    if (result.information_elements.has_ssid)
    {
        printf("SSID: ");

        for (uint8_t i = 0; i < result.information_elements.ssid.length; i++)
        {
            uint8_t c = result.information_elements.ssid.value[i];
            putchar((c >= 32 && c <= 126) ? c : '.');
        }

        printf("\n");
    }

    if (result.information_elements.has_supported_rates)
    {
        printf("Supported Rates:\n");

        for (uint8_t i = 0;
             i < result.information_elements.supported_rates.count; i++)
        {
            uint8_t units;

            int basic = decode_supported_rate(
                result.information_elements.supported_rates.values[i],
                &units
            );

            if (basic >= 0)
            {
                printf("  %u.%u Mbps (%s)\n",
                       (unsigned int)(units / 2),
                       (unsigned int)((units % 2) * 5),
                       basic ? "Basic" : "Non-Basic");
            }
        }
    }

    if (result.information_elements.has_ds_parameter_set)
    {
        printf("Wi-Fi Channel: %u\n",
               (unsigned int)result.information_elements.ds_parameter_set.channel);
    }

    printf("==================================\n");

    return 0;
}