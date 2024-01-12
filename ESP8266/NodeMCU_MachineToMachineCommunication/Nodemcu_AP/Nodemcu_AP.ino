#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

const char* SSID = "MoonKnight";
const char* Password = "P@v@nKum@r";
int MaxConnection = 4;
const char* AmbulanceState = "I";

// Static IP
IPAddress StaticIP(10, 10, 10, 1);
IPAddress Gateway(10, 10, 10, 1);
IPAddress Subnet(255, 255, 255, 0);

String a;
String Payload;
int HttpCode;
String URL;

HTTPClient http;

ESP8266WebServer server(80); 

// This run when we open its IP in browser
void HandleRoot()
{ 
  if (strcmp(AmbulanceState, "A") == 0)
  {
    server.send(200, "text/plain", "Ambulance A");
  } 
  else if (strcmp(AmbulanceState, "D") == 0)
  {
    server.send(200, "text/plain", "Ambulance D");
  } 
  else
  {
    server.send(200, "text/plain", "Ambulance Single Tripping");
  }
}

void Ambulance_A()
{
  server.send(200, "text/plain", "Ambulance A");
  Serial.println("Ambulance A");
  AmbulanceState = "A"; 
  digitalWrite(D2, HIGH);
}

void Ambulance_D()
{
  server.send(200, "text/plain", "Ambulance D");
  Serial.println("Ambulance D");
  AmbulanceState = "D"; 
  digitalWrite(D2, LOW);
}


void setup() 
{
  Serial.begin(115200);
  Serial.println("");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID, Password, 1, false, MaxConnection);
  WiFi.softAPConfig(StaticIP, Gateway, Subnet);
  Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());

  IPAddress MyIP = WiFi.softAPIP(); // Get IP Address
  Serial.print("Host IP: ");
  Serial.println(MyIP);
  pinMode(D2, OUTPUT);

  delay(1000);

  server.on("/", HandleRoot);
  server.on("/Ambulance_A", Ambulance_A);
  server.on("/Ambulance_D", Ambulance_D);
  server.begin();
  Serial.println("HTTP server started");

}

void loop() 
{
  Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
  delay(3000);
  server.handleClient();
}
