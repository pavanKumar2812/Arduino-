#include <SoftwareSerial.h>
#include <PN532_SWHSU.h>
#include <PN532.h>
#include <NfcAdapter.h>

SoftwareSerial SWSerial(2, 3); // RX, TX
PN532_SWHSU pn532swhsu(SWSerial);
//PN532 nfc(pn532swhsu);

NfcAdapter nfc = NfcAdapter(pn532swhsu);

void setup(void)
{
    Serial.begin(9600);
    Serial.println("Hello Maker!");
    delay(100);
    nfc.begin();
}

void loop()
{
    if(nfc.tagPresent())
    {
        NfcTag tag = nfc.read();
        // tag.print();

        NdefMessage message = tag.getNdefMessage();
        NdefRecord record = message.getRecord(0);
        byte length = record.getPayloadLength();
        byte payload[length];

        record.getPayload(payload);

        String payloadString = String((char *)payload);
        payloadString = payloadString.substring(3,payloadString.length()-2);
        Serial.println(payloadString);
        }
    delay(500);
  }