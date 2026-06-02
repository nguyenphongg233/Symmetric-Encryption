#include <bits/stdc++.h>
#include "KeySchedule.h"
#include "DES.h"
#include "DES_table.h"
using namespace std;

signed main(){
    
    // uint64_t key = 0x133457799BBCDFF1ULL; // Standard test key
    // KeySchedule keySchedule;
    // vector<uint64_t> roundKeys = keySchedule.generateRoundKeys(key, 16);    
    // // Print the generated round keys (48-bit) padded to 12 hex digits
    // for (size_t i = 0; i < roundKeys.size(); i++){
    //     // cout << "Round " << i << " : ";
    //     // printBinary(roundKeys[i], 48);
    //     cout << "Round " << (i + 1) << ": 0x" << setw(12) << setfill('0') << hex << uppercase << roundKeys[i]
    //             << " (" << toBinaryString(roundKeys[i], 48) << ")" << endl;
    // }

    // DES des;
    // uint64_t plaintext = 0x0123456789ABCDEF; // Example 64-bit plaintext
    // // Debug: print intermediate values for round 1

    // printBinary(plaintext, 64);

    // cout << "Plaintext: 0x" << hex << setw(16) << setfill('0') << uppercase << plaintext << endl;

    // uint64_t ciphertext = des.encrypt(plaintext, roundKeys);
    // cout << "Ciphertext: 0x" << hex << setw(16) << setfill('0') << uppercase << ciphertext << endl;

    // uint64_t decryptedText = des.decrypt(ciphertext, roundKeys);
    // cout << "Decrypted Text: 0x" << hex << setw(16) << setfill('0') << uppercase << decryptedText << endl;   

    uint64_t key;
    string plaintextStr;

    cin >> hex >> key; // Read the key in hexadecimal format
    cin >> plaintextStr; // Read the plaintext as a string

    assert(plaintextStr.size() <= 8); // Ensure the plaintext string is at most 8 characters (64 bits)
    while(plaintextStr.size() < 8){
        plaintextStr = "0" + plaintextStr; // Pad with leading zeros if necessary
    }
    // Convert the plaintext string to a block of 64-bit integer
    uint64_t plaintext = 0;
    for (size_t i = 0; i < plaintextStr.size() && i < 8; i++){
        plaintext = (plaintext << 8) | static_cast<uint8_t>(plaintextStr[i]);
    }

    KeySchedule keySchedule;
    vector<uint64_t> roundKeys = keySchedule.generateRoundKeys(key, 16);
    DES des;
    uint64_t ciphertext = des.encrypt(plaintext, roundKeys);
    cout << hex << uppercase << setw(16) << setfill('0') << ciphertext << endl;
    
    return 0;
}