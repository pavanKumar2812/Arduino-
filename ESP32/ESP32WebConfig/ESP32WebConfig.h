#ifndef ESP32WEBCONFIG_H
#define ESP32WEBCONFIG_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <ESPmDNS.h>
#include <Update.h>

class ESP32WebConfig {
public:
  ESP32WebConfig(const char* HOSTNAME, const char* SSID, const char* PASSWORD);
  void BEGIN();
  void HANDLECLIENT();
  
private:
  const char* _HOSTNAME;
  const char* _SSID;
  const char* _PASSWORD;
  const int MAX_WIFI_CONNECT_ATTEMPTS = 8;
  WebServer _SERVER;
  Preferences _PREFERENCES;
  bool DEBUG_FLAG;

  
  void debugPrint(const char* MESSAGE1);
  void debugPrintln(const char* MESSAGE2);
  void HANDLEROOT();
  void HANDLEWIFICONFIG();
  void HANDLEFIRMWAREUPDATEPAGE();
  void HANDLESAVE();
  void HANDLERESET();
  void SENDUPDATERESPONSEANDRESTART();
  void HANDLEFIRMWAREUPDATE();
};

#endif
