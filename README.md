
# Plexe - The platooning extension for Veins

> See the Plexe website <http://plexe.car2x.org/> for a tutorial, documentation,
and publications.

Plexe is composed of many parts. See the version control log for a full list of
contributors and modifications. Each part is protected by its own, individual
copyright(s), but can be redistributed and/or modified under an open source
license. License terms are available at the top of each file. Parts that do not
explicitly include license text shall be assumed to be governed by the "GNU
General Public License" as published by the Free Software Foundation -- either
version 2 of the License, or (at your option) any later version
(SPDX-License-Identifier: GPL-2.0-or-later). Parts that are not source code and
do not include license text shall be assumed to allow the Creative Commons
"Attribution-ShareAlike 4.0 International License" as an additional option
(SPDX-License-Identifier: GPL-2.0-or-later OR CC-BY-SA-4.0). Full license texts
are available with the source distribution.

Besides myself (Michele Segata), Plexe has been improved over the years thanks
to some incredibly valuable contributors:

- Bastian Bloessl
- Tobias Hardes
- Julian Heinovski
- Stefan Joerer
- Max Schettler
- Christoph Sommer

## Check out code

1. driver/PlexeRadioDriverInterface.cc
2. Different maneuvers and their data requests
3. messages/PlatooningBeacon
4. plexe/mobility/CommandInterface -- Remember how to work with plexeTraciVehicle

## Files and modules I should modify

1. Implement new message types in plexe/messages
   - Wait a second...
   - How about changing up the actual communication protocol? Instead of re-implementing every single message type...
   - Extend BaseProtocol or SimplePlatooningBeaconing classes to handle message encryption and decryption
2. Extend the GeneralPlatooningApp to handle the new message types
3. Maybe implement some of your encryption/decryption code in the utilities folder

