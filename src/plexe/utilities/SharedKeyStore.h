#ifndef SHAREDKEYSTORE_H_
#define SHAREDKEYSTORE_H_

#include <map>
#include <string>
#include "CryptoHelper.h"

namespace plexe {
    class SharedKeyStore {
    public:
        SharedKeyStore();
        SharedKeyStore(std::uint32_t privateLeaderKey);
        std::uint32_t getSharedKey(int vehicleId);
        void setSharedKey(int vehicleId, std::uint32_t sharedKey);
        std::size_t getSize();
    private:
        /** Storing the shared secret between platoon leader and all followers */
        std::map<int, std::uint32_t> sharedKeys;
        std::uint32_t privateLeaderKey;
    };
}

#endif /* SHAREDKEYSTORE_H_ */
