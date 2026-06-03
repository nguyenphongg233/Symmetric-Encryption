#ifndef AES_H
#define AES_H

#include <vector>
#include <cstdint>
#include <array>

// Lớp AES mã hóa/giải mã cơ bản.
// Ở đây chúng ta implement phiên bản AES-128
class AES {
public:
    // Kích thước của khóa cho AES-128 là 16 byte (128 bit).
    static constexpr int KEY_SIZE = 16;
    // Số round cho AES-128 là 10.
    static constexpr int NUM_ROUNDS = 10;
    // Kích thước của 1 block là 16 byte (128 bit).
    static constexpr int BLOCK_SIZE = 16;

    // Constructor nhận vào khóa 128-bit
    AES(const std::vector<uint8_t>& key);

    // Mã hóa 1 block 16-byte
    std::vector<uint8_t> encryptBlock(const std::vector<uint8_t>& plaintext) const;

    // Giải mã 1 block 16-byte
    std::vector<uint8_t> decryptBlock(const std::vector<uint8_t>& ciphertext) const;

private:
    // Chứa round keys, tổng cộng có (NUM_ROUNDS + 1) * BLOCK_SIZE byte
    std::vector<uint8_t> roundKeys;

    // --- Key Expansion ---
    void keyExpansion(const std::vector<uint8_t>& key);
    
    // Hàm phụ trợ cho Key Expansion
    uint32_t rotWord(uint32_t word) const;
    uint32_t subWord(uint32_t word) const;

    // --- Biến đổi trạng thái (State Transformation) ---
    // State được biểu diễn dưới dạng mảng 16 byte.
    // Thực tế nó là một ma trận 4x4, lưu theo cột.

    // Hàm chung
    void addRoundKey(std::array<uint8_t, 16>& state, int round) const;

    // Mã hóa
    void subBytes(std::array<uint8_t, 16>& state) const;
    void shiftRows(std::array<uint8_t, 16>& state) const;
    void mixColumns(std::array<uint8_t, 16>& state) const;

    // Giải mã
    void invSubBytes(std::array<uint8_t, 16>& state) const;
    void invShiftRows(std::array<uint8_t, 16>& state) const;
    void invMixColumns(std::array<uint8_t, 16>& state) const;
};

#endif // AES_H
