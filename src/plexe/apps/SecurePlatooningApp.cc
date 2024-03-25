#include "plexe/apps/SecurePlatooningApp.h"
#include "plexe/protocols/BaseProtocol.h"
#include "veins/modules/messages/BaseFrame1609_4_m.h"
#include "veins/base/messages/MacPkt_m.h"
#include "veins/modules/mac/ieee80211p/Mac1609_4.h"
#include "veins/base/utils/FindModule.h"
#include "plexe/messages/PlexeInterfaceControlInfo_m.h"
#include "plexe/driver/PlexeRadioDriverInterface.h"
#include "plexe/utilities/CryptoHelper.h"
#include <cassert>

/*
* TODO LIST
* - [x] Implementing sending encrypted messages
* - [x] Implementing receiving and decrypting encrypted messages
*/

using namespace veins;

namespace plexe {
    Define_Module(SecurePlatooningApp);

    void SecurePlatooningApp::initialize(int stage){
        BaseApp::initialize(stage);
        if (stage == 0) {
            // generate encryption key
            symmetricKey = CryptoHelper::generateSymmetricKey();
        } else if (stage == 1) {
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

    void SecurePlatooningApp::handleLowerMsg(cMessage* msg) {
        // getSimulation()->getEnvir()->alert("SecurePlatooningApp::handleLowerMsg");
        veins::BaseFrame1609_4* frame = check_and_cast<veins::BaseFrame1609_4*>(msg);
        cPacket* enc = frame->getEncapsulatedPacket();
        ASSERT2(enc, "received a BaseFrame1609_4s with nothing inside");
        // getSimulation()->getEnvir()->alert("SecurePlatooningApp::handleLowerMsg: received a message");

        // filter the messages based on their kind
        if (enc->getKind() == MANEUVER_TYPE) {
            LOG << "SecurePlatooningApp::handleLowerMsg: received a maneuver message at vehicle " << positionHelper->getId() << std::endl;
            ManeuverMessage* mm = check_and_cast<ManeuverMessage*>(
                frame->decapsulate());
            // we would care about the abandon platoon message or the update platoon formation message
            if (SecureManeuverMessage* smm = dynamic_cast<SecureManeuverMessage*>(mm)) {
                LOG << "SecurePlatooningApp::handleLowerMsg: received a secured maneuver message at vehicle " << positionHelper->getId() << std::endl;
                std::string plaintext = handleSecureManeuverMessage(smm);
                // getSimulation()->getEnvir()->alert(plaintext.c_str());
                // delete msg;
            }
            // delete the frame if it is not needed
            delete frame;
        } 
        else {
            BaseApp::handleLowerMsg(msg);
        } 
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
        // getSimulation()->getEnvir()->alert("SecurePlatooningApp::sendUnicast: sent a message");
    }

    /**
     * Create a secure maneuver message intended for a vehicle in the platoon.
     * @param message the message to be secured
     * @param algorithm the encryption algorithm to be used, default is AES
     * @return a secure maneuver message object
     * @todo Implement asymmetric encryption
     */
    SecureManeuverMessage *SecurePlatooningApp::createSecureManeuverMessage(
            std::string message, std::string algorithm="AES") {
        LOG << "SecurePlatooningApp::createSecureManeuverMessage: creating a secure maneuver message at vehicle " << positionHelper->getId() << std::endl;
        LOG << "SecurePlatooningApp::createSecureManeuverMessage: symmetric key " << symmetricKey << std::endl;
        LOG << "SecurePlatooningApp::createSecureManeuverMessage: message " << message << std::endl;
        SecureManeuverMessage* msg = new SecureManeuverMessage();
        // configure basic maneuver message properties
        msg->setPlatoonId(positionHelper->getPlatoonId());
        msg->setVehicleId(positionHelper->getId());
        msg->setExternalId(positionHelper->getExternalId().c_str());
        msg->setKind(MANEUVER_TYPE);
        // configure the secured message properties
        msg->setAlgorithm(algorithm.c_str());
        if (algorithm == "AES") {
            msg->setEncryptedData(CryptoHelper::symmetricEncrypt(message, symmetricKey).c_str());
        }
        else {
            // asymmetric encryption, dummy implementation for now
            msg->setEncryptedData(CryptoHelper::asymmetricEncrypt(message, symmetricKey).c_str());
            msg->setSignature(CryptoHelper::sign(message, symmetricKey).c_str());
            msg->setPublicKey(symmetricKey.c_str());
        }
        return msg;
    }

    /**
     * Send a secured message to a specific destination vehicle
     * @param message the message to be sent
     * @param destination the ID of the destination vehicle
     */
    void SecurePlatooningApp::sendSecuredMessage(std::string message, int destination) {
        SecureManeuverMessage* msg = createSecureManeuverMessage(message);
        msg->setDestinationId(destination);
        sendUnicast(msg, destination);
        getSimulation()->getEnvir()->alert("SecurePlatooningApp::sendSecuredMessage: sent a secured message");
        // delete msg;
    }

    /**
     * Handle a secure maneuver message received from the MAC layer
     * @param msg the secure maneuver message to be handled
     * @return a string representation of the decrypted message
     */
    std::string SecurePlatooningApp::handleSecureManeuverMessage(SecureManeuverMessage* msg) {
        getSimulation()->getEnvir()->alert("SecurePlatooningApp::handleSecureManeuverMessage");
        // getSimulation()->getEnvir()->alert("SecurePlatooningApp::handleSecureManeuverMessage");
        // ensure that the message is for the right platoon
        if (msg->getPlatoonId() != positionHelper->getPlatoonId())
            return "";
        // ensure that the message is for this vehicle
        if (msg->getDestinationId() != positionHelper->getId())
            return "";
        // get the message properties
        int platoonId = msg->getPlatoonId();
        int vehicleId = msg->getVehicleId();
        std::string externalId = msg->getExternalId();
        std::string algorithm = msg->getAlgorithm();
        std::string encryptedData = msg->getEncryptedData();
        std::string signature = msg->getSignature();
        std::string publicKey = msg->getPublicKey();
        std::string decrypted;
        // decrypt the message
        if (algorithm == "AES") {
            decrypted = CryptoHelper::symmetricDecrypt(encryptedData, symmetricKey);
        }
        else {
            // asymmetric encryption, dummy implementation for now
            decrypted = CryptoHelper::asymmetricDecrypt(encryptedData, symmetricKey);
        }
        // print some information about the message
        LOG << "SecurePlatooningApp::handleSecureManeuverMessage: received a secure maneuver message at vehicle " << positionHelper->getId() << std::endl;
        LOG << "SecurePlatooningApp::handleSecureManeuverMessage: cipher text " << encryptedData << std::endl;
        LOG << "SecurePlatooningApp::handleSecureManeuverMessage: decrypted text " << decrypted << std::endl;
        return decrypted;
    }
} // namespace plexe
