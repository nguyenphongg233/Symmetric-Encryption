#include <bits/stdc++.h>
#include "DES/DES.h"
#include "DES/KeySchedule.h"

using namespace std;

static vector<uint64_t> stringToBlocks(const string& input){
	string padded = input;
	size_t paddingSize = 8 - (padded.size() % 8);
	if (paddingSize == 0){
		paddingSize = 8;
	}
	padded.append(paddingSize, static_cast<char>(paddingSize));

	vector<uint64_t> blocks;
	for (size_t index = 0; index < padded.size(); index += 8){
		uint64_t block = 0;
		for (size_t offset = 0; offset < 8; ++offset){
			block = (block << 8) | static_cast<unsigned char>(padded[index + offset]);
		}
		blocks.push_back(block);
	}
	return blocks;
}

static string blocksToString(const vector<uint64_t>& blocks){
	string result;
	for (uint64_t block : blocks){
		for (int shift = 56; shift >= 0; shift -= 8){
			result.push_back(static_cast<char>((block >> shift) & 0xFF));
		}
	}

	if (!result.empty()){
		unsigned char paddingSize = static_cast<unsigned char>(result.back());
		if (paddingSize > 0 && paddingSize <= 8 && paddingSize <= result.size()){
			bool validPadding = true;
			for (size_t i = result.size() - paddingSize; i < result.size(); ++i){
				if (static_cast<unsigned char>(result[i]) != paddingSize){
					validPadding = false;
					break;
				}
			}
			if (validPadding){
				result.resize(result.size() - paddingSize);
			}
		}
	}

	return result;
}

int main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	// Fixed inputs for a complete DES round-trip demo.
	const uint64_t key = 0x182BAC27CAB12341ULL;
	const string plaintext = "Sinh Viết Đang is a good boy!!!";

	KeySchedule keySchedule;
	vector<uint64_t> roundKeys = keySchedule.generateRoundKeys(key, 16);
	DES des;

	vector<uint64_t> plainBlocks = stringToBlocks(plaintext);
	vector<uint64_t> cipherBlocks;
	vector<uint64_t> decryptedBlocks;

	cout << "Plaintext: " << plaintext << '\n';
	cout << "Key: 0x" << hex << uppercase << setw(16) << setfill('0') << key << dec << '\n';
	cout << "\n--- Ciphertext blocks ---\n";
	for (size_t i = 0; i < plainBlocks.size(); ++i){
		uint64_t encryptedBlock = des.encrypt(plainBlocks[i], roundKeys);
		cipherBlocks.push_back(encryptedBlock);
		cout << dec;
		cout << "Block " << (i + 1) << ": 0x"
			 << hex << uppercase << setw(16) << setfill('0') << encryptedBlock << '\n';
	}

	cout << dec;

	cout << "\n--- Decrypted blocks ---\n";
	for (size_t i = 0; i < cipherBlocks.size(); ++i){
		uint64_t decryptedBlock = des.decrypt(cipherBlocks[i], roundKeys);
		decryptedBlocks.push_back(decryptedBlock);
		cout << dec;
		cout << "Block " << (i + 1) << ": 0x"
			 << hex << uppercase << setw(16) << setfill('0') << decryptedBlock << '\n';
	}

	string decryptedText = blocksToString(decryptedBlocks);
	cout << dec;

	cout << "\nRecovered text: " << decryptedText << '\n';

	return 0;
}