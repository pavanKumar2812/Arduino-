#include <WiFi.h>

int red = 22;
int yellow = 23;
int green = 21;

const char* ssid = "OnePlus NordCE 5G";
const char* password = "12345678";

bool connectedToWiFi = false;

void setup() 
{
  Serial.begin(115200);

  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
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
}