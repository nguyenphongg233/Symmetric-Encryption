#include "AES.h"
#include "AES_Utils.h"
#include <stdexcept>

AES::AES(const std::vector<uint8_t>& key) {
    if (key.size() != KEY_SIZE) {
        throw std::invalid_argument("Invalid key size for AES-128. Must be 16 bytes.");
    }
    keyExpansion(key);
}

uint32_t AES::subWord(uint32_t word) const {
    uint32_t result = 0;
    result |= sbox[(word >> 24) & 0xFF] << 24;
    result |= sbox[(word >> 16) & 0xFF] << 16;
    result |= sbox[(word >> 8) & 0xFF] << 8;
    result |= sbox[word & 0xFF];
    return result;
}

uint32_t AES::rotWord(uint32_t word) const {
    return (word << 8) | (word >> 24);
}

void AES::keyExpansion(const std::vector<uint8_t>& key) {
    int numWords = (NUM_ROUNDS + 1) * 4; // 44 words cho AES-128
    roundKeys.resize(numWords * 4);

    uint32_t temp;
    int i = 0;

    // 4 words đầu tiên là key ban đầu
    while (i < 4) {
        roundKeys[i * 4] = key[i * 4];
        roundKeys[i * 4 + 1] = key[i * 4 + 1];
        roundKeys[i * 4 + 2] = key[i * 4 + 2];
        roundKeys[i * 4 + 3] = key[i * 4 + 3];
        i++;
    }

    // Các words còn lại
    while (i < numWords) {
        temp = (roundKeys[(i - 1) * 4] << 24) |
               (roundKeys[(i - 1) * 4 + 1] << 16) |
               (roundKeys[(i - 1) * 4 + 2] << 8) |
               (roundKeys[(i - 1) * 4 + 3]);

        if (i % 4 == 0) {
            temp = subWord(rotWord(temp)) ^ (rcon[i / 4] << 24);
        }

        uint32_t prev = (roundKeys[(i - 4) * 4] << 24) |
                        (roundKeys[(i - 4) * 4 + 1] << 16) |
                        (roundKeys[(i - 4) * 4 + 2] << 8) |
                        (roundKeys[(i - 4) * 4 + 3]);
        
        uint32_t word = prev ^ temp;
        roundKeys[i * 4] = (word >> 24) & 0xFF;
        roundKeys[i * 4 + 1] = (word >> 16) & 0xFF;
        roundKeys[i * 4 + 2] = (word >> 8) & 0xFF;
        roundKeys[i * 4 + 3] = word & 0xFF;
        i++;
    }
}

void AES::addRoundKey(std::array<uint8_t, 16>& state, int round) const {
    for (int i = 0; i < 16; ++i) {
        // State được lưu theo dạng ma trận 4x4 đọc theo cột.
        // i % 4 là hàng, i / 4 là cột.
        int rk_index = round * 16 + (i / 4) * 4 + (i % 4);
        state[i] ^= roundKeys[rk_index];
    }
}

void AES::subBytes(std::array<uint8_t, 16>& state) const {
    for (int i = 0; i < 16; ++i) {
        state[i] = sbox[state[i]];
    }
}

void AES::invSubBytes(std::array<uint8_t, 16>& state) const {
    for (int i = 0; i < 16; ++i) {
        state[i] = rsbox[state[i]];
    }
}

void AES::shiftRows(std::array<uint8_t, 16>& state) const {
    std::array<uint8_t, 16> temp = state;
    // Hàng 0: không dịch
    // Hàng 1: dịch trái 1 byte
    state[1] = temp[5]; state[5] = temp[9]; state[9] = temp[13]; state[13] = temp[1];
    // Hàng 2: dịch trái 2 byte
    state[2] = temp[10]; state[6] = temp[14]; state[10] = temp[2]; state[14] = temp[6];
    // Hàng 3: dịch trái 3 byte (tương đương dịch phải 1 byte)
    state[3] = temp[15]; state[7] = temp[3]; state[11] = temp[7]; state[15] = temp[11];
}

void AES::invShiftRows(std::array<uint8_t, 16>& state) const {
    std::array<uint8_t, 16> temp = state;
    // Hàng 0: không dịch
    // Hàng 1: dịch phải 1 byte (tương đương trái 3 byte)
    state[1] = temp[13]; state[5] = temp[1]; state[9] = temp[5]; state[13] = temp[9];
    // Hàng 2: dịch phải 2 byte
    state[2] = temp[10]; state[6] = temp[14]; state[10] = temp[2]; state[14] = temp[6];
    // Hàng 3: dịch phải 3 byte (tương đương trái 1 byte)
    state[3] = temp[7]; state[7] = temp[11]; state[11] = temp[15]; state[15] = temp[3];
}

void AES::mixColumns(std::array<uint8_t, 16>& state) const {
    std::array<uint8_t, 16> temp = state;
    for (int i = 0; i < 4; ++i) {
        int c = i * 4;
        state[c]     = gf_mult(0x02, temp[c]) ^ gf_mult(0x03, temp[c+1]) ^ temp[c+2] ^ temp[c+3];
        state[c + 1] = temp[c] ^ gf_mult(0x02, temp[c+1]) ^ gf_mult(0x03, temp[c+2]) ^ temp[c+3];
        state[c + 2] = temp[c] ^ temp[c+1] ^ gf_mult(0x02, temp[c+2]) ^ gf_mult(0x03, temp[c+3]);
        state[c + 3] = gf_mult(0x03, temp[c]) ^ temp[c+1] ^ temp[c+2] ^ gf_mult(0x02, temp[c+3]);
    }
}

void AES::invMixColumns(std::array<uint8_t, 16>& state) const {
    std::array<uint8_t, 16> temp = state;
    for (int i = 0; i < 4; ++i) {
        int c = i * 4;
        state[c]     = gf_mult(0x0e, temp[c]) ^ gf_mult(0x0b, temp[c+1]) ^ gf_mult(0x0d, temp[c+2]) ^ gf_mult(0x09, temp[c+3]);
        state[c + 1] = gf_mult(0x09, temp[c]) ^ gf_mult(0x0e, temp[c+1]) ^ gf_mult(0x0b, temp[c+2]) ^ gf_mult(0x0d, temp[c+3]);
        state[c + 2] = gf_mult(0x0d, temp[c]) ^ gf_mult(0x09, temp[c+1]) ^ gf_mult(0x0e, temp[c+2]) ^ gf_mult(0x0b, temp[c+3]);
        state[c + 3] = gf_mult(0x0b, temp[c]) ^ gf_mult(0x0d, temp[c+1]) ^ gf_mult(0x09, temp[c+2]) ^ gf_mult(0x0e, temp[c+3]);
    }
}

std::vector<uint8_t> AES::encryptBlock(const std::vector<uint8_t>& plaintext) const {
    if (plaintext.size() != BLOCK_SIZE) {
        throw std::invalid_argument("Plaintext block must be 16 bytes.");
    }

    std::array<uint8_t, 16> state;
    for (int i = 0; i < 16; ++i) {
        state[i] = plaintext[i];
    }

    addRoundKey(state, 0);

    for (int round = 1; round < NUM_ROUNDS; ++round) {
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, round);
    }

    // Vòng cuối không có mixColumns
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, NUM_ROUNDS);

    std::vector<uint8_t> ciphertext(16);
    for (int i = 0; i < 16; ++i) {
        ciphertext[i] = state[i];
    }
    return ciphertext;
}

std::vector<uint8_t> AES::decryptBlock(const std::vector<uint8_t>& ciphertext) const {
    if (ciphertext.size() != BLOCK_SIZE) {
        throw std::invalid_argument("Ciphertext block must be 16 bytes.");
    }

    std::array<uint8_t, 16> state;
    for (int i = 0; i < 16; ++i) {
        state[i] = ciphertext[i];
    }

    addRoundKey(state, NUM_ROUNDS);

    for (int round = NUM_ROUNDS - 1; round > 0; --round) {
        invShiftRows(state);
        invSubBytes(state);
        addRoundKey(state, round);
        invMixColumns(state);
    }

    invShiftRows(state);
    invSubBytes(state);
    addRoundKey(state, 0);

    std::vector<uint8_t> decrypted(16);
    for (int i = 0; i < 16; ++i) {
        decrypted[i] = state[i];
    }
    return decrypted;
}
