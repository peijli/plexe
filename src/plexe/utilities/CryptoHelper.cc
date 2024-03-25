// #include "plexe/utilities/CryptoHelper.h"
#include "CryptoHelper.h"

// #ifndef CRYPTOPP_LIBS
// #define CRYPTOPP_LIBS
// #include <cryptopp/cryptlib.h>
// #include <cryptopp/osrng.h>
// #include <cryptopp/integer.h>
// #endif

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <limits>
#include <cstddef>
#include <cassert>
#include <cmath>
#include "sha256.h"

namespace plexe {
    /** Trivial implementation of a symmetric encryption algorithm
     * similar to the AES algorithm with cipher block chaining (CBC) mode
     */
    std::string CryptoHelper::symmetricEncrypt(std::string plaintext, std::uint32_t key) {
        // using key as uint32_t
        std::string keyStr = std::to_string(key);
        std::string ciphertext = plaintext;
        for (int i = 0; i < plaintext.length(); i++) {
            ciphertext[i] = plaintext[i] ^ keyStr[i % keyStr.length()];
        }
        // CryptoPP::AutoSeededRandomPool prng;
        return ciphertext;
    }
    char* CryptoHelper::symmetricEncrypt(const char* plaintext, int size, std::uint32_t key) {
        std::string keyStr = std::to_string(key);
        char* ciphertext = new char[size];
        for (int i = 0; i < size; i++) {
            ciphertext[i] = plaintext[i] ^ keyStr[i % keyStr.length()];
        }
        return ciphertext;
    }
    /**
     * Trivial implementation of a symmetric decryption algorithm
     * similar to the AES algorithm with cipher block chaining (CBC) mode
     */
    std::string CryptoHelper::symmetricDecrypt(std::string ciphertext, std::uint32_t key) {
        std::string keyStr = std::to_string(key);
        std::string plaintext = ciphertext;
        for (int i = 0; i < ciphertext.length(); i++) {
            plaintext[i] = ciphertext[i] ^ keyStr[i % keyStr.length()];
        }
        return plaintext;
    }
    char* CryptoHelper::symmetricDecrypt(const char* ciphertext, int size, std::uint32_t key) {
        char* plaintext = new char[size];
        std::string keyStr = std::to_string(key);
        for (int i = 0; i < size; i++) {
            plaintext[i] = ciphertext[i] ^ keyStr[i % keyStr.length()];
        }
        return plaintext;
    }
    /**
     * Trivial implementation of an asymmetric encryption algorithm
     * similar to the RSA algorithm
     */
    std::string CryptoHelper::asymmetricEncrypt(std::string plaintext, std::uint32_t publicKey) {
        return plaintext + std::to_string(publicKey);
    }
    char* CryptoHelper::asymmetricEncrypt(const char* plaintext, int size, std::uint32_t publicKey) {
        return nullptr;
    }
    /**
     * Trivial implementation of an asymmetric decryption algorithm
     * similar to the RSA algorithm
     */
    std::string CryptoHelper::asymmetricDecrypt(std::string ciphertext, std::uint32_t privateKey) {
        return ciphertext.substr(0, ciphertext.length() - std::to_string(privateKey).length());
    }
    char* CryptoHelper::asymmetricDecrypt(const char* ciphertext, int size, std::uint32_t privateKey) {
        return nullptr;
    }
    /**
     * Trivial implementation of a digital signature algorithm
     * similar to the RSA algorithm
     */
    std::string CryptoHelper::sign(std::string message, std::uint32_t privateKey) {
        return message + std::to_string(privateKey);
    }
    char* CryptoHelper::sign(const char* message, int size, std::uint32_t privateKey) {
        return nullptr;
    }
    /**
     * Trivial implementation of a digital signature verification algorithm
     * similar to the RSA algorithm
     */
    bool CryptoHelper::verify(std::string message, std::string signature, std::uint32_t publicKey) {
        return message.substr(0, message.length() - std::to_string(publicKey).length()) == signature;
    }
    /**
     * Generates a symmetric key
     * @return a string representing the symmetric key
     * @note This is a trivial implementation that returns a fixed key for easier testing
    */
    std::uint32_t CryptoHelper::generateSymmetricKey(bool random) {
        static std::once_flag flag;
        if (!random) {
            // return 114514;
            // return a trivially small prime number
            return 3;
        } else {
            // call srand only once
            std::call_once(flag, []() { srand(time(NULL)); });
            std::uint32_t key = rand() % MAX_PRIVATE_KEY + 1;
            // CryptoPP::AutoSeededRandomPool prng;
            // CryptoPP::Integer keyInt(prng, 32);
            // std::uint32_t key = keyInt.ConvertToLong();
            return key;
        }
    }

    std::uint32_t CryptoHelper::computeSharedKey(std::uint32_t privateKey) {
        // return g^a mod p
        return power(PUBLIC_BASE, privateKey) % PUBLIC_MODULUS;
    }

    std::uint32_t CryptoHelper::computeSharedSecret(std::uint32_t privateKey, std::uint32_t sharedKey) {
        // return g^ab mod p
        return power(sharedKey, privateKey) % PUBLIC_MODULUS;
    }

    std::uint32_t CryptoHelper::power(std::uint32_t base, std::uint32_t exp) {
        std::uint32_t result = 1;
        for (int i = 0; i < exp; i++) {
            result *= base;
        }
        return result;
    }

    std::string CryptoHelper::computeMAC(std::string message, std::uint32_t key) {
        return sha256(std::to_string(key) + message);
    }

    bool CryptoHelper::verifyMAC(std::string message, std::string mac, std::uint32_t key) {
        std::string expectedMAC = computeMAC(message, key);
        return expectedMAC == mac;
    }
}
