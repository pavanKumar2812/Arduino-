#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

int relayInput = 2;
// const char* Ssid = "TechMMakers";
// const char* Password = "!nn0vationFactory";

const char* Ssid = "Skywalker";
const char* Password = "MayTheForceBeWithYou";

ESP8266WebServer Server(80);

const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
 <head>
   <script src="https://cdnjs.cloudflare.com/ajax/libs/p5.js/1.2.0/p5.js"></script>
   <script src="https://cdnjs.cloudflare.com/ajax/libs/p5.js/1.2.0/addons/p5.sound.min.js"></script>
   <link rel="stylesheet" type="text/css" href="style.css">
   <meta charset="utf-8" />
   <link rel="stylesheet" href="https://unpkg.com/handsfree@8.4.2/build/lib/assets/handsfree.css" />
   <script src="https://unpkg.com/handsfree@8.4.2/build/lib/handsfree.js"></script>
   <title>Fingers Counter</title>
 </head>
 <body>
   <script>
    // Video
    let video;
    // To store the classification
    let label = "";
    // To store the images to show
    let imgList = [];


    function setup() {
        createCanvas(1000, 680)

        handsfree = new Handsfree({
            showDebug: true, // Comment this out to hide the default webcam feed with landmarks
            hands: true
        })


        const gestures = {
            oneFinger: {
                "name": "oneFinger",
                "algorithm": "fingerpose",
                "models": "hands",
                "confidence": 7.5,
                "description": [
                    [
                        "addCurl",
                        "Thumb",
                        "HalfCurl",
                        1
                    ],
                    [
                        "addCurl",
                        "Thumb",
                        "NoCurl",
                        0.07142857142857142
                    ],
                    [
                        "addDirection",
                        "Thumb",
                        "DiagonalUpRight",
                        1
                    ],
                    [
                        "addCurl",
                        "Index",
                        "NoCurl",
                        1
                    ],
                    [
                        "addDirection",
                        "Index",
                        "VerticalUp",
                        1
                    ],
                    [
                        "addDirection",
                        "Index",
                        "DiagonalUpLeft",
                        0.034482758620689655
                    ],
                    [
                        "addCurl",
                        "Middle",
                        "FullCurl",
                        1
                    ],
                    [
                        "addDirection",
                        "Middle",
                        "VerticalUp",
                        1
                    ],
                    [
                        "addCurl",
                        "Ring",
                        "FullCurl",
                        1
                    ],
                    [
                        "addDirection",
                        "Ring",
                        "VerticalUp",
                        1
                    ],
                    [
                        "addCurl",
                        "Pinky",
                        "FullCurl",
                        1
                    ],
                    [
                        "addDirection",
                        "Pinky",
                        "DiagonalUpRight",
                        1
                    ]
                ]
            },
            twoFinger: {
                "name": "twoFinger",
                "algorithm": "fingerpose",
                "models": "hands",
                "confidence": 7.5,
                "description": [
                    [
                        "addCurl",
                        "Thumb",
                        "NoCurl",
                        1
                    ],
                    [
                        "addCurl",
                        "Thumb",
                        "HalfCurl",
                        0.5789473684210527
                    ],
                    [
                        "addDirection",
                        "Thumb",
                        "DiagonalUpRight",
                        1
                    ],
                    [
                        "addDirection",
                        "Thumb",
                        "HorizontalRight",
                        0.35
                    ],
                    [
                        "addDirection",
                        "Thumb",
                        "VerticalUp",
                        0.15
                    ],
                    [
                        "addCurl",
                        "Index",
                        "NoCurl",
                        1
                    ],
                    [
                        "addDirection",
                        "Index",
                        "VerticalUp",
                        1
                    ],
                    [
                        "addDirection",
                        "Index",
                        "DiagonalUpLeft",
                        0.42857142857142855
                    ],
                    [
                        "addCurl",
                        "Middle",
                        "NoCurl",
                        1
                    ],
                    [
                        "addDirection",
                        "Middle",
                        "VerticalUp",
                        1
                    ],
                    [
                        "addDirection",
                        "Middle",
                        "DiagonalUpRight",
                        0.18181818181818182
                    ],
                    [
                        "addDirection",
                        "Middle",
                        "DiagonalUpLeft",
                        0.18181818181818182
                    ],
                    [
                        "addCurl",
                        "Ring",
                        "FullCurl",
                        1
                    ],
                    [
                        "addDirection",
                        "Ring",
                        "DiagonalUpRight",
                        1
                    ],
                    [
                        "addDirection",
                        "Ring",
                        "VerticalUp",
                        0.42857142857142855
                    ],
                    [
                        "addCurl",
                        "Pinky",
                        "FullCurl",
                        1
                    ],
                    [
                        "addDirection",
                        "Pinky",
                        "DiagonalUpRight",
                        1
                    ],
                    [
                        "addDirection",
                        "Pinky",
                        "VerticalUp",
                        0.15384615384615385
                    ]
                ]
            },
            threeFinger: {
                "name": "threeFinger",
                "algorithm": "fingerpose",
                "models": "hands",
                "confidence": 7.5,
                "description": [
                    [
                        "addCurl",
                        "Thumb",
                        "NoCurl",
                        1
                    ],
                    [
                        "addDirection",
                        "Thumb",
                        "HorizontalRight",
                        0.7647058823529411
                    ],
                    [
                        "addDirection",
                        "Thumb",
                        "DiagonalUpRight",
                        1
                    ],
                    [
                        "addCurl",
                        "Index",
                        "NoCurl",
                        1
                    ],
                    [
                        "addDirection",
                        "Index",
                        "VerticalUp",
                        1
                    ],
                    [
                        "addDirection",
                        "Index",
                        "DiagonalUpLeft",
                        0.5
                    ],
                    [
                        "addCurl",
                        "Middle",
                        "NoCurl",
                        1
                    ],
                    [
                        "addDirection",
                        "Middle",
                        "VerticalUp",
                        1
                    ],
                    [
                        "addDirection",
                        "Middle",
                        "DiagonalUpRight",
                        0.125
                    ],
                    [
                        "addDirection",
                        "Middle",
                        "DiagonalUpLeft",
                        0.125
                    ],
                    [
                        "addCurl",
                        "Ring",
                        "NoCurl",
                        1
                    ],
                    [
                        "addDirection",
                        "Ring",
                        "DiagonalUpRight",
                        1
                    ],
                    [
                        "addDirection",
                        "Ring",
                        "VerticalUp",
                        0.7647058823529411
                    ],
                    [
                        "addCurl",
                        "Pinky",
                        "FullCurl",
                        1
                    ],
                    [
                        "addDirection",
                        "Pinky",
                        "DiagonalUpRight",
                        1
                    ],
                    [
                        "addDirection",
                        "Pinky",
                        "VerticalUp",
                        0.36363636363636365
                    ]
                ]
            }
        }

        handsfree.useGesture(gestures.oneFinger)
        handsfree.useGesture(gestures.twoFinger)
        handsfree.useGesture(gestures.threeFinger)
        handsfree.enableGestures()


        // Create start model button
        buttonStart = createButton('Start Model')
        buttonStart.mousePressed(() => handsfree.start())
        // Create a stop button
        buttonStop = createButton('Stop Model')
        buttonStop.mousePressed(() => handsfree.stop())

        video = createCapture(VIDEO);
        video.size(width, height);
        video.hide();

    }

    function draw() {
        tint(255)
        //Draw and flip the video
        push();
        scale(-1,1)
        image(video, -width, 0);
        pop();
        gestureLabel = getHandGestureLabel()
        console.log(gestureLabel)
    }

    function getHandGestureLabel() {
        const hands = handsfree.data?.hands
        if (hands?.gesture) {
            if (hands.gesture[0]) {
                return hands.gesture[0].name ? hands.gesture[0].name : ""
            }
        }
    }

    function logger() {
        
    }
   </script>
 </body>
</html>
)rawliteral";

void setup() {
  pinMode(relayInput, OUTPUT);
  WiFi.begin(Ssid, Password);
  Serial.begin(115200);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Server.on("/", HTTP_GET, []() {
    Server.send_P(200, "text/html", webpage);
  });

  Server.begin();
}

void loop() {
  Server.handleClient();
  digitalWrite(relayInput, HIGH);
  delay(1000);
  digitalWrite(relayInput, LOW);
  delay(1000);
}