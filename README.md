# Symmetric Encryption

A small C++ project that reimplements symmetric encryption algorithms for learning and experimentation.

## Overview

This workspace currently focuses on **DES (Data Encryption Standard)** and includes:

- key schedule generation
- 16-round encryption and decryption
- DES tables and bitwise helpers
- a demo program that accepts a long string, splits it into 64-bit blocks, encrypts each block, then decrypts back to the original string

The goal of the project is to study how symmetric ciphers work internally and to build the algorithms from scratch in C++.

## Project Structure

```text
Symmetric Encryption/
├── Main.cpp
├── README.md
└── DES/
    ├── DES.cpp
    ├── DES.h
    ├── DES_table.cpp
    ├── DES_table.h
    ├── KeySchedule.cpp
    ├── KeySchedule.h
    ├── main.cpp
    └── readme.md
```

## Build and Run

Build the string-based demo from the repository root:

```bash
g++ Main.cpp DES/DES.cpp DES/KeySchedule.cpp DES/DES_table.cpp -o test_main
./test_main
```

## Example

The demo asks for:

- a 64-bit DES key in hexadecimal
- a plaintext string of any length

It then:

- pads the string to 8-byte blocks
- encrypts each 64-bit block with DES
- prints the ciphertext blocks
- decrypts the blocks back
- removes padding and prints the recovered string

## Notes

- File names are case-sensitive on macOS/Linux, so keep the build command exactly as shown.
- The `DES/` folder still contains a smaller standalone demo and the DES implementation files.
- More symmetric encryption algorithms can be added later using the same project layout.
