#ifndef CRYPTOHELPER_H_
#define CRYPTOHELPER_H_

#include <string>

namespace plexe {
    class CryptoHelper {
    public:
        static std::string symmetricEncrypt(std::string plaintext, std::string key);
        static std::string symmetricDecrypt(std::string ciphertext, std::string key);
        static std::string asymmetricEncrypt(std::string plaintext, std::string publicKey);
        static std::string asymmetricDecrypt(std::string ciphertext, std::string privateKey);
        static std::string sign(std::string message, std::string privateKey);
        static bool verify(std::string message, std::string signature, std::string publicKey);
        static std::string hexToString(std::string hex);
        static std::string stringToHex(std::string str);
        static std::string generateSymmetricKey();
    };
}

#endif /* CRYPTOHELPER_H_ */
