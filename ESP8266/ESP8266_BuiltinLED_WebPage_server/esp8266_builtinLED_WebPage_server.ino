#include <ESP8266WiFi.h>

WiFiClient client;
WiFiServer server(80);

const int ledPin = 2; // GPIO pin where the LED is connected
const int dt = 200;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
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
  if (client) {
    String request = client.readStringUntil('\n');
    Serial.println(request);
    request.trim(); // removes all leading/trailing whitespaces from the string
    if (request.indexOf("GET /ledon HTTP/1.1") != -1) {
      digitalWrite(ledPin, LOW);
    }
    if (request.indexOf("GET /ledoff HTTP/1.1") != -1) {
      digitalWrite(ledPin, HIGH);
    }

    // Send the HTTP response with a simple HTML page
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println("<html>");
    client.println("<body>");
    client.println("<h1>NodeMCU LED Control</h1>");
    client.println("<p>Click below to control the LED:</p>");
    client.println("<a href=\"/ledon\">Turn LED On</a><br>");
    client.println("<a href=\"/ledoff\">Turn LED Off</a>");
    client.println("</body>");
    client.println("</html>");
    client.println();
    client.stop();
  }
}
