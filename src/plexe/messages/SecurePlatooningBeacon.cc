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

    buffer = this->mac.reserve(other.macLength);
    otherBuffer = other.mac.c_str();
    for (int i = 0; i < other.macLength; i++) {
        buffer[i] = otherBuffer[i];
    }

    // we don't need to deep copy the algorithm, as it is a constant string
    algorithm = other.algorithm;
}

void SecurePlatooningBeacon::setEncryptedData(const char* encryptedData) {
    char* buffer = this->encryptedData.reserve(encryptedDataLength);
    for (int i = 0; i < encryptedDataLength; i++) {
        buffer[i] = encryptedData[i];
    }
}


void SecurePlatooningBeacon::setEncryptedData(const char* encryptedData, int length) {
    this->encryptedDataLength = length;
    char* buffer = this->encryptedData.reserve(length);
    for (int i = 0; i < length; i++) {
        buffer[i] = encryptedData[i];
    }
}


const char * SecurePlatooningBeacon::getEncryptedData() const {
    return plexe::StringHelper::customDeepCopy(this->encryptedData.c_str(), encryptedDataLength);
}

void SecurePlatooningBeacon::setMAC(const char* mac, int length) {
    this->macLength = length;
    char* buffer = this->mac.reserve(length);
    for (int i = 0; i < length; i++) {
        buffer[i] = mac[i];
    }
}

void SecurePlatooningBeacon::setMAC(std::string mac) {
    this->macLength = mac.length();
    char* buffer = this->mac.reserve(macLength);
    for (int i = 0; i < macLength; i++) {
        buffer[i] = mac[i];
    }
}

const char * SecurePlatooningBeacon::getMAC() const {
    return plexe::StringHelper::customDeepCopy(this->mac.c_str(), macLength);
}

std::string SecurePlatooningBeacon::getMACString() const {
    std::string macString;
    const char * macCStr = this->mac.c_str();
    for (int i = 0; i < macLength; i++) {
        macString += macCStr[i];
    }
    return macString;
}
