#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketServer.h>

const char* ssid = "Makerslab";
const char* password = "makers@2022";

const int ledPin = 2; // Pin for the built-in LED
unsigned int number;

WebServer server(80);
WebSocketServer webSocketServer(81); // Create a WebSocket server on port 81

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

        var socket = new WebSocket("ws://" + location.hostname + ":81/");

        socket.onmessage = function(event) {
            console.log("Received message: " + event.data);
        };

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

void handleWebSocketMessage(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    if (type == WStype_TEXT) {
        Serial.println("Received WebSocket message: " + String((char*)payload));
    }
}

void setup() {
    pinMode(ledPin, OUTPUT);
    ledcAttachPin(ledPin, 0);
    ledcSetup(0, 5000, 8); // Configure LEDC channel 0 with a frequency of 5000Hz and 8-bit resolution
    Serial.begin(115200); // Set the baud rate to 115200

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/setBrightness", handleBrightness);
    server.on("/sendStatement", handleSendStatement);
    server.on("/sendNumber", handleSendNumber);
    server.begin();

    webSocketServer.begin();
    webSocketServer.onEvent(handleWebSocketMessage);
}

void loop() {
    if (valueReceived) {
        // ledcWrite(0, brightnessValue); // Write the brightness value to LEDC channel 0
        Serial.println(brightnessValue);
        valueReceived = false;
    }

    server.handleClient();
    webSocketServer.loop();
    // Other tasks, if any
}
