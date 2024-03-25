#include <map>
#include <cassert>
#include <limits>
#include "SecurePlatooningBeaconing.h"
#include "plexe/utilities/CryptoHelper.h"
#include "plexe/utilities/JSONParser.h"

#include "veins/modules/mac/ieee80211p/Mac1609_4.h"
#include "veins/base/utils/FindModule.h"
#include "veins/modules/messages/BaseFrame1609_4_m.h"

#include "plexe/PlexeManager.h"
#include "plexe/driver/Veins11pRadioDriver.h"
#include "plexe/messages/PlexeInterfaceControlInfo_m.h"


using namespace veins;

// I think I have to overwrite a bunch of BaseApp methods...

// TODO LIST
// - [ ] Create different keys for each vehicle
// - [ ] Implement symmetric key exchange between leader and followers

namespace plexe {

Define_Module(SecurePlatooningBeaconing)

void SecurePlatooningBeaconing::initialize(int stage)
{
    SimplePlatooningBeaconing::initialize(stage);
    if (stage == 0) {
        // generate encryption key
        fallbackKey = CryptoHelper::generateSymmetricKey(false);
        privateKey = CryptoHelper::generateSymmetricKey(true);
        // initialize symmetric key to the maximum value
        symmetricKey = std::numeric_limits<std::uint32_t>::max();
    }
    if (stage == 1) {
        std::string message = "Vehicle " + std::to_string(positionHelper->getId()) + " has symmetric key " + std::to_string(privateKey);
        getSimulation()->getEnvir()->alert(message.c_str());
        // broadcast the key to all followers if the vehicle is a leader
        int myId = positionHelper->getId();
        if (positionHelper->isLeader()) {
            // send the key to all followers
            for (int i = 0; i < positionHelper->getPlatoonSize(); i++) {
                int followerID = positionHelper->getMemberId(i);
                if (followerID != myId) {
                    KeyExchangeMessage* kxm = createKeyExchangeMessage(followerID);
                    sendUnicast(kxm, followerID);
                }
            }
            // initialize the shared key store
            for (int i = 0; i < positionHelper->getPlatoonSize(); i++) {
                sharedKeyStore.setSharedKey(positionHelper->getMemberId(i), std::numeric_limits<std::uint32_t>::max());
            }
        }
    }
}

void SecurePlatooningBeaconing::sendUnicast(cPacket* msg, int destination) {
    // create and send beacon
    auto wsm = veins::make_unique<BaseFrame1609_4>("", BEACON_TYPE);
    wsm->setRecipientAddress(LAddress::L2BROADCAST());
    wsm->setChannelNumber(static_cast<int>(Channel::cch));
    wsm->setUserPriority(priority);
    wsm->encapsulate(msg);
    sendTo(wsm.release(), PlexeRadioInterfaces::VEINS_11P);
}

KeyExchangeMessage* SecurePlatooningBeaconing::createKeyExchangeMessage(int destinationAddress) {
    KeyExchangeMessage *kxm = new KeyExchangeMessage();
    // configure basic properties of the message
    kxm->setPlatoonId(positionHelper->getPlatoonId());
    kxm->setVehicleId(positionHelper->getId());
    kxm->setExternalId(positionHelper->getExternalId().c_str());
    kxm->setDestinationId(destinationAddress);
    kxm->setKind(MANEUVER_TYPE);
    // configure key exchange properties
    kxm->setKeyExchangePayload(CryptoHelper::computeSharedKey(privateKey));
    return kxm;
}

void SecurePlatooningBeaconing::handleSelfMsg(cMessage* msg)
{
    SimplePlatooningBeaconing::handleSelfMsg(msg);
}

SecurePlatooningBeaconing::SecurePlatooningBeaconing()
{
}

SecurePlatooningBeaconing::~SecurePlatooningBeaconing()
{
}

std::unique_ptr<BaseFrame1609_4> SecurePlatooningBeaconing::createBeacon(int destinationAddress) {
    // vehicle's data to be included in the message
    VEHICLE_DATA data;
    // get information about the vehicle via traci
    plexeTraciVehicle->getVehicleData(&data);

    // create and send beacon
    auto wsm = veins::make_unique<BaseFrame1609_4>("", BEACON_TYPE);
    wsm->setRecipientAddress(LAddress::L2BROADCAST());
    wsm->setChannelNumber(static_cast<int>(Channel::cch));
    wsm->setUserPriority(priority);

    // create platooning beacon with data about the car
    PlatooningBeacon* pkt = new PlatooningBeacon();
    pkt->setVehicleId(myId);
    pkt->setControllerAcceleration(data.u);
    pkt->setAcceleration(data.acceleration);
    pkt->setSpeed(data.speed);
    pkt->setPositionX(data.positionX);
    pkt->setPositionY(data.positionY);
    pkt->setTime(data.time);
    pkt->setLength(length);
    pkt->setSpeedX(data.speedX);
    pkt->setSpeedY(data.speedY);
    pkt->setAngle(data.angle);

    // information not specific to the vehicle
    auto nextSequenceNumber = seq_n++;
    pkt->setKind(BEACON_TYPE);
    // TODO: what is this packetSize?
    pkt->setByteLength(packetSize);
    pkt->setSequenceNumber(nextSequenceNumber);

    SecurePlatooningBeacon* securePkt = encryptBeacon(pkt);

    wsm->encapsulate(securePkt);
    delete pkt;
    // wsm->encapsulate(pkt);
    // delete securePkt;
    return wsm;
}

SecurePlatooningBeacon* SecurePlatooningBeaconing::encryptBeacon(const PlatooningBeacon* beacon) {
    // create a SecurePlatooningBeacon object
    SecurePlatooningBeacon* secureBeacon = new SecurePlatooningBeacon();

    // convert the beacon to a map object
    std::map<std::string, std::string> map;
    map["vehicleId"] = std::to_string(beacon->getVehicleId());
    map["controllerAcceleration"] = std::to_string(beacon->getControllerAcceleration());
    map["acceleration"] = std::to_string(beacon->getAcceleration());
    map["speed"] = std::to_string(beacon->getSpeed());
    map["positionX"] = std::to_string(beacon->getPositionX());
    map["positionY"] = std::to_string(beacon->getPositionY());
    map["time"] = std::to_string(beacon->getTime());
    map["length"] = std::to_string(beacon->getLength());
    map["speedX"] = std::to_string(beacon->getSpeedX());
    map["speedY"] = std::to_string(beacon->getSpeedY());
    map["angle"] = std::to_string(beacon->getAngle());
    // convert the map to a JSON string
    std::string json = JSONParser::stringify(map);
    // encrypt the JSON string
    int dataLength = static_cast<int>(json.length());
    LOG << "Message with sequence number " << beacon->getSequenceNumber() << " from vehicle " << myId << " to vehicle " << beacon->getVehicleId() << " is being encrypted." << endl;
    LOG << "Payload: " << json << endl;

    uint32_t keyToUse;
    // select the right key!
    if (positionHelper->isLeader()) {
        keyToUse = sharedKeyStore.getSharedKey(beacon->getVehicleId());
    } else {
        keyToUse = symmetricKey;
    }
    // check if we need to use the fallback key
    if (keyToUse == std::numeric_limits<std::uint32_t>::max()) {
        keyToUse = fallbackKey;
        LOG << "Using a fallback key" << endl;
    }
    LOG << "Using key " << keyToUse << " for encryption" << endl;
    char * encrypted = CryptoHelper::symmetricEncrypt(
        json.c_str(), dataLength, keyToUse);

    LOG << "Encrypted message: " << encrypted << endl;
    secureBeacon->setEncryptedData(encrypted, dataLength);
    secureBeacon->setAlgorithm("AES");

    // set the properties of the secure beacon
    secureBeacon->setKind(BEACON_TYPE);
    secureBeacon->setVehicleId(beacon->getVehicleId());
    auto newByteLength = beacon->getByteLength() + dataLength;
    secureBeacon->setByteLength(newByteLength);
    secureBeacon->setSequenceNumber(beacon->getSequenceNumber());
    return secureBeacon;
}

PlatooningBeacon *SecurePlatooningBeaconing::handleSecurePlatooningBeacon(SecurePlatooningBeacon* secureBeacon) {
    // if (secureBeacon->getDestinationId() != myId) {
    //     return nullptr;
    // }
    LOG << "Message with sequence number " << secureBeacon->getSequenceNumber() << " from vehicle " << secureBeacon->getVehicleId() << " to vehicle " << myId << " is being decrypted." << endl;
    LOG << "Encrypted message: " << secureBeacon->getEncryptedData() << endl;
    // decrypt the beacon
    std::map<std::string, std::string> * map = decryptBeacon(secureBeacon);
    // create a PlatooningBeacon object
    PlatooningBeacon* beacon = new PlatooningBeacon();
    // set the properties of the beacon
    try {
        beacon->setVehicleId(std::stoi(map->at("vehicleId")));
        beacon->setControllerAcceleration(std::stod(map->at("controllerAcceleration")));
        beacon->setAcceleration(std::stod(map->at("acceleration")));
        beacon->setSpeed(std::stod(map->at("speed")));
        beacon->setPositionX(std::stod(map->at("positionX")));
        beacon->setPositionY(std::stod(map->at("positionY")));
        beacon->setTime(std::stod(map->at("time")));
        beacon->setLength(std::stod(map->at("length")));
        beacon->setSpeedX(std::stod(map->at("speedX")));
        beacon->setSpeedY(std::stod(map->at("speedY")));
        beacon->setAngle(std::stod(map->at("angle")));
        beacon->setSequenceNumber(secureBeacon->getSequenceNumber());
        beacon->setKind(BEACON_TYPE);
        beacon->setByteLength(secureBeacon->getByteLength());
    }

    catch (...) {
        getSimulation()->getEnvir()->alert("Error parsing decrypted map");
        LOG << "Falling back to using own vehicle data" << endl;
        // vehicle's data to be included in the message
        VEHICLE_DATA data;
        // get information about the vehicle via traci
        plexeTraciVehicle->getVehicleData(&data);
        beacon->setVehicleId(myId);
        beacon->setControllerAcceleration(data.u);
        beacon->setAcceleration(data.acceleration);
        beacon->setSpeed(data.speed);
        beacon->setPositionX(data.positionX);
        beacon->setPositionY(data.positionY);
        beacon->setTime(data.time);
        beacon->setLength(length);
        beacon->setSpeedX(data.speedX);
        beacon->setSpeedY(data.speedY);
        beacon->setAngle(data.angle);
        beacon->setSequenceNumber(secureBeacon->getSequenceNumber());
        beacon->setKind(BEACON_TYPE);
        beacon->setByteLength(secureBeacon->getByteLength());
    }
    
    // return the beacon
    delete map;
    return beacon;
}


std::map<std::string, std::string>* SecurePlatooningBeaconing::decryptBeacon(const SecurePlatooningBeacon* secureBeacon) {
    const char* decryptedChar = new char[secureBeacon->getEncryptedDataLength()];
    std::string decrypted = "";
    std::string algorithm = secureBeacon->getAlgorithm();
    // if (algorithm == "AES") {
    //     decryptedChar = CryptoHelper::symmetricDecrypt(
    //         secureBeacon->getEncryptedData(), 
    //         secureBeacon->getEncryptedDataLength(), 
    //         fallbackKey);
    // } else { // assume plaintext
    //     decryptedChar = secureBeacon->getEncryptedData();
    // }

    auto ciphertext = secureBeacon->getEncryptedData();
    auto size = secureBeacon->getEncryptedDataLength();

    // try to parse the decrypted message
    std::map<std::string, std::string>* map = new std::map<std::string, std::string>();

    // select the right key!
    uint32_t keyToUse;
    if (positionHelper->isLeader()) {
        keyToUse = sharedKeyStore.getSharedKey(secureBeacon->getVehicleId());
    } else {
        keyToUse = symmetricKey;
    }

    // check if we need to use the fallback key
    if (keyToUse == std::numeric_limits<std::uint32_t>::max()) {
        keyToUse = fallbackKey;
        LOG << "Using a fallback key" << endl;
    }

    try {
        decryptedChar = CryptoHelper::symmetricDecrypt(ciphertext, size, keyToUse);
        decrypted = std::string(decryptedChar, secureBeacon->getEncryptedDataLength());
        // convert the JSON string to a map
        LOG << "Decrypted message: " << decrypted << endl;
        *map = JSONParser::parse(decrypted);
    } 

    catch (...) {
        getSimulation()->getEnvir()->alert("Error parsing decrypted message");
        LOG << "Error parsing decrypted message" << endl;
        LOG << "Using fallback key" << endl;
        decryptedChar = CryptoHelper::symmetricDecrypt(ciphertext, size, fallbackKey);
        decrypted = std::string(decryptedChar, secureBeacon->getEncryptedDataLength());
        // convert the JSON string to a map
        try {
            *map = JSONParser::parse(decrypted);
        } catch (...) {
            getSimulation()->getEnvir()->alert("Error parsing decrypted message*2");
            LOG << "Error parsing decrypted message" << endl;
        }
    }
    return map;
}

void SecurePlatooningBeaconing::handleLowerMsg(cMessage* msg) {
    // sanity checks
    BaseFrame1609_4* frame = check_and_cast<BaseFrame1609_4*>(msg);
    ASSERT2(frame, "received a frame not of type BaseFrame1609_4");

    cPacket* enc = frame->getEncapsulatedPacket();
    ASSERT2(enc, "received a BaseFrame1609_4 with nothing inside");

    // check if packet is the SecurePlatooningBeacon type
    if (SecurePlatooningBeacon * secureBeacon = dynamic_cast<SecurePlatooningBeacon*>(enc)) {
        // decrypt the beacon
        PlatooningBeacon* beacon = handleSecurePlatooningBeacon(secureBeacon);
        // handle the beacon as a normal PlatooningBeacon
        if (beacon != nullptr) {
            BaseProtocol::handleLowerPlatooningBeacon(beacon, frame);
        }
        // dropAndDelete(secureBeacon);
        if (!frame) return;
    } else if (KeyExchangeMessage * beacon = dynamic_cast<KeyExchangeMessage*>(enc)) {
        // handle the key exchange message
        handleKeyExchangeMessage(beacon);
    }
    else if (PlatooningBeacon * beacon = dynamic_cast<PlatooningBeacon*>(enc)) {
        // handle the beacon as a normal PlatooningBeacon
        BaseProtocol::handleLowerPlatooningBeacon(beacon, frame);
        if (!frame) return;
    } 

    // find the application responsible for this beacon
    ApplicationMap::iterator app = apps.find(frame->getKind());
    if (app != apps.end() && app->second.size() != 0) {
        AppList applications = app->second;
        for (AppList::iterator i = applications.begin(); i != applications.end(); i++) {
            // send the message to the applications responsible for it
            send(frame->dup(), std::get<1>(*i));
        }
    }
    delete frame;
}

void SecurePlatooningBeaconing::handleKeyExchangeMessage(KeyExchangeMessage* msg) {
    // discard the message if the vehicle is not the destination
    if (msg->getDestinationId() != positionHelper->getId()) {
        return;
    }

    // compute the shared secret
    std::uint32_t sharedSecret = CryptoHelper::computeSharedSecret(privateKey, msg->getKeyExchangePayload());
    // next steps depend on whether the vehicle is a leader or a follower
    if (positionHelper->isLeader()) {
        // store the shared key in the shared key store
        sharedKeyStore.setSharedKey(msg->getVehicleId(), sharedSecret);
        std::string message = "Leader stored shared secret with vehicle " + std::to_string(msg->getVehicleId()) + " as " + std::to_string(sharedSecret);
        getSimulation()->getEnvir()->alert(message.c_str());
        // FOR DEBUGGING ONLY
        // symmetricKey = fallbackKey;
        // sharedKeyStore.setSharedKey(msg->getVehicleId(), fallbackKey);
    } else {
        symmetricKey = sharedSecret;
        std::string message = "Follower " + std::to_string(positionHelper->getId()) + " stored shared secret with leader as " + std::to_string(sharedSecret);
        getSimulation()->getEnvir()->alert(message.c_str());
        // send a key exchange message to the leader
        KeyExchangeMessage* kxm = createKeyExchangeMessage(positionHelper->getLeaderId());
        sendUnicast(kxm, positionHelper->getLeaderId());
        // FOR DEBUGGING ONLY
        // symmetricKey = fallbackKey;
    }
}


} // namespace plexe
