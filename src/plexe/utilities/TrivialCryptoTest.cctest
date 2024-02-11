// Sanity checks for the CryptoHelper class

#include <string>
#include <iostream>
#include <cassert>
#include "plexe/utilities/CryptoHelper.h"
// #include "CryptoHelper.h"

int main() {
    std::string plaintext = "Hello, world!";
    std::string key = "key";
    std::string publicKey = "public";
    std::string privateKey = "private";

    std::cout << "Plaintext: " << plaintext << std::endl;
    std::cout << "Key: " << key << std::endl;
    std::cout << "Plaintext in hex: " << plexe::CryptoHelper::stringToHex(plaintext) << std::endl;
    std::cout << "Key in hex: " << plexe::CryptoHelper::stringToHex(key) << std::endl;

    std::cout << "Testing symmetric encryption and decryption" << std::endl;
    std::string ciphertext = plexe::CryptoHelper::symmetricEncrypt(plaintext, key);
    std::string decrypted = plexe::CryptoHelper::symmetricDecrypt(ciphertext, key);
    std::cout << "Cipher text: " << ciphertext << std::endl;
    std::cout << "Cipher text in hex: " << plexe::CryptoHelper::stringToHex(ciphertext) << std::endl;
    std::cout << "Decrypted text: " << decrypted << std::endl;
    std::cout << "Decrypted text in hex: " << plexe::CryptoHelper::stringToHex(decrypted) << std::endl;
    assert(plaintext == decrypted);
}
