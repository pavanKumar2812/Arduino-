#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

ESP8266WebServer server;
WebSocketsServer webSocket = WebSocketsServer(81);

uint8_t pin_led = 16;
char* ssid = "Venkeydresses";
char* password = "Venkat@676";

char webpage[] PROGMEM = R"=====(
<html>
<head>
  <script>
    var Socket;
    function Init() 
    {
      Socket = new WebSocket("ws://"+ window.location.hostname + ":81/");
      Socket.onmessage = function(event)
      {
        document.getElementById("RxConsole").value += event.data;
      }
    }
  </script>
</head>
<body onload="javascript:Init()">
  <textarea id="RxConsole"></textarea>
</body>
</html>
)=====";

void setup()
{
  pinMode(pin_led, OUTPUT);
  WiFi.begin(ssid,password);
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/",[](){server.send(200,"text/html", webpage);});
  server.begin();
  webSocket.begin();
}

void loop()
{
  webSocket.loop();
  server.handleClient();
  if(Serial.available() > 0)
  {
    char c[] = { (char)Serial.read() };
    webSocket.broadcastTXT(c, sizeof(c));
  }
}