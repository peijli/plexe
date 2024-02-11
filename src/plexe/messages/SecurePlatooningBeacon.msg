import PlatooningBeacon;

packet SecurePlatooningBeacon extends PlatooningBeacon {
    string encryptedData = "";
    string signature = "";
    string publicKey = "";
    string algorithm = "plaintext";
}
