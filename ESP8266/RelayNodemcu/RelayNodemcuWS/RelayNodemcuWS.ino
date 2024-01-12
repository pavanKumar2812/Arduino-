#include <WebSocketsServer.h>
#include <ESP8266WiFi.h>

int relayInput = 2;

WebSocketsServer webSocket(81);
const char* Ssid = "Skywalker";
const char* Password = "MayTheForceBeWithYou";

void setup() {
  pinMode(relayInput, OUTPUT);
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(Ssid, Password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Print NodeMCU IP address
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(handleWebSocketMessage);
}

void loop() {
  webSocket.loop();
  // digitalWrite(relayInput, HIGH);
  // delay(1000);
  // digitalWrite(relayInput, LOW);
  // delay(1000);
}

void handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  Serial.println("Received data: " + String((char*)payload));
  String receivedData = String((char*)payload);
  if (receivedData == "oneFinger") {
    digitalWrite(relayInput, HIGH);
  } 
}
