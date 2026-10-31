# 802.11 Frame Parser in C

## Project Overview

A lightweight C program that parses raw IEEE 802.11 wireless frame data and extracts important fields from the frame header.

This project demonstrates foundational skills used in WLAN and embedded firmware development, including byte-level data processing, bitwise operations, structures, pointers, and defensive memory handling.

## Planned Features

- Decode the IEEE 802.11 Frame Control field.
- Identify frame types and subtypes.
- Extract MAC addresses from supported frame headers.
- Validate frame lengths before accessing data.
- Detect and safely reject malformed or incomplete frames.
- Include test cases for valid and invalid frame data.

## Technology

- Language: C (C11)
- Compiler: GCC
- Development environment: Visual Studio Code
- Version control: Git and GitHub

## Project Structure

```text
include/   Header files
src/       Parser implementation
tests/     Test programs
samples/   Sample frame data
docs/      Technical documentation
```

## Project Status

<badge>In development</badge>

The project is being built incrementally, beginning with the IEEE 802.11 frame structure and basic frame-header parsing.

## Learning Goals

- Understand the structure of IEEE 802.11 frames.
- Practice C programming for low-level data processing.
- Apply bitwise operations to decode protocol fields.
- Build robust parsers with input validation and memory safety.
- Develop and test firmware-style C code.