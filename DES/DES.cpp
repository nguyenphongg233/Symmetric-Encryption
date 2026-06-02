#include "DES.h"
#include "DES_table.h"
using namespace std;

#define DES_DEBUG 1

uint64_t DES::initialPermutation(uint64_t plaintext){
    // Implement the initial permutation of the DES algorithm
    uint64_t permutedText = 0;
    for (int i = 0;i < 64;i++){
        permutedText = (permutedText << 1) | ((plaintext >> (64 - IP[i])) & 1ULL);
    }
    return permutedText;
}

uint64_t DES::finalPermutation(uint64_t ciphertext){
    // Implement the final permutation of the DES algorithm
    uint64_t permutedText = 0;
    for (int i = 0;i < 64;i++){
        permutedText = (permutedText << 1) | ((ciphertext >> (64 - FP[i])) & 1ULL);
    }
    return permutedText;
}

uint64_t DES::ExpansionPermutation(uint32_t halfBlock){
    // Implement the expansion permutation of the DES algorithm (returns 48-bit value in uint64_t)
    uint64_t expandedBlock = 0;
    for (int i = 0;i < 48;i++){
        expandedBlock = (expandedBlock << 1) | ((halfBlock >> (32 - E[i])) & 1ULL);
    }
    return expandedBlock;
}

uint32_t DES::Permutation(uint32_t sBoxOutput){
    // Implement the permutation step of the DES algorithm
    uint32_t permutedBlock = 0;
    for (int i = 0;i < 32;i++){
        permutedBlock = (permutedBlock << 1) | ((sBoxOutput >> (32 - P[i])) & 1U);
    }
    return permutedBlock;
}
uint32_t DES::feistelFunction(uint32_t halfBlock, uint64_t roundKey){
    // Implement the Feistel function used in the DES algorithm
    // This function should take a 32-bit half-block and a 48-bit round key,
    // and return a 32-bit output after applying the expansion, substitution, and permutation steps.
    // For simplicity, this is a placeholder implementation. You will need to implement the actual logic.
    
    // cout << "Feistel function input half-block: 0x" << hex << setw(8) << setfill('0') << uppercase << halfBlock 
    //      << " | Round Key: 0x" << hex << setw(12) << setfill('0') << uppercase << roundKey << endl;
    // printBinary(halfBlock, 32);
    // printBinary(roundKey, 48);
    
    uint64_t expandedHalfBlock = ExpansionPermutation(halfBlock);
    uint64_t xored = expandedHalfBlock ^ roundKey; // 48-bit value
    uint32_t sBoxOutput = 0;

    static int feistel_calls = 0;
    bool doDebug = (feistel_calls == 0);
    feistel_calls++;
    for (int block = 0; block < 8; block++){
        uint8_t sixBits = (xored >> (42 - block * 6)) & 0x3F; // Extract 6 bits for the current block (MSB-first)
        uint8_t row = ((sixBits & 0x20) >> 4) | (sixBits & 0x01); // bits 6 and 1
        uint8_t col = (sixBits >> 1) & 0x0F;
        uint8_t sBoxValue = S_BOX[block][row][col];
        sBoxOutput = (sBoxOutput << 4) | (sBoxValue & 0x0F);
    }

    sBoxOutput = Permutation(sBoxOutput); // Apply the permutation to the S-box output

    // cout << "Feistel function output: 0x" << hex << setw(8) << setfill('0') << uppercase << sBoxOutput << endl;
    // printBinary(sBoxOutput, 32);
    return sBoxOutput;
}

uint64_t DES::encrypt(uint64_t plaintext, vector<uint64_t> roundKeys){
    // Implement the encryption process of the DES algorithm
    // This function should take a 64-bit plaintext and a vector of round keys,
    // and return the 64-bit ciphertext after applying the initial permutation, 16 rounds of processing, and final permutation.
    // For simplicity, this is a placeholder implementation. You will need to implement the actual logic.
    // cout << "Initial plaintext: 0x" << hex << setw(16) << setfill('0') << uppercase << plaintext << endl;
    // printBinary(plaintext, 64);

    uint64_t permutedText = initialPermutation(plaintext);

    // cout << "After initial permutation: 0x" << hex << setw(16) << setfill('0') << uppercase << permutedText << endl;
    // printBinary(permutedText, 64);

    uint32_t leftHalf = (permutedText >> 32) & 0xFFFFFFFF; // Left 32 bits
    uint32_t rightHalf = permutedText & 0xFFFFFFFF; // Right 32 bits

    // cout << "Initial Left Half: 0x" << hex << setw(8) << setfill('0') << uppercase << leftHalf 
    //      << " | Initial Right Half: 0x" << hex << setw(8) << setfill('0') << uppercase << rightHalf << endl;
    //     printBinary(leftHalf, 32);
    //     printBinary(rightHalf, 32);
    
    for (size_t i = 0;i < roundKeys.size();i++){
        uint32_t temp = rightHalf;
        rightHalf = leftHalf ^ feistelFunction(rightHalf, roundKeys[i]);
        leftHalf = temp;

        // cout << "After Round " << (i + 1) << ": Left Half: 0x" << hex << setw(8) << setfill('0') << uppercase << leftHalf 
        //      << " | Right Half: 0x" << hex << setw(8) << setfill('0') << uppercase << rightHalf << endl;
        // printBinary(leftHalf, 32);
        // printBinary(rightHalf, 32); 
    }
    swap(leftHalf, rightHalf); // Swap halves after the final round
    uint64_t result = ((uint64_t)leftHalf << 32) | rightHalf;
    return finalPermutation(result);
}

uint64_t DES::decrypt(uint64_t ciphertext, vector<uint64_t> roundKeys){
    // Implement the decryption process of the DES algorithm
    // This function should take a 64-bit ciphertext and a vector of round keys,
    // and return the 64-bit plaintext after applying the initial permutation, 16 rounds of processing in reverse order, and final permutation.
    // For simplicity, this is a placeholder implementation. You will need to implement the actual logic.
    
    uint64_t permutedText = initialPermutation(ciphertext);
    uint32_t leftHalf = (permutedText >> 32) & 0xFFFFFFFF; // Left 32 bits
    uint32_t rightHalf = permutedText & 0xFFFFFFFF; // Right 32 bits
    for (int i = roundKeys.size() - 1;i >= 0;i--){
        uint32_t temp = rightHalf;
        rightHalf = leftHalf ^ feistelFunction(rightHalf, roundKeys[i]);
        leftHalf = temp;
    }
    swap(leftHalf, rightHalf); // Swap halves after the final round
    uint64_t result = ((uint64_t)leftHalf << 32) | rightHalf;
    return finalPermutation(result);    
    
    return 0; // Placeholder return value
}