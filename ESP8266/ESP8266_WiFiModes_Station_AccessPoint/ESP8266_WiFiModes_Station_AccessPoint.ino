#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer Server;
uint8_t PinLed = 16;
const char* Ssid = "TechMMakers";
const char* Password = "!nn0vationFactory";

const char* MySsid = "MoonNight";
IPAddress Ip(192,168,11,4);
IPAddress Gateway(192,168,11,1);
IPAddress Subnet(255,255,255,0);

void setup() 
{
  pinMode(PinLed, OUTPUT);
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(Ssid, Password);

  Serial.begin(115200);
  
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());

  Server.on("/", [](){Server.send(200, "text/plain", "Hello World!");});
  Server.on("/Toggle", ToggleLED);
  Server.begin();

  WiFi.softAPConfig(Ip, Gateway, Subnet);
  WiFi.softAP(MySsid, Password);
}

void loop()
{
  Server.handleClient();
}

void ToggleLED()
{
  digitalWrite(PinLed,!digitalRead(PinLed));
  Server.send(204,"");
}
