#include "plexe/apps/SecurePlatooningApp.h"
#include "plexe/protocols/BaseProtocol.h"
#include "veins/modules/messages/BaseFrame1609_4_m.h"
#include "veins/base/messages/MacPkt_m.h"
#include "veins/modules/mac/ieee80211p/Mac1609_4.h"
#include "veins/base/utils/FindModule.h"
#include "plexe/messages/PlexeInterfaceControlInfo_m.h"
#include "plexe/driver/PlexeRadioDriverInterface.h"
#include "plexe/utilities/CryptoHelper.h"

/*
* TODO LIST
* - [ ] Try to send and receive an encrypted message
* - [ ] Try to share encrypted vehicle status data with platoon members
* - [ ] Try to re-implement PlatooningBeacon
*/

using namespace veins;

namespace plexe {
    Define_Module(SecurePlatooningApp);

    void SecurePlatooningApp::initialize(int stage){
        BaseApp::initialize(stage);
        if (stage != 0) {
            // output to console the name of this app and the vehicle it is attached to
            auto platoonId = positionHelper->getPlatoonId();
            auto vehicleId = positionHelper->getId();
            auto externalId = positionHelper->getExternalId();
            // auto platoonFormation = positionHelper->getPlatoonFormation();
            std::string msg = "SecurePlatooningApp::initialize: " + std::to_string(vehicleId) + " in platoon " + std::to_string(platoonId) + " with external ID " + externalId + " and stage " + std::to_string(stage);
            getSimulation()->getEnvir()->alert(msg.c_str());
        }
    }

    void SecurePlatooningApp::handleLowerMsg(cMessage* msg) {
        // getSimulation()->getEnvir()->alert("SecurePlatooningApp::handleLowerMsg");
        BaseApp::handleLowerMsg(msg);
    }

    /**
     * Send a message to a specific destination via the OMNet++ MAC layer
     * @note I hope I never have to mess with the backend indeed!
     * @param msg the message to be sent
     * @param destination the ID of the destination vehicle
     */
    void SecurePlatooningApp::sendUnicast(cPacket* msg, int destination) {
        // getSimulation()->getEnvir()->alert("SecurePlatooningApp::sendUnicast");
        Enter_Method_Silent();
        take(msg);
        // encapsulate the message in a BaseFrame1609_4 and send it to the MAC layer
        veins::BaseFrame1609_4* frame = new veins::BaseFrame1609_4(
            "BaseFrame1609_4", msg->getKind());
        frame->setRecipientAddress(destination);
        // prepare the frequency channel for the message
        frame->setChannelNumber(static_cast<int>(Channel::cch));
        frame->encapsulate(msg);
        PlexeInterfaceControlInfo* controlInfo = new PlexeInterfaceControlInfo();
        controlInfo->setInterfaces(plexe::PlexeRadioInterfaces::VEINS_11P);
        frame->setControlInfo(controlInfo);
        // push the frame down to the MAC layer
        sendDown(frame);
    }
}

