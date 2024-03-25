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

// TODO: instrument the scenario to monitor initialization stages
// TODO: try to get the encrypted messages to work

package org.car2x.plexe.scenarios;

import org.car2x.plexe.scenarios.BBaseScenario;

simple SecuredScenario extends BBaseScenario
{
    parameters:
        @display("i=block/app2");
        @class(plexe::SecuredScenario);
}
