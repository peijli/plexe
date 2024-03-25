#include "SharedKeyStore.h"

namespace plexe {
    SharedKeyStore::SharedKeyStore() {
        privateLeaderKey = CryptoHelper::generateSymmetricKey(false);
    }
    SharedKeyStore::SharedKeyStore(std::uint32_t privateLeaderKey) {
        this->privateLeaderKey = privateLeaderKey;
    }
    std::uint32_t SharedKeyStore::getSharedKey(int vehicleId) {
        return sharedKeys[vehicleId];
    }
    void SharedKeyStore::setSharedKey(int vehicleId, std::uint32_t sharedKey) {
        sharedKeys[vehicleId] = sharedKey;
    }
}
