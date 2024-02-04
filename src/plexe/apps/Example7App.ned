package org.car2x.plexe.apps;
import org.car2x.plexe.apps.BaseApp;

// Essentially, in Vehicular Networks, applications usually run on top of a minimal MAC+PHY networking stack, 
// leading to a typical APP-MAC-PHY 3-layers architecture which is simpler compared to a full, classic TCP/IP one. 
simple Example7App like BaseApp {
    parameters:
        int headerLength @unit("bit") = default(0 bit);
        // provide the implementation of the application
        @class(plexe::Example7App);
        // @namespace(plexe);
    gates:
        input lowerLayerIn;
        output lowerLayerOut;
        input lowerControlIn;
        output lowerControlOut;
}
