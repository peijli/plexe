#include <map>
#include <cassert>
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
// - [x] Test encryption and decryption on sample payload. 
// - [x] Re-implement the encryption functionalities in this class
// - [ ] Test this class again for performance
// - [ ] Go through literature and see how OMNet++ handles network capabilities and different packet sizes

namespace plexe {

Define_Module(SecurePlatooningBeaconing)

void SecurePlatooningBeaconing::initialize(int stage)
{
    SimplePlatooningBeaconing::initialize(stage);
    if (stage == 0) {
        // generate encryption key
        symmetricKey = CryptoHelper::generateSymmetricKey();
    }
    // if (stage == 1) {
    //     // print some useful information regarding the method and the vehicle
    //     std::string msg = "SecurePlatooningBeaconing::initialize at vehicle " + std::to_string(positionHelper->getId());
    //     getSimulation()->getEnvir()->alert(msg.c_str());
    // }
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
    LOG << "Data length: " << dataLength << endl;
    LOG << "Payload: " << json << endl;
    char* encrypted = CryptoHelper::symmetricEncrypt(json.c_str(), dataLength, symmetricKey);
    // set the properties of the secure beacon
    // secureBeacon->setEncryptedData(encrypted);
    // Use the plaintext for testing
    secureBeacon->setEncryptedData(json.c_str(), dataLength);
    secureBeacon->setAlgorithm("AES");
    secureBeacon->setKind(BEACON_TYPE);
    secureBeacon->setVehicleId(beacon->getVehicleId());
    // DO THESE EVEN WORK?
    auto newByteLength = beacon->getByteLength() + dataLength;
    secureBeacon->setByteLength(newByteLength);
    secureBeacon->setSequenceNumber(beacon->getSequenceNumber());
    // getSimulation()->getEnvir()->alert("SecurePlatooningBeaconing::encryptBeacon");
    // getSimulation()->getEnvir()->alert(CryptoHelper::symmetricDecrypt(secureBeacon->getEncryptedData(), secureBeacon->getEncryptedDataLength(), symmetricKey));
    // getSimulation()->getEnvir()->alert("SecurePlatooningBeaconing::encryptBeacon end");
    return secureBeacon;
}

PlatooningBeacon* SecurePlatooningBeaconing::decryptBeacon(const SecurePlatooningBeacon* secureBeacon) {
    // getSimulation()->getEnvir()->alert("SecurePlatooningBeaconing::decryptBeacon");
    // decrypt the beacon
    // char* decryptedChar = CryptoHelper::symmetricDecrypt(
    //     secureBeacon->getEncryptedData(), 
    //     secureBeacon->getEncryptedDataLength(), 
    //     symmetricKey);
    // getSimulation()->getEnvir()->alert("SecurePlatooningBeaconing::decryptBeacon end");
    // getSimulation()->getEnvir()->alert(decryptedChar);
    const char* decryptedChar = secureBeacon->getEncryptedData();
    int decryptedLength = secureBeacon->getEncryptedDataLength();
    std::string decrypted(decryptedChar, decryptedLength);
    // convert the JSON string to a map
    getSimulation()->getEnvir()->alert(decrypted.c_str());
    getSimulation()->getEnvir()->alert(decryptedChar);
    std::map<std::string, std::string> map = JSONParser::parse(decrypted);
    // create a PlatooningBeacon object
    PlatooningBeacon* beacon = new PlatooningBeacon();
    // set the properties of the beacon
    // getSimulation()->getEnvir()->alert("SecurePlatooningBeaconing::decryptBeacon STOI");
    beacon->setVehicleId(std::stoi(map["vehicleId"]));
    beacon->setControllerAcceleration(std::stod(map["controllerAcceleration"]));
    beacon->setAcceleration(std::stod(map["acceleration"]));
    beacon->setSpeed(std::stod(map["speed"]));
    beacon->setPositionX(std::stod(map["positionX"]));
    beacon->setPositionY(std::stod(map["positionY"]));
    beacon->setTime(std::stod(map["time"]));
    beacon->setLength(std::stod(map["length"]));
    beacon->setSpeedX(std::stod(map["speedX"]));
    beacon->setSpeedY(std::stod(map["speedY"]));
    beacon->setAngle(std::stod(map["angle"]));
    beacon->setSequenceNumber(secureBeacon->getSequenceNumber());
    beacon->setKind(BEACON_TYPE);
    beacon->setByteLength(secureBeacon->getByteLength());
    // return the beacon
    return beacon;
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
        PlatooningBeacon* beacon = decryptBeacon(secureBeacon);
        // handle the beacon as a normal PlatooningBeacon
        BaseProtocol::handleLowerPlatooningBeacon(beacon, frame);
        // dropAndDelete(secureBeacon);
        if (!frame) return;
    } else if (PlatooningBeacon * beacon = dynamic_cast<PlatooningBeacon*>(enc)) {
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


} // namespace plexe
