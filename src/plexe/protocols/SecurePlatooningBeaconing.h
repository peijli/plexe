#ifndef SECUREPLATOONINGBEACONING_H_
#define SECUREPLATOONINGBEACONING_H_

#include "BaseProtocol.h"
#include "SimplePlatooningBeaconing.h"
#include "plexe/utilities/CryptoHelper.h"
#include "plexe/messages/PlatooningBeacon_m.h"
#include "plexe/messages/SecurePlatooningBeacon.h"

#include "veins/base/modules/BaseApplLayer.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/messages/BaseFrame1609_4_m.h"

#include "plexe/mobility/CommandInterface.h"
#include "plexe/utilities/BasePositionHelper.h"
#include "plexe/utilities/SharedKeyStore.h"
#include "plexe/messages/KeyExchangeMessage_m.h"
#include "plexe/driver/PlexeRadioDriverInterface.h"

#include <memory>
#include <tuple>

namespace plexe {
    class SecurePlatooningBeaconing : public SimplePlatooningBeaconing {
    private:
        // TODO: write this key to the plexe TraCI vehicle interface
        std::uint32_t fallbackKey;
        std::uint32_t symmetricKey;
        std::uint32_t privateKey; // for Diffie-Hellman key exchange
        std::uint32_t publicKey; // for Diffie-Hellman key exchange
        SharedKeyStore sharedKeyStore; // for storing shared keys if the vehicle is a leader
        // overloaded method to check for duplicated secure beacons
        bool isDuplicated(const SecurePlatooningBeacon* beacon);
        // returns a SecurePlatooningBeacon from a PlatooningBeacon
        SecurePlatooningBeacon* encryptBeacon(const PlatooningBeacon* beacon, int destinationAddress);
        KeyExchangeMessage* createKeyExchangeMessage(int destinationAddress, bool acknowledge=false);
        virtual void sendUnicast(cPacket* msg, int destination);
        void handleKeyExchangeMessage(KeyExchangeMessage* msg);
    protected:
        virtual void handleSelfMsg(cMessage* msg) override;
        virtual std::unique_ptr<BaseFrame1609_4> createBeacon(int destinationAddress) override;
        virtual void handleLowerMsg(cMessage* msg) override;
        // returns a decrypted and parsed beacon from received SecurePlatooningBeacon
        PlatooningBeacon* handleSecurePlatooningBeacon(SecurePlatooningBeacon* beacon);
        std::map<std::string, std::string>* decryptBeacon(const SecurePlatooningBeacon* beacon);
        /**
         * Sends a platooning message with all information about the car. This is an utility function for
         * subclasses
         */
        virtual void sendPlatooningMessage(int destinationAddress, enum PlexeRadioInterfaces interfaces = PlexeRadioInterfaces::ALL) override;
    public:
        SecurePlatooningBeaconing();
        virtual ~SecurePlatooningBeaconing();
        virtual void initialize(int stage) override;
    };
} // namespace plexe

#endif /* SECUREPLATOONINGBEACONING_H_ */
