#include <WiFi.h>
#include <WebServer.h>

// const char* Ssid = "Neurosis";
// const char* Password = "12345678";
const char* Ssid = "The Skywalkers";
const char* Password = "MayTheForceBeWithYou";

unsigned int Number;

WebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
    <!DOCTYPE HTML>
    <html>
      <head>
        <title>LED Brightness Control</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="icon" href="data:,">
      </head>
      <body>
        <h2>ESP32 Network Test</h2>
        <button onclick="SendPacket100Bits()">Packet100Bits</button>
        <button onclick="SendPacket2KB()">Packet2KB</button>
        <script>
          var BufferDelay = false;
          var SetTimerValue = 500;
          var Packet100Bits = CreatePacket(100);
          var Packet2KB = CreatePacket(2 * 1024);

          function CreatePacket(PacketSize) 
          {
            const Packet = new Uint8Array(PacketSize);
            // Fill the packet with some data
            for (let i = 0; i < PacketSize; i++) {
              Packet[i] = i % 256; // Assign a value between 0 and 255 to each byte
            }
            return Packet;
          }
        
          function SendPacket100Bits() {
            var timestamp = new Date().toLocaleTimeString(undefined, { hour12: false }) + `.${new Date().getMilliseconds()}`;
            var packet = `${Packet100Bits} : ${timestamp} : SendingFromBrowserToESP32`;
            var xhr = new XMLHttpRequest();
            var url = "/SendPacket100Bits?100Bits=" + encodeURIComponent(packet);
            xhr.open("GET", url, true);

            xhr.onreadystatechange = function() 
            {
              var ReceivedTimestamp100Bits = new Date().toLocaleTimeString(undefined, { hour12: false }) + `.${new Date().getMilliseconds()}`;
              if (xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {
                var data = xhr.responseText;
                var sendTime = data.match(/Sending at:([^ ]+)/)[1];
                
                var sendDateTime = new Date(`2000-01-01T${sendTime}`);
                var receiveDateTime = new Date(`2000-01-01T${ReceivedTimestamp100Bits}`);
                
                var timeDifference = receiveDateTime.getTime() - sendDateTime.getTime();
                console.log(`${xhr.responseText} : ack received time: ${ReceivedTimestamp100Bits}: ${timeDifference} milliseconds`);
              }
            };

            xhr.send();

            var i = 1;
            while(i<=1000)
            {
              SendPacket100Bits();
            }
          }

          function sendPackets100BitsWithDelay(count, delay) {
            var i = 0;
            var intervalId = setInterval(function() {
              SendPacket100Bits();
              i++;
              if (i >= count) {
                clearInterval(intervalId); // Stop the interval after sending the specified number of packets
              }
            }, delay);
          }

          function SendPacket2KB() 
          {
            setTimeout(function() 
            {
              var timestamp = new Date().toLocaleTimeString(undefined, { hour12: false }) + `.${new Date().getMilliseconds()}`;
              var packet2KB = `${Packet2KB} : ${timestamp} : SendingFromBrowserToESP32`;
              var xhr = new XMLHttpRequest();
              var url = "/SendPacket2KB?2KB=" + encodeURIComponent(packet2KB);
              xhr.open("GET", url, true);

              xhr.onreadystatechange = function() 
              {
                var ReceivedTimestamp2KB = new Date().toLocaleTimeString(undefined, { hour12: false }) + `.${new Date().getMilliseconds()}`;
                if (xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) 
                {
                  var data = xhr.responseText;
                  var sendTime = data.match(/Sending at:([^ ]+)/)[1];
                  
                  var sendDateTime = new Date(`2000-01-01T${sendTime}`);
                  var receiveDateTime = new Date(`2000-01-01T${ReceivedTimestamp2KB}`);
                  
                  var timeDifference = receiveDateTime.getTime() - sendDateTime.getTime();
                  console.log(`${xhr.responseText} : ack received time: ${ReceivedTimestamp2KB}: ${timeDifference} milliseconds`);
                }
              };
              xhr.send();
            }, SetTimerValue);

            var i = 1;
            while(i<=1000)
            {
              SendPacket2KB();
            }
          }

          function sendPackets2KBWithDelay(count, delay) {
            var i = 0;
            var intervalId = setInterval(function() {
              SendPacket2KB();
              i++;
              if (i >= count) {
                clearInterval(intervalId); // Stop the interval after sending the specified number of packets
              }
            }, delay);
          }

          function delayedConsoleLog(number) 
          {
            setTimeout(function() 
            {
              var http = new XMLHttpRequest();
              var url = "/sendNumber";
              http.open("POST", url, true);
              http.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
              http.send("number=" + number);

              if (number < 1000) 
              {
                http.onreadystatechange = function() 
                {
                  if (http.readyState === XMLHttpRequest.DONE && http.status === 200) 
                  {
                    var timestamp = new Date().toLocaleTimeString(undefined, { hours12: false }) + `.${new Date().getMilliseconds()}`;
                    document.body.append(http.responseText);
                    console.log(http.responseText + " - " + number + " - " + timestamp );
                    delayedConsoleLog(number + 1);
                  }
                };
              }
            }, SetTimerValue);
          }
        </script>
      </body>
    </html>
)rawliteral";

void handleRoot() 
{
  server.send(200, "text/html", index_html);
}

void handlePacket100Bits() 
{
  String ReceivedTimeData100Bits;
  if (server.hasArg("100Bits"))
  {
    String Packet100BitsData = server.arg("100Bits");
    Serial.println(Packet100BitsData);
    ReceivedTimeData100Bits = Packet100BitsData.substring(Packet100BitsData.indexOf(" : ") + 3, Packet100BitsData.lastIndexOf(" : "));
  }
  server.send(200, "text/plain", "Packet100Bits Sending at:" + ReceivedTimeData100Bits);
}

void handlePacket2KB() 
{
  String ReceivedTimeData2KB;
  if (server.hasArg("2KB")) 
  {
    String packet2KBData = server.arg("2KB"); // Corrected the variable name
    Serial.print(packet2KBData);
    Serial.println(millis());
    ReceivedTimeData2KB = packet2KBData.substring(packet2KBData.indexOf(" : ") + 3, packet2KBData.lastIndexOf(" : "));
  }
  server.send(200, "text/plain", "Packet2KB Sending at:" + ReceivedTimeData2KB);
}

void handleSendNumber()
{
  if (server.hasArg("number")) 
  {
    String NumberStr = server.arg("number");
    Number = NumberStr.toInt();
    Serial.print(millis());
    Serial.print(": ");
    Serial.print("Rx - ");
    Serial.println(Number);
  }
  server.send(200, "text/plain", "OK");
}

void setup() 
{
  Serial.begin(115200);  // Set the baud rate to 115200

  WiFi.begin(Ssid, Password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/SendPacket100Bits", handlePacket100Bits);
  server.on("/SendPacket2KB", handlePacket2KB);
  server.begin();
}

void loop() 
{
  server.handleClient();
  delay(10);
}