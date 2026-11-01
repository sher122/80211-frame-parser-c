# 802.11 Wi-Fi Beacon Frame Parser in C

A lightweight C-based parser that reads raw IEEE 802.11 Beacon frames and extracts useful Wi-Fi network information.

## Project Overview

Wi-Fi devices use management frames to exchange information about wireless networks. One important management frame is the **Beacon frame**, which is regularly sent by an Access Point to announce its presence.

In this project, I developed a C program that parses a raw 802.11 Beacon frame, reads its fields, and displays important network information such as:

- SSID (Wi-Fi network name)
- MAC addresses
- Beacon interval
- Timestamp
- Supported data rates
- Wi-Fi channel

The main focus of this project is to understand 802.11 frame structure, byte-level data parsing, and safe handling of input buffers in C.

## Features

- Parses the 802.11 Frame Control field.
- Extracts Management Frame header information.
- Reads Receiver, Transmitter, and BSSID MAC addresses.
- Parses SSID Information Elements.
- Decodes Supported Rates, including the Basic Rate flag.
- Extracts the Wi-Fi channel from the DS Parameter Set element.
- Reads Beacon fixed parameters.
- Parses a complete Beacon frame.
- Skips unsupported Information Elements safely.
- Checks for invalid pointers, incomplete data, and malformed Information Elements.
- Clears partially parsed results when parsing fails.

## Technologies Used

- **Language:** C
- **Compiler:** GCC
- **Platform:** Windows 10
- **Development Environment:** Visual Studio Code
- **Version Control:** Git and GitHub
- **Build and Test Scripts:** PowerShell

## Project Structure

```text
80211-frame-parser-c/
│
├── app/
│   └── main.c
│
├── include/
│   └── frame_parser.h
│
├── src/
│   └── frame_parser.c
│
├── tests/
│   ├── test_frame_control.c
│   ├── test_ssid.c
│   ├── test_supported_rates.c
│   ├── test_ds_parameter_set.c
│   ├── test_beacon_ies.c
│   ├── test_beacon_fixed.c
│   └── test_beacon_frame.c
│
├── samples/
│   ├── beacon_sample.bin
│   └── create_sample.py
│
├── build.ps1
├── test.ps1
├── .gitignore
└── README.md
```

## How to Build

### Requirements

- Windows with PowerShell
- GCC compiler

The build script currently uses the GCC path configured in `build.ps1`. If GCC is installed in a different location, update the `$Compiler` path in that script.

Open PowerShell in the project directory and run:

```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1
```

This compiles the source files and generates:

```text
frame_parser.exe
```

## How to Run

Use the included sample Beacon frame:

```powershell
.\frame_parser.exe samples\beacon_sample.bin
```

The program reads the raw frame, parses its contents, and displays the extracted information.

## Sample Output

```text
Beacon Frame Analysis

Receiver: FF:FF:FF:FF:FF:FF
Transmitter: AA:BB:CC:11:22:33
BSSID: AA:BB:CC:11:22:33

Timestamp: 1000 microseconds
Beacon Interval: 100 TU
Capability: 0x0431

SSID: TEST
Supported Rates Count: 3
Wi-Fi Channel: 6
```

*Output is based on the included synthetic Beacon frame.*

## Testing

The project includes seven standalone C test programs covering the parser's main functions.

To run the complete test suite:

```powershell
powershell -ExecutionPolicy Bypass -File .\test.ps1
```

The tests cover:

- Frame Control and Management Header parsing
- SSID validation
- Supported Rates parsing and decoding
- DS Parameter Set parsing
- Beacon Information Elements
- Beacon fixed parameters
- Complete Beacon frame parsing
- Truncated and malformed input
- NULL pointer handling
- Partial-result cleanup after parsing failure

All seven test programs pass with the included test suite.

## What I Learned

While developing this project, I gained practical experience with:

- C structures and functions
- Pointers and arrays
- Bitwise operations and bit masking
- Binary and hexadecimal data
- Little-endian byte interpretation
- Parsing structured network data
- Input validation and buffer-length checks
- Writing unit tests for valid and invalid inputs
- Using Git for version control

## Current Limitations

- The program currently accepts raw 802.11 Beacon frame data.
- It does not currently read PCAP files.
- It does not capture live Wi-Fi traffic.
- The included sample frame is synthetically generated for testing.

## Future Improvements

- Add PCAP file support.
- Parse additional 802.11 Information Elements.
- Support Probe Response frames.
- Add more malformed-input and boundary-condition tests.
- Improve diagnostic messages and error reporting.

## Author

Developed as a personal learning project to strengthen my understanding of C programming, WLAN fundamentals, and low-level packet parsing, with a focus on skills relevant to embedded and wireless firmware development.