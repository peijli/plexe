#ifndef CRYPTOHELPER_H_
#define CRYPTOHELPER_H_

#include <string>

namespace plexe {
    class CryptoHelper {
    public:
        static std::string symmetricEncrypt(std::string plaintext, std::string key);
        static char* symmetricEncrypt(const char* plaintext, int size, std::string key);
        static std::string symmetricDecrypt(std::string ciphertext, std::string key);
        static char* symmetricDecrypt(const char* ciphertext, int size, std::string key);
        static std::string asymmetricEncrypt(std::string plaintext, std::string publicKey);
        static char* asymmetricEncrypt(const char* plaintext, int size, std::string publicKey);
        static std::string asymmetricDecrypt(std::string ciphertext, std::string privateKey);
        static char* asymmetricDecrypt(const char* ciphertext, int size, std::string privateKey);
        static std::string sign(std::string message, std::string privateKey);
        static char* sign(const char* message, int size, std::string privateKey);
        static bool verify(std::string message, std::string signature, std::string publicKey);
        static std::string hexToString(std::string hex);
        static std::string stringToHex(std::string str);
        static std::string generateSymmetricKey(bool random = false);
    };
}

#endif /* CRYPTOHELPER_H_ */
