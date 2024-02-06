//
// Copyright (C) 2024 Peijing Li <peijli@umich.edu>
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "plexe/scenarios/SecuredScenario.h"

using namespace veins;

namespace plexe {

    Define_Module(SecuredScenario);

    void SecuredScenario::initialize(int stage) {

        BaseScenario::initialize(stage);

        if (stage == 0){
            // get pointer to application
            appl = FindModule<SecurePlatooningApp*>::findSubModule(getParentModule());
        } else if (stage == 2) {
            // average speed, convert from km/h to m/s
            leaderSpeed = par("leaderSpeed").doubleValue() / 3.6;
            // get the platoon formation as a vector
            std::vector<int> formation = positionHelper->getPlatoonFormation();
            if (positionHelper->isLeader()) {
                // set base cruising speed for the leader
                plexeTraciVehicle->setCruiseControlDesiredSpeed(leaderSpeed);
                // send out an encrypted message to the follower
                encryptedCMessage = new cMessage("encryptedCMessage");
                scheduleAt(simTime() + 5, encryptedCMessage);
                LOG << "Scheduled an encrypted message to be sent out at " << simTime() + 5 << "s" << std::endl;
            } else {
                // let the follower set a higher desired speed to stay connected
                // to the leader when it is accelerating
                plexeTraciVehicle->setCruiseControlDesiredSpeed(leaderSpeed + 10);
            }
        }
    }

    void SecuredScenario::handleMessage(cMessage* msg) {
        getSimulation()->getEnvir()->alert("SecuredScenario::handleMessage: received a message");
        if (msg == encryptedCMessage) {
            LOG << "Sending an encrypted message to the last vehicle" << std::endl;
            // formulate the message that contains some information about the sender vehicle
            // get the platoon formation as a vector
            std::vector<int> formation = positionHelper->getPlatoonFormation();
            std::string message = "This is an encrypted message from vehicle " + std::to_string(positionHelper->getId());
            appl->sendSecuredMessage(message, formation[formation.size() - 1]);
        }
    }

    // define destructor
    SecuredScenario::~SecuredScenario() {
        cancelAndDelete(encryptedCMessage);
        appl->deleteModule(); // THIS IS WHAT I WAS MISSING!
    }
    //void SecuredScenario::finish() {
    //    cancelAndDelete(encryptedCMessage);
    //}
 
} // namespace plexe
