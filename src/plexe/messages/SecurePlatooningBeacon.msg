import PlatooningBeacon;

packet SecurePlatooningBeacon extends PlatooningBeacon {
    string encryptedData = "";
    int encryptedDataLength = 0;
    string signature = "";
    int signatureLength = 0;
    string publicKey = "";
    string algorithm = "plaintext";
}
