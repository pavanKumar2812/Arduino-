#include "ESP32WebConfig.h"

ESP32WebConfig::ESP32WebConfig(const char* HOSTNAME, const char* SSID, const char* PASSWORD) : _HOSTNAME(HOSTNAME), _SSID(SSID), _PASSWORD(PASSWORD), _SERVER(80), _PREFERENCES() 
{
  DEBUG_FLAG = true;
}

void ESP32WebConfig::BEGIN() 
{
  Serial.begin(115200);
  int attempts = 0;

  // Open preferences with a namespace called "wifi"
  _PREFERENCES.begin("wifi", false);

  // Try to read Wi-Fi credentials from preferences
  String savedSsid = _PREFERENCES.getString("SSID", "");
  String savedPassword = _PREFERENCES.getString("PASSWORD", "");
  debugPrint("Saved SSID: ");
  debugPrintln(savedSsid.c_str());

  WiFi.begin(savedSsid, savedPassword);
  while (WiFi.status() != WL_CONNECTED && attempts < MAX_WIFI_CONNECT_ATTEMPTS) 
  {
    delay(1000);
    debugPrintln("Connecting to WiFi...");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) 
  {
    debugPrint("Connected to WiFi SSID: ");
    debugPrintln(savedSsid.c_str());
    debugPrint("IP address: ");
    debugPrintln(WiFi.localIP().toString().c_str());
  } 
  else 
  {
    debugPrintln("Connection to saved Wi-Fi failed. Switching to AP mode...");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("Enetra2", "MyPassword");  // Set your desired AP SSID and password
    debugPrint("AP IP address: ");
    debugPrintln(WiFi.softAPIP().toString().c_str());
  }
  // Setup mDNS
  if (!MDNS.begin(_HOSTNAME)) 
  {
    debugPrintln("Error setting up MDNS responder!");
    while (1) 
    {
      delay(1000);
    }
  }
  debugPrintln("mDNS responder started");

  _SERVER.on("/", HTTP_GET, [this](){ this->HANDLEROOT(); });
  _SERVER.on("/save", HTTP_POST, [this](){ this->HANDLESAVE(); });
  _SERVER.on("/reset", HTTP_GET, [this](){ this->HANDLERESET(); });
  _SERVER.on("/update", HTTP_POST, [this](){ this->SENDUPDATERESPONSEANDRESTART(); }, [this](){ this->HANDLEFIRMWAREUPDATE(); });
  _SERVER.on("/wifi-config", HTTP_GET, [this](){ this->HANDLEWIFICONFIG(); });
  _SERVER.on("/firmware-update", HTTP_GET, [this](){ this->HANDLEFIRMWAREUPDATEPAGE(); });
  _SERVER.begin();
}

void ESP32WebConfig::debugPrint(const char* MESSAGE1) 
{
  if (DEBUG_FLAG) 
  {
    Serial.print(MESSAGE1);
  }
}

void ESP32WebConfig::debugPrintln(const char* MESSAGE2) 
{
  if (DEBUG_FLAG) 
  {
    Serial.println(MESSAGE2);
  }
}

void ESP32WebConfig::HANDLECLIENT() 
{
  _SERVER.handleClient();
}

void ESP32WebConfig::HANDLEROOT() 
{
  String html = "<html><head>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; margin: 20px; }";
  html += "h1 { color: #333; }";
  html += "a { color: #0074D9; text-decoration: none; }";
  html += "a:hover { text-decoration: underline; }";
  html += ".button { display: inline-block; background-color: #0074D9; color: #fff; padding: 10px 20px; margin: 10px; text-align: center; text-decoration: none; border-radius: 4px; cursor: pointer; }";
  html += ".form-container { max-width: 400px; margin: 0 auto; text-align: left; }";
  html += ".form-container label { display: block; margin-bottom: 5px; }";
  html += ".form-container input[type='text'], .form-container input[type='password'] { width: 100%; padding: 10px; margin-bottom: 10px; border: 1px solid #ccc; border-radius: 4px; }";
  html += ".form-container input[type='submit'] { background-color: #0074D9; color: #fff; padding: 10px 20px; border: none; border-radius: 4px; cursor: pointer; }";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>Welcome</h1>";
  html += "<a class='button' href='/wifi-config'>Configure Wi-Fi</a>";
  html += "<a class='button' href='/firmware-update'>Firmware Update</a>";
  html += "</body></html>";

  _SERVER.send(200, "text/html", html);
}

void ESP32WebConfig::HANDLEWIFICONFIG() 
{
  String html = "<html><head>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; margin: 20px; }";
  html += "h1 { color: #333; }";
  html += ".form-container { max-width: 400px; margin: 0 auto; text-align: left; }";
  html += ".form-container label { display: block; margin-bottom: 5px; }";
  html += ".form-container input[type='text'], .form-container input[type='PASSWORD'] { width: 100%; padding: 10px; margin-bottom: 10px; border: 1px solid #ccc; border-radius: 4px; }";
  html += ".form-container input[type='submit'] { background-color: #0074D9; color: #fff; padding: 10px 20px; border: none; border-radius: 4px; cursor: pointer; }";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>Wi-Fi Configuration</h1>";
  html += "<div class='form-container'>";
  html += "<form method='post' action='/save'>";
  html += "<label for='SSID'>SSID:</label>";
  html += "<input type='text' name='SSID' id='SSID' value='" + String(_SSID) + "'><br>";
  html += "<label for='PASSWORD'>Password:</label>";
  html += "<input type='PASSWORD' name='PASSWORD' id='PASSWORD' value='" + String(_PASSWORD) + "'><br>";
  html += "<input type='submit' value='Save'>";
  html += "</form>";
  html += "<p><a href='/reset'>Reset Wi-Fi Credentials</a></p>";
  html += "</div></body></html>";

  _SERVER.send(200, "text/html", html);
}

void ESP32WebConfig::HANDLEFIRMWAREUPDATEPAGE() 
{
  String html = "<html><head>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; margin: 20px; }";
  html += "h1 { color: #333; }";
  html += ".form-container { max-width: 400px; margin: 0 auto; text-align: left; }";
  html += ".form-container .center-content { text-align: center; }"; // Center align the label and button
  html += ".form-container label { display: block; margin-bottom: 5px; }";
  html += ".form-container input[type='file'] { display: none; }";
  html += ".form-container label[for='file-input'] { display: inline-block; padding: 10px 20px; background-color: #0074D9; color: #fff; cursor: pointer; border-radius: 4px; }";
  html += ".form-container #file-input { display: none; }";
  html += ".form-container .btn-container { text-align: center; }"; // Center align the buttons
  html += ".form-container .btn { background-color: #0074D9; color: #fff; padding: 10px 20px; border: none; border-radius: 4px; cursor: pointer; }";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>Firmware Update</h1>";
  html += "<div class='form-container'>";
  html += "<form method='POST' action='/update' enctype='multipart/form-data' id='upload_form'>";
  html += "<div class='center-content'>"; // Container for center-aligned label and button
  html += "<br><label for='file-input' id='file-label'>Choose firmware file...</label>";
  html += "<input type='file' name='update' id='file-input' onchange='sub(this)'>";
  html += "</div>"; // End of center-content
  html += "<div class='btn-container'>"; // Container for center-aligned buttons
  html += "<br><br><input type='submit' class='btn' value='Update'><br><br>";
  html += "<div id='prg'></div>";
  html += "</div>"; // End of btn-container
  html += "</form></div></body></html>";

  _SERVER.send(200, "text/html", html);
}

void ESP32WebConfig::HANDLESAVE() 
{
  // Implement logic for saving Wi-Fi credentials here
  if (_SERVER.method() == HTTP_POST) 
  {
    String NEWSSID = _SERVER.arg("SSID");
    String NEWPASSWORD = _SERVER.arg("PASSWORD");

    // Save the new Wi-Fi credentials to preferences
    _PREFERENCES.putString("SSID",NEWSSID);
    _PREFERENCES.putString("PASSWORD",NEWPASSWORD);

    _SERVER.send(200, "text/plain", "Wi-Fi credentials saved. Rebooting...");
    delay(1000);
    ESP.restart();
  }
}

void ESP32WebConfig::HANDLERESET() 
{
  // Clear the Wi-Fi credentials from preferences
  _PREFERENCES.clear();
  _SERVER.send(200, "text/plain", "Wi-Fi credentials reset. Rebooting...");
  delay(1000);
  ESP.restart();
}

void ESP32WebConfig::SENDUPDATERESPONSEANDRESTART() 
{
  _SERVER.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
  delay(1000);
  ESP.restart();
}

void ESP32WebConfig::HANDLEFIRMWAREUPDATE() 
{
  HTTPUpload& upload = _SERVER.upload();
  Update.begin(UPDATE_SIZE_UNKNOWN);
  if (upload.status == UPLOAD_FILE_WRITE) 
  {
    Update.write(upload.buf, upload.currentSize);
  } 
  else if (upload.status == UPLOAD_FILE_END) 
  {
    Update.end(true);
  }
}
