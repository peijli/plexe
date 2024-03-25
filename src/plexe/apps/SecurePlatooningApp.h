#include "plexe/apps/BaseApp.h"
#include "plexe/scenarios/BaseScenario.h"
#include "plexe/CC_Const.h"
#include <string>

namespace plexe {
    class SecurePlatooningApp : public plexe::BaseApp {
    public:
        virtual void sendUnicast(cPacket* msg, int destination);
    protected:
        virtual void initialize(int stage) override;
        virtual void handleLowerMsg(cMessage* msg) override;
        plexe::BaseScenario* scenario;
    private:
        std::string symmetricKey;
        // std::string toString();
    };
}
