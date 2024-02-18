#include "plexe/apps/BaseApp.h"
#include "plexe/scenarios/BaseScenario.h"
#include "plexe/CC_Const.h"
#include "plexe/messages/SecureManeuverMessage_m.h"
#include <string>

namespace plexe {
    class SecurePlatooningApp : public plexe::BaseApp {
    // std::string symmetricKey;
    std::uint32_t symmetricKey;
    public:
    //virtual ~SecurePlatooningApp()
    //{
    //    delete symmetricKey;
    //}
        virtual void sendUnicast(cPacket* msg, int destination);
        void sendSecuredMessage(std::string message, int destination);
    protected:
        virtual void initialize(int stage) override;
        virtual void handleLowerMsg(cMessage* msg) override;
        plexe::BaseScenario* scenario;
    private:
        SecureManeuverMessage* createSecureManeuverMessage(std::string message, std::string algorithm);
        std::string handleSecureManeuverMessage(SecureManeuverMessage* msg);
        // std::string toString();
    };
}
