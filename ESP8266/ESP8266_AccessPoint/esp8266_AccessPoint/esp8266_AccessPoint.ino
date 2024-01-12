#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* SSID = "Pavan Kumar A";
const char* PASSWORD = "Pavan";

IPAddress local_IP(192,168,1,22);
IPAddress gateway(192,168,1,5);
IPAddress subnet(255,255,255,0);

const int ledPin = 2; // Pin for the built-in LED
unsigned int number;

ESP8266WebServer server(80);
volatile int brightnessValue = 0;
volatile bool valueReceived = false;

const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE HTML>
    <html>
    <head>
        <title>LED Brightness Control</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="icon" href="data:,">
    </head>
    <body>
        <input type="range" min="0" max="1000" value="0" oninput="SendData()" id="Slider">
        <button class="SendSample" id="Send" onclick="send()">Send</button>
        <button onclick="sendNumbers()">Send Numbers</button>
    <script>
        var Slider = document.getElementById("Slider");
        var btn = document.getElementById("Send");
        var BufferDelay = false;
        const stmt = "Powerstar Pawan Kalyan is currently in the news, not for his fiery political speeches but for his personal life. Speculations are doing the rounds that Pawan and his wife, Anna Lezhneva, are heading for a divorce. It is said that the two have been living separately for a while now. Anna has not been seen with Pawan Kalyan at his family events."
      
        function send() {
          var xhr = new XMLHttpRequest();
          var url = "/sendStatement?stmt=" + encodeURIComponent(stmt);
          xhr.open("GET", url, true);
          xhr.send();
        }

        function sendNumbers() {
        var i = 1;

        function sendNumber() {
            var xhr = new XMLHttpRequest();
            var url = "/sendNumber";
            xhr.open("POST", url, true);
            xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
            xhr.send("number=" + i);

            i++;

            if (i <= 1000) {
                xhr.onreadystatechange = function() {
                    if (xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {
                        sendNumber();
                    }
                };
            }
        }

        sendNumber();
    }

        function SendData() {
            if (BufferDelay) {
                return; // Ignore actions if data is being sent too fast
            }

            var xhr = new XMLHttpRequest();
            var url = "/setBrightness?brightness=" + Slider.value;
            xhr.open("GET", url, true);
            xhr.send();
            BufferDelay = true;

            setTimeout(function() {
                BufferDelay = false;
            }, 15);
        }
    </script>
    </body>
    </html>
)rawliteral";

void handleRoot() {
    server.send(200, "text/html", index_html);
}

void handleBrightness() {
    if (server.hasArg("brightness")) {
        if (!valueReceived) {
          String brightnessStr = server.arg("brightness");
          brightnessValue = brightnessStr.toInt();
          valueReceived = true;
        }
    }

    server.send(200, "text/plain", "OK");
}

void handleSendStatement() {
    if (server.hasArg("stmt")) {
        String stmtStr = server.arg("stmt");
        // Do something with the received statement, e.g., print it
        Serial.println(stmtStr);
    }

    server.send(200, "text/plain", "OK");
}

void handleSendNumber() {
    if (server.hasArg("number")) {
        String numberStr = server.arg("number");
        number = numberStr.toInt();
        Serial.print("Received number: ");
        Serial.println(number);
    }

    server.send(200, "text/plain", "OK");
}

void setup() {
    pinMode(ledPin, OUTPUT);
    Serial.print("Hola");

    // ledcAttachPin(ledPin, 0);
    // ledcSetup(0, 5000, 8); // Configure LEDC channel 0 with a frequency of 5000Hz and 8-bit resolution

    Serial.begin(115200); // Set the baud rate to 115200

    Serial.print("Setting up Access Point ... ");
    Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

    Serial.print("Starting Access Point ... ");
    Serial.println(WiFi.softAP(SSID, PASSWORD) ? "Ready" : "Failed!");

    Serial.print("IP address = ");
    Serial.println(WiFi.softAPIP());

    server.on("/", handleRoot);
    server.on("/setBrightness", handleBrightness);
    server.on("/sendStatement", handleSendStatement);
    server.on("/sendNumber", handleSendNumber);
    server.begin();
}

void loop() {
    if (valueReceived) {
        // ledcWrite(0, brightnessValue); // Write the brightness value to LEDC channel 0
        Serial.println(brightnessValue);
        valueReceived = false;
    }
    server.handleClient();
    // Other tasks, if any
}