#include "plexe/apps/BaseApp.h"
#include "plexe/apps/GeneralPlatooningApp.h"

#include "plexe/scenarios/BaseScenario.h"
#include "plexe/CC_Const.h"
#include "plexe/messages/SecureManeuverMessage_m.h"
#include <string>


/*
TODO list:
- [ ] Figure out difference between SecurePlatooningApp and GeneralPlatooningApp
- [ ] Figure out the maneuvers and scenarios involved in platoon join and merging scenarios
- [ ] Figure out how to work with these...
*/

namespace plexe {
    class SecurePlatooningApp : public GeneralPlatooningApp {
    // // std::string symmetricKey;
    // std::uint32_t symmetricKey;
    // public:
    //     // virtual void sendUnicast(cPacket* msg, int destination);
    //     void sendSecuredMessage(std::string message, int destination);
    protected:
        virtual void initialize(int stage) override;
        virtual void handleLowerMsg(cMessage* msg) override;
    //     // plexe::BaseScenario* scenario;
    };
}
