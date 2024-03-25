#include "plexe/apps/Example7App.h"
#include "plexe/protocols/BaseProtocol.h"
// #include "plexe/scenarios/BaseScenario.h"
#include "veins/modules/messages/BaseFrame1609_4_m.h"
#include "veins/base/messages/MacPkt_m.h"
#include "veins/modules/mac/ieee80211p/Mac1609_4.h"
#include "veins/base/utils/FindModule.h"
#include "plexe/messages/PlexeInterfaceControlInfo_m.h"
#include "plexe/driver/PlexeRadioDriverInterface.h"
// #include "plexe/scenarios/BaseScenario.h"


using namespace veins;

namespace plexe {
    Define_Module(Example7App);

    /**
     * Correctly initialize the application and bound it to the MAC layer
     */
    void Example7App::initialize(int stage) {
        BaseApp::initialize(stage);
        // TODO: how do you define stage numbers?
        if (stage == 1) {
            // connect to the MAC layer
            protocol->registerApplication(
                MANEUVER_TYPE,
                gate("lowerLayerIn"),
                gate("lowerLayerOut"),
                gate("lowerControlIn"),
                gate("lowerControlOut"));
            // register to the signal indicating failed unicast transmissions
            findHost()->subscribe(veins::Mac1609_4::sigRetriesExceeded, this);
            scenario = veins::FindModule<plexe::BaseScenario*>::findSubModule(getParentModule());
        }
    }

    /**
     * To be called when the vehicle wants to abandon the platoon
     * by the upper-level scenario manager
     */
    void Example7App::sendAbandonMessage() {
        // Create environment alert
        // TODO: THIS IS HOW TO DISPLAY TEXT IN OMNET++ SIMULATION
        getSimulation()->getActiveEnvir()->alert(
            "Abandoning platoon -- sending abandon message to leader\n");
        AbandonPlatoon* abandonMessage = createAbandonMessage();
        // sendUnicast(abandonMessage, abandonMessage->getPlatoonId());
        sendUnicast(abandonMessage, abandonMessage->getDestinationId());
    }

    /**
     * Create an instance of the AbandonPlatoon message,
     * designating the current vehicle as the sender and the leader as the receiver
     */
    AbandonPlatoon* Example7App::createAbandonMessage() {
        // allocate a new message
        AbandonPlatoon* abandonMessage = new AbandonPlatoon();
        // configure basic maneuver message fields
        abandonMessage->setPlatoonId(positionHelper->getPlatoonId());
        abandonMessage->setVehicleId(positionHelper->getId());
        // set the destination of the message to the leader of the platoon
        abandonMessage->setDestinationId(positionHelper->getLeaderId());
        abandonMessage->setExternalId(positionHelper->getExternalId().c_str());
        // tag the message as a maneuver message
        abandonMessage->setKind(MANEUVER_TYPE);
        return abandonMessage;
    }

    /**
     * Send a message to a specific destination via the OMNet++ MAC layer
     * @note I hope I never have to mess with the backend indeed!
     * @param msg the message to be sent
     * @param destination the ID of the destination vehicle
     */
    void Example7App::sendUnicast(cPacket* msg, int destination) {
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

    /**
     * Handle messages received from the MAC layer
     * @param msg the message received from the MAC layer
     */
    void Example7App::handleLowerMsg(cMessage* msg) {
        veins::BaseFrame1609_4* frame = check_and_cast<veins::BaseFrame1609_4*>(msg);
        cPacket* enc = frame->getEncapsulatedPacket();
        ASSERT2(enc, "received a BaseFrame1609_4s with nothing inside");

        // filter the messages based on their kind
        if (enc->getKind() == MANEUVER_TYPE) {
            ManeuverMessage* mm = check_and_cast<ManeuverMessage*>(
                frame->decapsulate());
            // we would care about the abandon platoon message or the update platoon formation message
            if (AbandonPlatoon* msg = dynamic_cast<AbandonPlatoon*>(mm)) {
                handleAbandonMessage(msg);
                delete msg;
            } else if (UpdatePlatoonFormation* msg = dynamic_cast<UpdatePlatoonFormation*>(mm)) {
                handleUpdatePlatoonFormationMessage(msg);
                delete msg;
            }
            // delete the frame if it is not needed
            delete frame;
        } else // dispatch the message to the parent class
            BaseApp::handleLowerMsg(msg);
    }

    /**
     * Handle the reception of an abandon platoon message
     * @param msg the abandon platoon message received
    */
    void Example7App::handleAbandonMessage(AbandonPlatoon* msg) {
        // ensure that the message is for the right platoon
        if (msg->getPlatoonId() != positionHelper->getPlatoonId())
            return;
        // ensure that the message is for the leader of the platoon
        if (msg->getDestinationId() != positionHelper->getLeaderId())
            return;
        // ensure that the message is for this vehicle
        if (msg->getDestinationId() != positionHelper->getId())
            return;

        // Retrieving relevant info from Abandon Message
        int leaderID, leaverID, platoonID;
        leaderID = positionHelper->getId();
        leaverID = msg->getVehicleId();
        platoonID = msg->getPlatoonId();

        // Informing SUMO via Plexe Interface to remove vehicle from platoon
        plexeTraciVehicle->removePlatoonMember(msg->getExternalId());

        // Changing platoon Formation...
        std::vector<int> formation = positionHelper->getPlatoonFormation();

        // Removing the vehicle that abandoned the platoon
        formation.pop_back();
        // Updating the platoon formation
        positionHelper->setPlatoonFormation(formation);

        char text[250];
        sprintf(text, "LEADER[%d]: I'm removing v<%d> from platoon<%d>\n", leaderID, leaverID, platoonID);
        // log the event
        LOG << text << endl;
        // display the event in the simulation
        getSimulation()->getActiveEnvir()->alert(text);

        // Informing the rest of the platoon about the new formation
        sendUpdatePlatoonFormationMessage(formation);
    }

    /**
     * Send an update platoon formation message to each platoon member
     * @param newPlatoonFormation the new platoon formation to be sent
     */
    void Example7App::sendUpdatePlatoonFormationMessage(const std::vector<int>& newPlatoonFormation) {
        // create the message with every field except the destination pre-populated
        UpdatePlatoonFormation* newFormationMessage = createUpdatePlatoonFormationMessage(newPlatoonFormation);
        int dest;
        // iterate over the platoon members and send the message to each one
        for (int i = 0; i < newPlatoonFormation.size(); i++) {
            dest = newPlatoonFormation[i];
            // create a duplicate of the message for each destination
            UpdatePlatoonFormation* duplicate = newFormationMessage->dup();
            // set the destination of the duplicate message
            duplicate->setDestinationId(dest);
            // send the duplicate message to the destination
            sendUnicast(duplicate, dest);
        }
        // delete the original message
        delete newFormationMessage;
    }

    /**
     * Create an instance of the UpdatePlatoonFormation message with the given platoon formation
     * Every field except the destination is pre-populated
     * @param newPlatoonFormation the new platoon formation to be sent
     * @return a pointer to the newly created message
     */
    UpdatePlatoonFormation* Example7App::createUpdatePlatoonFormationMessage(const std::vector<int>& newPlatoonFormation) {
        UpdatePlatoonFormation* newFormationMessage = new UpdatePlatoonFormation();
        newFormationMessage->setPlatoonId(positionHelper->getPlatoonId());
        newFormationMessage->setVehicleId(positionHelper->getId());
        newFormationMessage->setExternalId(positionHelper->getExternalId().c_str());
        newFormationMessage->setKind(MANEUVER_TYPE);
        // attributes unique to UpdatePlatoonFormation
        newFormationMessage->setPlatoonSpeed(positionHelper->getPlatoonSpeed());
        newFormationMessage->setPlatoonLane(positionHelper->getPlatoonLane());
        newFormationMessage->setPlatoonFormationArraySize(newPlatoonFormation.size());
        // set the platoon formation entries one by one
        for (int i = 0; i < newPlatoonFormation.size(); i++) {
            newFormationMessage->setPlatoonFormation(i, newPlatoonFormation[i]);
        }
        return newFormationMessage;
    }

    /**
     * Handle the reception of an update platoon formation message
     * @param msg the update platoon formation message received
     */
    void Example7App::handleUpdatePlatoonFormationMessage(UpdatePlatoonFormation* msg) {
        std::vector<int> newFormation;
        // retrieve the new formation from the message
        for (int i = 0; i < msg->getPlatoonFormationArraySize(); i++)
            // Use the getPlatoonFormation() method to index the array
            newFormation.push_back(msg->getPlatoonFormation(i));
        positionHelper->setPlatoonFormation(newFormation);

        // log the event
        std::string formationString = "[ ";
        for (int i = 0; i < newFormation.size(); i++)
            formationString += std::to_string(newFormation[i]) + " ";
        formationString += "]";

        char text[250];
        // Let's see if copilot is worth its salt
        // sprintf(text, "LEADER[%d]: Received new formation<%s> from v<%d>\n", positionHelper->getId(), formationString.c_str(), msg->getVehicleId());
        // Use the sprintf function to format the text
        sprintf(text, "v<%d> got newFormation = %s\n", positionHelper->getId(), formationString.c_str());
        getSimulation()->getActiveEnvir()->alert(text);
    }
}
