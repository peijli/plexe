package org.car2x.plexe.apps;
import org.car2x.plexe.apps.BaseApp;
import org.car2x.plexe.apps.GeneralPlatooningApp;

simple SecurePlatooningApp extends GeneralPlatooningApp like BaseApp {
    parameters:
        // int headerLength @unit("bit") = default(0 bit);
        // provide the implementation of the application
        @class(plexe::SecurePlatooningApp);
    // gates:
    //     input lowerLayerIn;
    //     output lowerLayerOut;
    //     input lowerControlIn;
    //     output lowerControlOut;
}
