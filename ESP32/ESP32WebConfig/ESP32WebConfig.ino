#include <Arduino.h>
#include "ESP32WebConfig.h"

ESP32WebConfig webConfig("esp32","abc","123");

void setup() {
  webConfig.BEGIN();
}

void loop() {
  webConfig.HANDLECLIENT();
}