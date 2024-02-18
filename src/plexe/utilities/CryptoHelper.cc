// #include "plexe/utilities/CryptoHelper.h"
#include "CryptoHelper.h"
#include <sstream>
#include <stdlib.h>
#include <time.h>

namespace plexe {
    /** Trivial implementation of a symmetric encryption algorithm 
     * similar to the AES algorithm with cipher block chaining (CBC) mode
     */
    std::string CryptoHelper::symmetricEncrypt(std::string plaintext, std::string key) {
        std::string ciphertext = plaintext;
        for (int i = 0; i < plaintext.length(); i++) {
            ciphertext[i] = plaintext[i] ^ key[i % key.length()];
        }
        return ciphertext;
    }
    char* CryptoHelper::symmetricEncrypt(const char* plaintext, int size, std::string key) {
        char* ciphertext = new char[size];
        for (int i = 0; i < size; i++) {
            ciphertext[i] = plaintext[i] ^ key[i % key.length()];
        }
        return ciphertext;
    }
    /**
     * Trivial implementation of a symmetric decryption algorithm
     * similar to the AES algorithm with cipher block chaining (CBC) mode
     */
    std::string CryptoHelper::symmetricDecrypt(std::string ciphertext, std::string key) {
        std::string plaintext = ciphertext;
        for (int i = 0; i < ciphertext.length(); i++) {
            plaintext[i] = ciphertext[i] ^ key[i % key.length()];
        }
        return plaintext;
    }
    char* CryptoHelper::symmetricDecrypt(const char* ciphertext, int size, std::string key) {
        char* plaintext = new char[size];
        for (int i = 0; i < size; i++) {
            plaintext[i] = ciphertext[i] ^ key[i % key.length()];
        }
        return plaintext;
    }
    /**
     * Trivial implementation of an asymmetric encryption algorithm
     * similar to the RSA algorithm
     */
    std::string CryptoHelper::asymmetricEncrypt(std::string plaintext, std::string publicKey) {
        std::string ciphertext = plaintext;
        for (int i = 0; i < plaintext.length(); i++) {
            ciphertext[i] = plaintext[i] ^ publicKey[i % publicKey.length()];
        }
        return ciphertext;
    }
    char* CryptoHelper::asymmetricEncrypt(const char* plaintext, int size, std::string publicKey) {
        return nullptr;
    }
    /**
     * Trivial implementation of an asymmetric decryption algorithm
     * similar to the RSA algorithm
     */
    std::string CryptoHelper::asymmetricDecrypt(std::string ciphertext, std::string privateKey) {
        std::string plaintext = ciphertext;
        for (int i = 0; i < ciphertext.length(); i++) {
            plaintext[i] = ciphertext[i] ^ privateKey[i % privateKey.length()];
        }
        return plaintext;
    }
    char* CryptoHelper::asymmetricDecrypt(const char* ciphertext, int size, std::string privateKey) {
        return nullptr;
    }
    /**
     * Trivial implementation of a digital signature algorithm
     * similar to the RSA algorithm
     */
    std::string CryptoHelper::sign(std::string message, std::string privateKey) {
        return message + privateKey;
    }
    char* CryptoHelper::sign(const char* message, int size, std::string privateKey) {
        return nullptr;
    }
    /**
     * Trivial implementation of a digital signature verification algorithm
     * similar to the RSA algorithm
     */
    bool CryptoHelper::verify(std::string message, std::string signature, std::string publicKey) {
        return message == signature.substr(0, message.length()) && signature.substr(message.length()) == publicKey;
    }
    /**
     * Converts a string of hexadecimal characters to a string of ASCII characters
     */
    std::string CryptoHelper::hexToString(std::string hex) {
        std::string str;
        for (size_t i = 0; i < hex.length(); i += 2) {
            str += (char)std::stoi(hex.substr(i, 2), nullptr, 16);
        }
        return str;
    }
    /**
     * Converts a string of ASCII characters to a string of hexadecimal characters
     */
    std::string CryptoHelper::stringToHex(std::string str) {
        std::stringstream stream;
        stream << str;
        std::string hex;
        stream >> std::hex >> hex;
        return hex;
    }
    /**
     * Generates a symmetric key
     * @return a string representing the symmetric key
     * @note This is a trivial implementation that returns a fixed key for easier testing
    */
    std::string CryptoHelper::generateSymmetricKey(bool random) {
        if (!random) {
            return "114514";
        }
        else {
            srand(time(NULL));
            std::string key = "";
            for (int i = 0; i < 16; i++) {
                key += (char)(rand() % 256);
            }
            return key;
        }
    }
}
