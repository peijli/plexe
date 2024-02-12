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

Register_Class(SecurePlatooningBeacon);
void SecurePlatooningBeacon::copy(const SecurePlatooningBeacon& other) {
    // TODO: this might not work! Check if the deep copy is actually performed
    // char * encryptedDataBuffer = this->encryptedData.reserve(other.encryptedDataLength);
    // encryptedDataBuffer = plexe::StringHelper::customDeepCopy(other.encryptedData.c_str(), other.encryptedDataLength);
    encryptedData = other.encryptedData;
    encryptedDataLength = other.encryptedDataLength;
    char * signatureBuffer = this->signature.reserve(other.signatureLength);
    signatureBuffer = plexe::StringHelper::customDeepCopy(other.signature.c_str(), other.signatureLength);
    signatureLength = other.signatureLength;
    char * publicKeyBuffer = this->publicKey.reserve(other.publicKeyLength);
    publicKeyBuffer = plexe::StringHelper::customDeepCopy(other.publicKey.c_str(), other.publicKeyLength);
    publicKeyLength = other.publicKeyLength;
    // we don't need to deep copy the algorithm, as it is a constant string
    algorithm = other.algorithm;
}

void SecurePlatooningBeacon::setEncryptedData(const char* encryptedData) {
    // char * encryptedDataBuffer = this->encryptedData.reserve(encryptedDataLength);
    // encryptedDataBuffer = plexe::StringHelper::customDeepCopy(encryptedData, encryptedDataLength);
    encryptedData = encryptedData;
}

void SecurePlatooningBeacon::setSignature(const char* signature) {
    char * signatureBuffer = this->signature.reserve(signatureLength);
    signatureBuffer = plexe::StringHelper::customDeepCopy(signature, signatureLength);
}

void SecurePlatooningBeacon::setPublicKey(const char* publicKey) {
    char * publicKeyBuffer = this->publicKey.reserve(publicKeyLength);
    publicKeyBuffer = plexe::StringHelper::customDeepCopy(publicKey, publicKeyLength);
}

void SecurePlatooningBeacon::setEncryptedData(const char* encryptedData, int length) {
    // char * encryptedDataBuffer = this->encryptedData.reserve(length);
    // encryptedDataBuffer = plexe::StringHelper::customDeepCopy(encryptedData, length);
    this->encryptedDataLength = length;
    this->encryptedData = encryptedData;
}

void SecurePlatooningBeacon::setSignature(const char* signature, int length) {
    char * signatureBuffer = this->signature.reserve(length);
    signatureBuffer = plexe::StringHelper::customDeepCopy(signature, length);
    signatureLength = length;
}

void SecurePlatooningBeacon::setPublicKey(const char* publicKey, int length) {
    char * publicKeyBuffer = this->publicKey.reserve(length);
    publicKeyBuffer = plexe::StringHelper::customDeepCopy(publicKey, length);
    publicKeyLength = length;
}

const char * SecurePlatooningBeacon::getEncryptedData() const {
    // return plexe::StringHelper::customDeepCopy(this->encryptedData.c_str(), encryptedDataLength);
    return encryptedData.c_str();
}

const char * SecurePlatooningBeacon::getSignature() const {
    return plexe::StringHelper::customDeepCopy(this->signature.c_str(), signatureLength);
}

const char * SecurePlatooningBeacon::getPublicKey() const {
    return plexe::StringHelper::customDeepCopy(this->publicKey.c_str(), publicKeyLength);
}

