#include "plexe/apps/Example7App.h"
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
        findHost()->subscribe(Mac1609_4::sigRetriesExceeded, this);
        scenario = FindModule<BaseScenario*>::findSubModule(getParentModule());
}

void Example7App::sendAbandonMessage() {
    // Create environment alert
    // TODO: THIS IS HOW TO DISPLAY TEXT IN OMNET++ SIMULATION
    getSimulation()->getActiveEnvir()->alert("Abandoning platoon");
    AbandonPlatoon* abandonMessage = createAbandonMessage();
    sendUnicast(abandonMessage, abandonMessage->getPlatoonId());
}

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

void Example7App::sendUnicast(cPacket* msg, int destination) {
    Enter_Method_Silent();
    take(msg);
    // encapsulate the message in a BaseFrame1609_4 and send it to the MAC layer
    BaseFrame1609_4* frame = new BaseFrame1609_4(
        "BaseFrame1609_4", msg->getKind());
    frame->setRecipientAddress(destination);
    // prepare the frequency channel for the message
    frame->setChannelNumber(static_cast<int>(Channel::CCH));
    frame->encapsulate(msg);
    PlexeInterfaceControlInfo* controlInfo = new PlexeInterfaceControlInfo();
    controlInfo->setInterfaceId(PlexeRadioInterfaces::VEINS_11P);
    frame->setControlInfo(controlInfo);
    // push the frame down to the MAC layer
    sendDown(frame);
}

void Example7App::handleLowerMsg(cMessage* msg) {
    BaseFrame1609_4* frame = check_and_cast<BaseFrame1609_4*>(msg);
    cPacket* enc = frame->getEncapsulatedPacket();
    ASSERT2(enc, "received a BaseFrame1609_4s with nothing inside");

    // filter the messages based on their kind
    if (enc->getKind() == MANEUVER_TYPE) {
        ManeuverMessage* mm = check_and_cast<ManeuverMessage*>(
            frame->decapsulate());
        if (AbandonPlatoon* msg = dynamic_cast<AbandonPlatoon*>(mm)) {
            handleAbandonPlatoon(msg);
            delete msg;
        } else if (NewFormation* msg = dynamic_cast<NewFormation*>(mm)) {
            handleNewFormation(msg);
            delete msg;
        }
        delete frame;
    } else // dispatch the message to the parent class
        BaseApp::handleLowerMsg(msg);
}


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
    sendNewFormationMessage(formation);
}

void Example7App::sendNewFormationMessage(const std::vector<int>& newPlatoonFormation) {
    NewFormation* newFormationMessage = createNewFormationMessage(newPlatoonFormation);
    for (int i = 0; i < newPlatoonFormation.size(); i++) {
        dest = newPlatoonFormation[i];
        NewFormation* newFormationMessage = newFormationMessage->dup();
        newFormationMessage->setDestinationId(dest);
        sendUnicast(newFormationMessage, dest);
    }
    delete newFormationMessage;
}

NewFormation* Example7App::createNewFormationMessage(const std::vector<int>& newPlatoonFormation) {
    NewFormation* newFormationMessage = new NewFormation();
    newFormationMessage->setPlatoonId(platoonId);
    newFormationMessage->setFormationArraySize(newPlatoonFormation.size());
    for (int i = 0; i < newPlatoonFormation.size(); i++) {
        newFormationMessage->setFormation(i, newPlatoonFormation[i]);
    }
    return newFormationMessage;
}

void Example7App::handleNewFormationMessage(NewFormation* msg) {
    std::vector<int> newFormation;
    // retrieve the new formation from the message
    for (int i = 0; i < msg->getFormationArraySize(); i++) {
        newFormation.push_back(msg->getFormation(i));
    }
    positionHelper->setPlatoonFormation(newFormation);

    // log the event
    std::string formationString = "[ ";
    for (int i = 0; i < newFormation.size(); i++) {
        formationString += std::to_string(newFormation[i]) + " ";
    }
    formationString += "]";

    char text[250];
    // Let's see if copilot is worth its salt
    sprintf(text, "LEADER[%d]: Received new formation<%s> from v<%d>\n", positionHelper->getId(), formationString.c_str(), msg->getVehicleId());
    // sprintf(text, "v<%d> got newFormation = %s\n", positionHelper->getId(), formationString.c_str());
    getSimulation()->getActiveEnvir()->alert(text);
    
}

