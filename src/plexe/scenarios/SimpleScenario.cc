//
// Copyright (C) 2018-2023 Julian Heinovski <julian.heinovski@ccs-labs.org>
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

#include "plexe/scenarios/SimpleScenario.h"
#include "plexe/apps/Example7App.h"

using namespace veins;

namespace plexe {

Define_Module(SimpleScenario);

void SimpleScenario::initialize(int stage)
{

    BaseScenario::initialize(stage);

    if (stage == 0)
        // get pointer to application
        appl = FindModule<Example7App*>::findSubModule(getParentModule());

    if (stage == 2) {
        // average speed, convert from km/h to m/s
        leaderSpeed = par("leaderSpeed").doubleValue() / 3.6;

        if (positionHelper->isLeader()) {
            // set base cruising speed for the leader
            plexeTraciVehicle->setCruiseControlDesiredSpeed(leaderSpeed);
        }
        else {
            // let the follower set a higher desired speed to stay connected
            // to the leader when it is accelerating
            plexeTraciVehicle->setCruiseControlDesiredSpeed(leaderSpeed + 10);

            // get the platoon formation as a vector
            std::vector<int> formation = positionHelper->getPlatoonFormation();
            // if we are the last vehicle in the platoon...
            if (positionHelper->getId() == formation[formation.size() - 1]) {
                // generate new console messages
                startBraking = new cMessage("startBraking");
                checkDistance = new cMessage("checkDistance");
                // schedule brake operation
                scheduleAt(10, startBraking);
            }
        }
    }
}

void SimpleScenario::handleMessage(cMessage* msg)
{
    if (msg == startBraking) {
        // Increase CACC Constant Spacing to 15m
        plexeTraciVehicle->setCACCConstantSpacing(15);
        // change the color of the traci vehicle
        traciVehicle->setColor(TraCIColor(100,100,100,255));
        // start checking distance
        scheduleAt(simTime() + 0.1, checkDistance);
    } else if (msg == checkDistance) {
        // initialize distance and relative speed to the vehicle in front
        double distance, relativeSpeed;
        plexeTraciVehicle->getRadarMeasurements(distance, relativeSpeed);
        LOG << "Distance to the vehicle in front: " << distance << "m" << endl;
        LOG << "Relative speed to the vehicle in front: " << relativeSpeed << "m/s" << endl;

        // if the distance is greater than 14.9m, switch to ACC
        if (distance > 14.9) {
            plexeTraciVehicle->setActiveController(ACC);
            plexeTraciVehicle->setACCHeadwayTime(1.5);
            traciVehicle->setColor(TraCIColor(255,0,0,255));
            LOG << "Switching to ACC" << endl;
            // send abandon message to the leader
            appl->sendAbandonMessage();
        } else {
            // check again in 0.1s
            scheduleAt(simTime() + 0.1, checkDistance);
        }
    }
}

// define destructor
SimpleScenario::~SimpleScenario()
{
    cancelAndDelete(startBraking);
    cancelAndDelete(checkDistance);
}
} // namespace plexe
