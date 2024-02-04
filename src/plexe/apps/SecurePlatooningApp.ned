package org.car2x.plexe.apps;
import org.car2x.plexe.apps.BaseApp;

simple SecurePlatooningApp like BaseApp {
    parameters:
        int headerLength @unit("bit") = default(0 bit);
        // provide the implementation of the application
        @class(plexe::SecurePlatooningApp);
    gates:
        input lowerLayerIn;
        output lowerLayerOut;
        input lowerControlIn;
        output lowerControlOut;
}
