#ifndef SECUREPLATOONINGBEACONING_H_
#define SECUREPLATOONINGBEACONING_H_

#include "BaseProtocol.h"
#include "SimplePlatooningBeaconing.h"
#include "plexe/utilities/CryptoHelper.h"
#include "plexe/messages/PlatooningBeacon_m.h"
#include "plexe/messages/SecurePlatooningBeacon_m.h"

#include "veins/base/modules/BaseApplLayer.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/messages/BaseFrame1609_4_m.h"

#include "plexe/mobility/CommandInterface.h"
#include "plexe/utilities/BasePositionHelper.h"

#include "plexe/driver/PlexeRadioDriverInterface.h"

#include <memory>
#include <tuple>

namespace plexe {
    class SecurePlatooningBeaconing : public SimplePlatooningBeaconing {
    private:
        // TODO: write this key to the plexe TraCI vehicle interface
        std::string symmetricKey;
        // overloaded method to check for duplicated secure beacons
        bool isDuplicated(const SecurePlatooningBeacon* beacon);
        // returns a SecurePlatooningBeacon from a PlatooningBeacon
        SecurePlatooningBeacon* encryptBeacon(const PlatooningBeacon* beacon);
        // returns a decrypted and parsed beacon from received SecurePlatooningBeacon
        PlatooningBeacon* decryptBeacon(const SecurePlatooningBeacon* beacon);
    protected:
        virtual void handleSelfMsg(cMessage* msg) override;
        virtual std::unique_ptr<BaseFrame1609_4> createBeacon(int destinationAddress) override;
        virtual void handleLowerMsg(cMessage* msg) override;
    public:
        SecurePlatooningBeaconing();
        virtual ~SecurePlatooningBeaconing();
        virtual void initialize(int stage) override;
    };
} // namespace plexe

#endif /* SECUREPLATOONINGBEACONING_H_ */
