#include "plexe/apps/BaseApp.h"
#include "plexe/scenarios/BaseScenario.h"
#include "plexe/messages/AbandonMessage_m.h"
#include "plexe/CC_Const.h"

class Example7App: public plexe::BaseApp {

public:
    void sendAbandonMessage();
    virtual void sendUnicast(cPacket* msg, int destination);

protected:
    virtual void initialize(int stage) override;
    virtual void handleLowerMsg(cMessage* msg) override;
    plexe::BaseScenario* scenario;

private:
    AbandonPlatoon* createAbandonMessage();
    NewFormation* createNewFormationMessage(const std::vector<int>& newPlatoonFormation);
    void handleAbandoonMessage(AbandonPlatoon* msg);
    void handleNewFormationMessage(NewFormation* msg);
    void sendNewFormationMessage(const std::vector<int>& newPlatoonFormation);
};
