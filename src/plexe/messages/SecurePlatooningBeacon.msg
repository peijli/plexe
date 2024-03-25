import PlatooningBeacon;

packet SecurePlatooningBeacon extends PlatooningBeacon {
    @customize(true);
    string encryptedData = "";
    int encryptedDataLength = 0;
    string signature = "";
    int signatureLength = 0;
    string publicKey = "";
    int publicKeyLength = 0;
    string algorithm = "plaintext";
}
