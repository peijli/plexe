import ManeuverMessage;

packet KeyExchangeMessage extends ManeuverMessage
{
    int keyExchangeType;
    int keyExchangeDataLength;
}
