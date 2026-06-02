#include "KeySchedule.h"
#include <bits/stdc++.h>

using namespace std;

uint64_t KeySchedule::rotateLeft(uint64_t value, int shift){
    return ((value << shift) | (value >> (28 - shift))) & 0x0FFFFFFF;
}
uint64_t KeySchedule::PC1Permutation(uint64_t key){
    // input = 64-bit key, output = 56-bit key
    uint64_t permutedKey = 0;
    for (int i = 0;i < 56;i++){
        // PC_1 entries use 1..64 with bit 1 = MSB, so extract using (64 - index)
        permutedKey = (permutedKey << 1) | ((key >> (64 - PC_1[i])) & 1ULL);
    }
    return permutedKey;
}
uint64_t KeySchedule::PC2Permutation(uint64_t key){
    // input = 56-bit key, output = 48-bit round key
    uint64_t roundKey = 0;
    for (int i = 0;i < 48;i++){
        // PC_2 entries index into 56-bit combined key; bit 1 is MSB of the 56-bit value
        roundKey = (roundKey << 1) | ((key >> (56 - PC_2[i])) & 1ULL);
    }
    return roundKey;
}
vector<uint64_t> KeySchedule::generateRoundKeys(uint64_t key, int rounds){
    vector<uint64_t> roundKeys;
    // Implement the key schedule algorithm to generate round keys

    // Step 1: Apply PC-1 permutation to the original key
    uint64_t permutedKey = PC1Permutation(key);

    uint64_t leftHalf = (permutedKey >> 28) & 0xFFFFFFF; // Left 28 bits
    uint64_t rightHalf = permutedKey & 0xFFFFFFF; // Right 28 bits

    for (int i = 0;i < rounds;i++){
        // step 2: Rotate the halves according to the shifts
        leftHalf = rotateLeft(leftHalf, shifts[i]);
        rightHalf = rotateLeft(rightHalf, shifts[i]);
        // step 3: Combine the halves and apply PC-2 permutation to get the round key
        uint64_t combinedKey = (leftHalf << 28) | rightHalf;
        uint64_t roundKey = PC2Permutation(combinedKey);
        roundKeys.push_back(roundKey);
    }

    return roundKeys;
}