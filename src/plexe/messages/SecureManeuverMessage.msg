import ManeuverMessage;

packet SecureManeuverMessage extends ManeuverMessage {
    // The encrypted data
    string encryptedData = "";
    // The length of the encrypted data
    int encryptedDataLength = 0;
    // The signature of the encrypted data, if applicable
    string signature = "";
    // The public key used to encrypt the data, if applicable
    string publicKey = "";
    // The algorithm used to encrypt the data
    // TODO: should this be an enum?
    string algorithm = "plaintext";
}
