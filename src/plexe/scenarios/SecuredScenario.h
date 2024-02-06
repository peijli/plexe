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

#ifndef SECUREDSCENARIO_H_
#define SECUREDSCENARIO_H_

#include "plexe/scenarios/BaseScenario.h"
// #include "plexe/apps/BaseApp.h"
#include "plexe/apps/SecurePlatooningApp.h"

namespace plexe {

class SecuredScenario : public BaseScenario {
public:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage* msg);
    // destructor
    virtual ~SecuredScenario();

protected:
    // leader average speed
    double leaderSpeed;
    // application layer, used to stop the simulation
    // BaseApp* appl;
    SecurePlatooningApp* appl;
    // triggers leader to send an encrypted message to the last vehicle
    cMessage *encryptedCMessage;

public:
    SecuredScenario()
        : leaderSpeed(0)
        , appl(nullptr){};
};

} // namespace plexe

#endif
