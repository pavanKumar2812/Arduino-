#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server;
uint8_t PinLed = 16;
const char* Ssid = "TechMMakers";
const char* Password = "!nn0vationFactory";

char webpage[] PROGMEM = R"=====(
<html>
<head>
</head>
<body>
<p> LED Status: <span id="led-state">__</span> </p>
<button onclick="myFunction()"> TOGGLE </button>
</body>
<script>
function myFunction()
{
  console.log("button was clicked!");
  var xhr = new XMLHttpRequest();
  var url = "/ledstate";

  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("led-state").innerHTML = this.responseText;
    }
  };

  xhr.open("GET", url, true);
  xhr.send();
};
document.addEventListener('DOMContentLoaded', myFunction, false);
</script>
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

  server.on("/",[](){server.send_P(200,"text/html", webpage);});
  server.on("/ledstate",getLEDState);
  server.begin();
}

void loop()
{
  server.handleClient();
}

void toggleLED()
{
  digitalWrite(PinLed,!digitalRead(PinLed));
}

void getLEDState()
{
  toggleLED();
  String led_state = digitalRead(PinLed) ? "ON" : "OFF";
  server.send(200,"text/plain", led_state);
}
