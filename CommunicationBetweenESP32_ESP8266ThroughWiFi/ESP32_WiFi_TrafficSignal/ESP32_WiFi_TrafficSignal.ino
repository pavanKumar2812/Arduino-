#include <WiFi.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

int red = 22;
int yellow = 23;
int green = 21;

const char* ssid = "MyESP8266AP";
const char* password = "password123";

bool connectedToWiFi = false;

SoftwareSerial SerialGPS(4, 5); // SoftwareSerial for GPS module

TinyGPSPlus gps; // Create an instance of the TinyGPS++ library


void setup() 
{
  Serial.begin(115200);

  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  // Clear any previous GPS data
  gps = TinyGPSPlus();
}

void loop() 
{
  int rssi = WiFi.RSSI();
  Serial.print("RSSI: ");
  Serial.println(rssi);

  if ((!connectedToWiFi && WiFi.status() == WL_CONNECTED) && (rssi > -50)) 
  {
    connectedToWiFi = true;
    Serial.println("Connected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(yellow, LOW);
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
  }

  if (WiFi.status() != WL_CONNECTED) {
    if (connectedToWiFi) {
      connectedToWiFi = false;
      Serial.println("Disconnected from WiFi");
      delay(5000); // Wait for 5 seconds before trying to reconnect
      WiFi.begin(ssid, password);
      Serial.println("Reconnecting to WiFi...");
    }
    digitalWrite(green, LOW);
    digitalWrite(yellow, HIGH);
    delay(3000);

  // turn off yellow, then turn red on for 5 seconds
    digitalWrite(yellow, LOW);
    digitalWrite(red, HIGH);
    delay(5000);

  // red and yellow on for 2 seconds (red is already on though)
    digitalWrite(yellow, HIGH);
    delay(2000);

  // turn off red and yellow, then turn on green
    digitalWrite(yellow, LOW);
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    delay(3000);
  }

  // Read and print GPS data
  while (SerialGPS.available() > 0) {
    char c = SerialGPS.read();
    gps.encode(c); // Pass the character to TinyGPS++
    Serial.println("GPS data received");
  }

  // Check for valid GPS data and print it
  if (gps.location.isValid()) {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
  }
}