# DES Encryption Project

This project is a C++ implementation of the **Data Encryption Standard (DES)**, including:

- key schedule generation
- 16-round DES encryption
- DES decryption
- helper tables and bitwise utilities

It also includes a small `main.cpp` demo that uses the standard DES test vector.

## Project Structure

```text
DES/
├── DES.cpp
├── DES.h
├── DES_table.cpp
├── DES_table.h
├── KeySchedule.cpp
├── KeySchedule.h
├── main.cpp
└── readme.md
```

## Build

Use `g++` to compile the demo program:

```bash
g++ main.cpp DES.cpp KeySchedule.cpp DES_table.cpp -o test
```

## Run

```bash
./test
```

## Example

The demo uses the classic DES example:

- Key: `0x133457799BBCDFF1`
- Plaintext: `0x0123456789ABCDEF`

It prints:

- generated round keys
- plaintext in binary and hexadecimal
- ciphertext after encryption
- decrypted text after decryption

## Notes

- The code is written for educational purposes and to help understand how DES works internally.
- File names are case-sensitive on macOS/Linux, so keep the build command exactly as shown.