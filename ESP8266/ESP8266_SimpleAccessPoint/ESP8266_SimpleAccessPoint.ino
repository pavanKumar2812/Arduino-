#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "MoonNight";
const char* password = "STEVE";

ESP8266WebServer server(80);

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Namaste</h1>";
  html += "<h1>You can now control the this traffic single Hinjewadi Shivaji chowk North</h1>";
  html += "<p>SSID: " + WiFi.SSID() + "</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  // Set up the ESP8266 as an Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  // Print the ESP8266's IP address to the serial monitor
  Serial.begin(115200);
  Serial.println();
  Serial.print("Access Point SSID: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Define the web server route
  server.on("/", handleRoot);

  // Start the server
  server.begin();
}

void loop() {
  // Handle client requests
  server.handleClient();
}
