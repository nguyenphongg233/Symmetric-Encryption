#include<bits/stdc++.h>
using namespace std;
// RSA for encryption and decryption 128 bit key
struct RSA{
private: 
    uint64_t p, q, n, phi, e, d;
    uint64_t isPrime(uint64_t n);
    uint64_t findPrime();
    vector<uint64_t> primeNumbers = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
                                    53, 59, 61, 67, 71, 73, 79, 83, 89, 97,
                                    101, 103, 107, 109, 113};
    uint8_t attempts = 5;
    std::random_device rd;
    std::mt19937_64 gen{rd()};

    uint64_t gcd(uint64_t a, uint64_t b);
    uint64_t modInverse(uint64_t a, uint64_t m);
    uint64_t power(uint64_t x, uint64_t y, uint64_t m);

public:
    RSA();
    RSA(uint64_t p, uint64_t q);
    RSA(uint64_t p, uint64_t q, uint64_t e);
    uint64_t encrypt(uint64_t message);
    uint64_t decrypt(uint64_t messagecipher);
};