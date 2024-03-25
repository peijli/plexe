#ifndef CRYPTOHELPER_H_
#define CRYPTOHELPER_H_

#include <string>

namespace plexe {
    class CryptoHelper {
    public:
        static std::string symmetricEncrypt(std::string plaintext, std::uint32_t key);
        static char* symmetricEncrypt(const char* plaintext, int size, std::uint32_t key);
        static std::string symmetricDecrypt(std::string ciphertext, std::uint32_t key);
        static char* symmetricDecrypt(const char* ciphertext, int size, std::uint32_t key);
        static std::string asymmetricEncrypt(std::string plaintext, std::uint32_t publicKey);
        static char* asymmetricEncrypt(const char* plaintext, int size, std::uint32_t publicKey);
        static std::string asymmetricDecrypt(std::string ciphertext, std::uint32_t privateKey);
        static char* asymmetricDecrypt(const char* ciphertext, int size, std::uint32_t privateKey);
        static std::string sign(std::string message, std::uint32_t privateKey);
        static char* sign(const char* message, int size, std::uint32_t privateKey);
        static bool verify(std::string message, std::string signature, std::uint32_t publicKey);
        static std::uint32_t generateSymmetricKey(bool random = false);
    };
}

#endif /* CRYPTOHELPER_H_ */
