import ManeuverMessage;

// Message used to abandon the platoon.
// It is most of the time recommended to define new packets as extension to Maneuver Messages, since information such as vehicleId, platoonId and destinationId are necessary for the correct processing of packets… in 99% of the conceivable scenarios.
// Assigning to packets that share a similar purpose (in the present case, messages supporting some coordinated maneuver) a unique ‘TYPE’ is a good practice to enable easy filtering of packets at reception time. 
// Do not forget that, in Vehicular networks, transmissions are most of the time broadcast, so filtering packets can become extremely important.
packet AbandonPlatoon extends ManeuverMessage {
    string msgContent = "AbandonPlatoon";
}
