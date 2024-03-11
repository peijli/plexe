import PlatooningBeacon;

packet SecurePlatooningBeacon extends PlatooningBeacon {
    @customize(true);
    string encryptedData = "";
    int encryptedDataLength = 0;
    string mac = "";
    int macLength = 0;
    string algorithm = "plaintext";
    int destinationId = 0;
}
