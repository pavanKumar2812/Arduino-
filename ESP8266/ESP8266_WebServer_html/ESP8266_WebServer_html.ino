#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer Server;
uint8_t PinLed = 16;
const char* Ssid = "TechMMakers";
const char* Password = "!nn0vationFactory";

const char webpage[] PROGMEM = R"=====(
<html>
<head>
</head>
<body>
<form action="/toggle">
<button> TOGGLE </button>
</form>
</body>
</html>
)=====";


void setup()
{
  pinMode(PinLed, OUTPUT);
  WiFi.begin(Ssid,Password);
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Server.on("/",[](){Server.send(200,"text/html","<h1>Hello World!</h1>");});
  Server.on("/Toggle",ToggleLED);
  Server.begin();
}

void loop()
{
  Server.handleClient();
}

void ToggleLED()
{
  digitalWrite(PinLed,!digitalRead(PinLed));
  Server.send_P(200,"text/html", webpage);
}