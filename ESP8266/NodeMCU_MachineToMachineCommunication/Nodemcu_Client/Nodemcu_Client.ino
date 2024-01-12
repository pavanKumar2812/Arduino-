#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

const char* SSID = "MoonKnight";
const char* Password = "P@v@nKum@r";

IPAddress IP(10, 10, 10, 55);
IPAddress Gateway(10, 10, 10, 1);
IPAddress Subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

void HandleRoot()
{
  server.send(200, "text/html", "MessageFromClient");
}

void setup()
{
  server.begin();
  server.on("/", HandleRoot);
  Serial.begin(115200);
  WiFi.config(IP, Gateway, Subnet);
  for(uint8_t t = 4; t > 0; t--)
  {
    Serial.printf("SETUP WAIT ...\n", t);
    Serial.flush();
    delay(1000);
  }
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  WiFi.begin(SSID, Password);
  if(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  pinMode(16, OUTPUT);

}

void loop()
{
  server.handleClient();

  // Wait for WiFi connection
  if(WiFi.status() == WL_CONNECTED)
  {
    WiFiClient client; // Create a WiFiClient object
    HTTPClient http;

    if(digitalRead(16) == LOW)
    {
      Serial.print("[HTTP] begin...\n");
      http.begin(client, "http://10.10.10.1/Ambulance_A");
      Serial.println("http://10.10.10.1/Ambulance_A");
      Serial.print("[HTTP] GET...\n");
      int httpCode = http.GET();
      http.end();
      digitalWrite(16, HIGH); // Turn on the built-in LED
      delay(2000); // Wait for 2 seconds
    }
    else
    {
      http.begin(client, "http://10.10.10.1/Ambulance_D");
      Serial.println("http://10.10.10.1/Ambulance_D");
      int httpCode = http.GET();
      http.end();
      digitalWrite(16, LOW); // Turn off the built-in LED
      delay(2000); // Wait for 2 seconds
    }
  }
}