#include<bits/stdc++.h>
#include "RSA.h"

using namespace std;

uint64_t RSA::isPrime(uint64_t n) {

    // rabin miller primality test

    if (n <= 1) return false;
    for (uint8_t i = 0; i < primeNumbers.size(); i++) {
        if (n == primeNumbers[i]) return true;
        if (n % primeNumbers[i] == 0) return false;
    }

    if(n <= primeNumbers.back()) return false;

    uint64_t d = n - 1;
    int s = 0;
    while((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    uniform_int_distribution<uint64_t> dis(2, n - 2);
    for (uint8_t i = 0;i < attempts;i++){
        uint64_t a = dis(gen);
        uint64_t x = power(a, d, n);
        if (x == 1 || x == n - 1) continue;
        
        bool composite = true;
        for (int r = 1; r < s; r++) {
            x = (uint64_t)(((unsigned __int128)x * x) % n);
            if (x == 1) return false;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }

    return true;
}

uint64_t RSA::findPrime() {
    uniform_int_distribution<uint64_t> dis(1ULL << 31, 1ULL << 32);
    uint64_t primeCandidate;
    do {
        primeCandidate = dis(gen);
    } while (!isPrime(primeCandidate));
    return primeCandidate;
}

uint64_t RSA::gcd(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

uint64_t RSA::modInverse(uint64_t a, uint64_t m) {
    __int128 m0 = m, t, q;
    __int128 x0 = 0, x1 = 1;
    __int128 a_128 = a;

    if (m == 1) return 0;

    while (a_128 > 1) {
        q = a_128 / m;
        t = m;
        m = a_128 % m; a_128 = t;
        t = x0;
        x0 = x1 - q * x0; x1 = t;
    }

    if (x1 < 0) x1 += m0;

    return (uint64_t)x1;
}

RSA::RSA(uint64_t p, uint64_t q, uint64_t e) {
    this->p = p;
    this->q = q;
    n = p * q;
    phi = (p - 1) * (q - 1);
    this->e = e;
    d = modInverse(e, phi);
}

RSA::RSA(uint64_t p, uint64_t q) {
    this->p = p;
    this->q = q;
    n = p * q;
    phi = (p - 1) * (q - 1);
    e = 65537; // common choice for e
    d = modInverse(e, phi);
}
RSA::RSA() {
    p = findPrime();
    q = findPrime();
    n = p * q;
    phi = (p - 1) * (q - 1);
    e = 65537; // common choice for e
    d = modInverse(e, phi);
}

uint64_t RSA::power(uint64_t x, uint64_t y, uint64_t m) {
    uint64_t result = 1;
    x = x % m;
    while (y > 0) {
        if (y & 1) result = (uint64_t)(((unsigned __int128)result * x) % m);
        y = y >> 1;
        x = (uint64_t)(((unsigned __int128)x * x) % m);
    }
    return result;
}

uint64_t RSA::encrypt(uint64_t message) {
    cout << "Public Key (n, e): (" << n << ", " << e << ")\n";
    return power(message, e, n);
}

uint64_t RSA::decrypt(uint64_t messagecipher) {
    cout << "Private Key (n, d): (" << n << ", " << d << ")\n";
    return power(messagecipher, d, n);
}