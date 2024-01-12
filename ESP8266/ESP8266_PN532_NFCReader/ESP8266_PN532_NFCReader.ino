#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
String tagId = "None";
byte nuidPICC[4];

void setup() 
{
  Serial.begin(115200);        
  Serial.println('\n');
  Serial.println("System initialized");

  nfc.begin();

}

void loop() 
{

  readNFC();
  
}

void readNFC() 
{
  if (nfc.tagPresent())
  {
    NfcTag tag = nfc.read();
    tag.print();
    tagId = tag.getUidString();
    Serial.println("Tag id");
    Serial.println(tagId);
  }
  delay(1000);
}