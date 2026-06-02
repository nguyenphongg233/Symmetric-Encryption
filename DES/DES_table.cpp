#include "DES_table.h"

string toBinaryString(uint64_t value, int bits) {
    string binaryStr;
    for (int i = bits - 1; i >= 0; i--) {
        binaryStr += ((value >> i) & 1) ? '1' : '0';
    }
    return binaryStr;
}
void printBinary(uint64_t value, int bits) {
    string binaryStr = toBinaryString(value, bits);
    for(size_t i = 0; i < binaryStr.size(); i++) {
        cout << binaryStr[i];
        if ((i + 1) % 8 == 0) cout << " "; // Add space every 8 bits
    }
    cout << endl;
}