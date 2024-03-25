package org.car2x.plexe.protocols;

import org.car2x.plexe.protocols.SimplePlatooningBeaconing;

//
// Define static beaconing for platooning
//
simple SecurePlatooningBeaconing extends SimplePlatooningBeaconing
{
    parameters:
        @display("i=block/network2");
        @class(plexe::SecurePlatooningBeaconing);
        int symmetricKeyLength = 16; // length of the symmetric key in bytes
        int symmetricKey = 0; // symmetric key used for encryption
}
