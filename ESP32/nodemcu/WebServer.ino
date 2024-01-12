// To establish the connection to wifi and turn off and on inbuilt LED in NodeMCU by web

#include <ESP8266WiFi.h>

WiFiClient client;
WiFiServer server(80);

const int dt = 200;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  WiFi.begin("IDR@MakersLab", "Robots0nly");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("..");
    delay(dt);
  }
  Serial.println("");
  Serial.println("NodeMCU is connected...!");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // Get a client that is connected to the server and has data available for reading
  client = server.available();
  if (client == 1) {
    String request = client.readStringUntil('\n');
    Serial.println(request);
    request.trim(); // removes all leading/trailing whitespaces from the string
    if (request == "GET /ledon HTTP/1.1") {
      digitalWrite(2, LOW);
    }
    if (request == "GET /ledoff HTTP/1.1") {
      digitalWrite(2, HIGH);
    }
  }
}
