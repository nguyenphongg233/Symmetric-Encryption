#include "AES/AES.h"
#include "DES/DES.h"
#include "DES/KeySchedule.h"
#include "RSA/RSA.h"
#include <bits/stdc++.h>

using namespace std;

// ======================= DES Utils =======================
static vector<uint64_t> stringToBlocks(const string &input) {
  string padded = input;
  size_t paddingSize = 8 - (padded.size() % 8);
  if (paddingSize == 0) {
    paddingSize = 8;
  }
  padded.append(paddingSize, static_cast<char>(paddingSize));

  vector<uint64_t> blocks;
  for (size_t index = 0; index < padded.size(); index += 8) {
    uint64_t block = 0;
    for (size_t offset = 0; offset < 8; ++offset) {
      block = (block << 8) | static_cast<unsigned char>(padded[index + offset]);
    }
    blocks.push_back(block);
  }
  return blocks;
}

static string blocksToString(const vector<uint64_t> &blocks) {
  string result;
  for (uint64_t block : blocks) {
    for (int shift = 56; shift >= 0; shift -= 8) {
      result.push_back(static_cast<char>((block >> shift) & 0xFF));
    }
  }

  if (!result.empty()) {
    unsigned char paddingSize = static_cast<unsigned char>(result.back());
    if (paddingSize > 0 && paddingSize <= 8 && paddingSize <= result.size()) {
      bool validPadding = true;
      for (size_t i = result.size() - paddingSize; i < result.size(); ++i) {
        if (static_cast<unsigned char>(result[i]) != paddingSize) {
          validPadding = false;
          break;
        }
      }
      if (validPadding) {
        result.resize(result.size() - paddingSize);
      }
    }
  }

  return result;
}

// ======================= AES Utils =======================
static vector<vector<uint8_t>> stringToAESBlocks(const string &input) {
  string padded = input;
  size_t paddingSize = 16 - (padded.size() % 16);
  if (paddingSize == 0) {
    paddingSize = 16;
  }
  padded.append(paddingSize, static_cast<char>(paddingSize));

  vector<vector<uint8_t>> blocks;
  for (size_t index = 0; index < padded.size(); index += 16) {
    vector<uint8_t> block(16);
    for (size_t offset = 0; offset < 16; ++offset) {
      block[offset] = static_cast<uint8_t>(padded[index + offset]);
    }
    blocks.push_back(block);
  }
  return blocks;
}

static string aesBlocksToString(const vector<vector<uint8_t>> &blocks) {
  string result;
  for (const auto &block : blocks) {
    for (uint8_t byte : block) {
      result.push_back(static_cast<char>(byte));
    }
  }

  if (!result.empty()) {
    unsigned char paddingSize = static_cast<unsigned char>(result.back());
    if (paddingSize > 0 && paddingSize <= 16 && paddingSize <= result.size()) {
      bool validPadding = true;
      for (size_t i = result.size() - paddingSize; i < result.size(); ++i) {
        if (static_cast<unsigned char>(result[i]) != paddingSize) {
          validPadding = false;
          break;
        }
      }
      if (validPadding) {
        result.resize(result.size() - paddingSize);
      }
    }
  }

  return result;
}

// ======================= Main Program =======================
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  const string plaintext = "Sinh Viết Đang is a good boy!!!";
  cout << "=== BẮT ĐẦU CHƯƠNG TRÌNH ===\n";
  cout << "Plaintext gốc: " << plaintext << "\n\n";

  // ------------------------- DES -------------------------
  cout << "============= TEST DES =============\n";
  const uint64_t des_key = 0x182BAC27CAB12341ULL;
  KeySchedule keySchedule;
  vector<uint64_t> roundKeys = keySchedule.generateRoundKeys(des_key, 16);
  DES des;

  vector<uint64_t> des_plainBlocks = stringToBlocks(plaintext);
  vector<uint64_t> des_cipherBlocks;
  vector<uint64_t> des_decryptedBlocks;

  cout << "DES Key: 0x" << hex << uppercase << setw(16) << setfill('0')
       << des_key << dec << '\n';
  cout << "--- DES Ciphertext blocks ---\n";
  for (size_t i = 0; i < des_plainBlocks.size(); ++i) {
    uint64_t encryptedBlock = des.encrypt(des_plainBlocks[i], roundKeys);
    des_cipherBlocks.push_back(encryptedBlock);
    cout << "Block " << (i + 1) << ": 0x" << hex << uppercase << setw(16)
         << setfill('0') << encryptedBlock << '\n';
  }

  cout << dec;
  for (size_t i = 0; i < des_cipherBlocks.size(); ++i) {
    uint64_t decryptedBlock = des.decrypt(des_cipherBlocks[i], roundKeys);
    des_decryptedBlocks.push_back(decryptedBlock);
  }
  string des_decryptedText = blocksToString(des_decryptedBlocks);
  cout << "DES Recovered text: " << des_decryptedText << "\n\n";

  // ------------------------- AES -------------------------
  cout << "============= TEST AES-128 =============\n";
  // Khóa 16 byte (128 bit)
  vector<uint8_t> aes_key = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                             0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  AES aes(aes_key);

  vector<vector<uint8_t>> aes_plainBlocks = stringToAESBlocks(plaintext);
  vector<vector<uint8_t>> aes_cipherBlocks;
  vector<vector<uint8_t>> aes_decryptedBlocks;

  cout << "AES Key: ";
  for (uint8_t b : aes_key) {
    cout << hex << setw(2) << setfill('0') << static_cast<int>(b) << " ";
  }
  cout << dec << "\n--- AES Ciphertext blocks ---\n";

  for (size_t i = 0; i < aes_plainBlocks.size(); ++i) {
    vector<uint8_t> encryptedBlock = aes.encryptBlock(aes_plainBlocks[i]);
    aes_cipherBlocks.push_back(encryptedBlock);
    cout << "Block " << (i + 1) << ": ";
    for (uint8_t b : encryptedBlock) {
      cout << hex << uppercase << setw(2) << setfill('0') << static_cast<int>(b)
           << " ";
    }
    cout << dec << '\n';
  }

  for (size_t i = 0; i < aes_cipherBlocks.size(); ++i) {
    vector<uint8_t> decryptedBlock = aes.decryptBlock(aes_cipherBlocks[i]);
    aes_decryptedBlocks.push_back(decryptedBlock);
  }
  string aes_decryptedText = aesBlocksToString(aes_decryptedBlocks);
  cout << "AES Recovered text: " << aes_decryptedText << "\n";

  // ------------------------- RSA -------------------------
  cout << "============= TEST RSA =============\n";
  //RSA rsa(7,17, 5);
  RSA rsa;
  uint64_t message = 19; // Ví dụ message 128-bit
  cout << "Original message: " << message << "\n";

  uint64_t encryptedMessage = rsa.encrypt(message);
  cout << "Encrypted message: " << encryptedMessage << "\n";

  uint64_t decryptedMessage = rsa.decrypt(encryptedMessage);
  cout << "Decrypted message: " << decryptedMessage << "\n";

  
  return 0;
}