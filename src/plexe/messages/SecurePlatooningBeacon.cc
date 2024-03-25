// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include "SecurePlatooningBeacon.h"
#include "plexe/utilities/StringHelper.h"

#include <iostream>
#include <string>

Register_Class(SecurePlatooningBeacon);
void SecurePlatooningBeacon::copy(const SecurePlatooningBeacon& other) {
    // TODO: this might not work! Check if the deep copy is actually performed
    // char * encryptedDataBuffer = this->encryptedData.reserve(other.encryptedDataLength);
    // encryptedDataBuffer = plexe::StringHelper::customDeepCopy(other.encryptedData.c_str(), other.encryptedDataLength);
    char * buffer = this->encryptedData.reserve(other.encryptedDataLength);
    const char * otherBuffer = other.encryptedData.c_str();
    for (int i = 0; i < other.encryptedDataLength; i++) {
        buffer[i] = otherBuffer[i];
    }
    encryptedDataLength = other.encryptedDataLength;
    buffer = this->signature.reserve(other.signatureLength);
    otherBuffer = other.signature.c_str();
    for (int i = 0; i < other.signatureLength; i++) {
        buffer[i] = otherBuffer[i];
    }
    signatureLength = other.signatureLength;
    buffer = this->publicKey.reserve(other.publicKeyLength);
    otherBuffer = other.publicKey.c_str();
    for (int i = 0; i < other.publicKeyLength; i++) {
        buffer[i] = otherBuffer[i];
    }
    publicKeyLength = other.publicKeyLength;
    // we don't need to deep copy the algorithm, as it is a constant string
    algorithm = other.algorithm;
}

void SecurePlatooningBeacon::setEncryptedData(const char* encryptedData) {
    char* buffer = this->encryptedData.reserve(encryptedDataLength);
    for (int i = 0; i < encryptedDataLength; i++) {
        buffer[i] = encryptedData[i];
    }
}

void SecurePlatooningBeacon::setSignature(const char* signature) {
    char* buffer = this->signature.reserve(signatureLength);
    for (int i = 0; i < signatureLength; i++) {
        buffer[i] = signature[i];
    }
}

void SecurePlatooningBeacon::setPublicKey(const char* publicKey) {
    char* buffer = this->publicKey.reserve(publicKeyLength);
    for (int i = 0; i < publicKeyLength; i++) {
        buffer[i] = publicKey[i];
    }
}

void SecurePlatooningBeacon::setEncryptedData(const char* encryptedData, int length) {
    this->encryptedDataLength = length;
    char* buffer = this->encryptedData.reserve(length);
    for (int i = 0; i < length; i++) {
        buffer[i] = encryptedData[i];
    }
}

void SecurePlatooningBeacon::setSignature(const char* signature, int length) {
    this->signatureLength = length;
    char* buffer = this->signature.reserve(length);
    for (int i = 0; i < length; i++) {
        buffer[i] = signature[i];
    }
}

void SecurePlatooningBeacon::setPublicKey(const char* publicKey, int length) {
    this->publicKeyLength = length;
    char* buffer = this->publicKey.reserve(length);
    for (int i = 0; i < length; i++) {
        buffer[i] = publicKey[i];
    }
}

const char * SecurePlatooningBeacon::getEncryptedData() const {
    return plexe::StringHelper::customDeepCopy(this->encryptedData.c_str(), encryptedDataLength);
}

const char * SecurePlatooningBeacon::getSignature() const {
    return plexe::StringHelper::customDeepCopy(this->signature.c_str(), signatureLength);
}

const char * SecurePlatooningBeacon::getPublicKey() const {
    return plexe::StringHelper::customDeepCopy(this->publicKey.c_str(), publicKeyLength);
}

std::uint32_t SecurePlatooningBeacon::getPublicKeyAsInt() const {
    return std::stoul(this->publicKey.c_str());
}

void SecurePlatooningBeacon::setPublicKey(std::uint32_t publicKey) {
    std::string publicKeyStr = std::to_string(publicKey);
    this->publicKeyLength = publicKeyStr.length();
    char* buffer = this->publicKey.reserve(publicKeyStr.length());
    for (int i = 0; i < publicKeyStr.length(); i++) {
        buffer[i] = publicKeyStr[i];
    }
}

