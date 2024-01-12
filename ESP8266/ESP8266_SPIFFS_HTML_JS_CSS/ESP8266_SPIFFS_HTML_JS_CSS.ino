#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

ESP8266WebServer server;
uint8_t PinLed = 16;
const char* Ssid = "TechMMakers";
const char* Password = "!nn0vationFactory";

void setup()
{
  SPIFFS.begin();
  pinMode(PinLed, OUTPUT);
  WiFi.begin(Ssid, Password);
  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", serveIndexFile);
  server.on("/Script.js", serveJavaScriptFile); // Add a handler for JavaScript file
  server.on("/Style.css", serveCSSFile); // Add a handler for CSS file
  server.on("/ledstate", getLEDState);
  server.begin();
}

void loop()
{
  server.handleClient();
}

void serveIndexFile()
{
  File file = SPIFFS.open("/Index.html", "r");
  server.streamFile(file, "text/html");
  file.close();
  Serial.println("HTML file is Served");
}

void serveJavaScriptFile()
{
  File file = SPIFFS.open("/Script.js", "r");
  server.streamFile(file, "application/javascript"); // Set the correct MIME type
  file.close();
  Serial.println("JavaScript file is Served");
}

void serveCSSFile()
{
  File file = SPIFFS.open("/Style.css", "r");
  server.streamFile(file, "text/css"); // Set the correct MIME type
  file.close();
  Serial.println("CSS file is Served");
}

void toggleLED()
{
  digitalWrite(PinLed, !digitalRead(PinLed));
}

void getLEDState()
{
  toggleLED();
  String led_state = digitalRead(PinLed) ? "ON" : "OFF";
  server.send(200, "text/plain", led_state);
}
