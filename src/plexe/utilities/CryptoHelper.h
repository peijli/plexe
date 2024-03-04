#ifndef CRYPTOHELPER_H_
#define CRYPTOHELPER_H_

#include <string>
#include <mutex>

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

        static const std::uint32_t MAX_PRIVATE_KEY = 10;

        // constants for diffe-hellman key exchange
        static const std::uint32_t PUBLIC_MODULUS = 7;
        static const std::uint32_t PUBLIC_BASE = 2;

        // functions for key exchange
        static std::uint32_t computeSharedKey(std::uint32_t privateKey);
        static std::uint32_t computeSharedSecret(std::uint32_t privateKey, std::uint32_t sharedKey);
        
        static std::uint32_t power(std::uint32_t base, std::uint32_t exp);
    };
}

#endif /* CRYPTOHELPER_H_ */
