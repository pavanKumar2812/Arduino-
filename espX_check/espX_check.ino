#ifdef ESP8266
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#else
  #error "Not using the ESP8266 or ESP32!"
#endif

void setup() 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

void loop() 
{
   int count = WiFi.scanNetworks();
  Serial.print(count);
  Serial.println(" networks found");
  for (int i = 0; i < count; i++) 
  {
    Serial.print(i + 1); 
    Serial.print(": ");
    Serial.print(WiFi.SSID(i)); 
    Serial.print(" (");
    Serial.print(WiFi.RSSI(i)); 
    Serial.println(")");
    delay(10);
  }
  Serial.println("");
  delay(5000); 
}
