#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

const char* ssid = "MyESP8266AP";
const char* password = "password123";

TinyGPSPlus gps;
SoftwareSerial SerialGPS(4, 5); 

float Latitude, Longitude;
int year, month, date, hour, minute, second;
String DateString, TimeString, LatitudeString, LongitudeString;

ESP8266WebServer server(80);

void sendGPSDataToESP32() 
{
  String data = "Latitude=" + LatitudeString + "&Longitude=" + LongitudeString + "&Date=" + DateString + "&Time=" + TimeString;
  server.send(200, "text/plain", data);
}

void handleRoot() 
{
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>NEO-6M GPS Readings</title> <style>";
  s += "table, th, td {border: 1px solid blue;} </style> </head> <body> <h1  style=";
  s += "font-size:300%;";
  s += " ALIGN=CENTER>NEO-6M GPS Readings</h1>";
  s += "<p ALIGN=CENTER style=""font-size:150%;""";
  s += "> <b>Location Details</b></p> <table ALIGN=CENTER style=";
  s += "width:50%";
  s += "> <tr> <th>Latitude</th>";
  s += "<td ALIGN=CENTER >";
  s += LatitudeString;
  s += "</td> </tr> <tr> <th>Longitude</th> <td ALIGN=CENTER >";
  s += LongitudeString;
  s += "</td> </tr> <tr>  <th>Date</th> <td ALIGN=CENTER >";
  s += DateString;
  s += "</td></tr> <tr> <th>Time</th> <td ALIGN=CENTER >";
  s += TimeString;
  s += "</td>  </tr> </table> ";
  
  if (gps.location.isValid()) {
    s += "<p align=center><a style=""color:RED;font-size:125%;"" href=""http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=";
    s += LatitudeString;
    s += "+";
    s += LongitudeString;
    s += """ target=""_top"">Click here</a> to open the location in Google Maps.</p>";
  }

  s += "</body> </html> \n";

  server.send(200, "text/html", s);
}

void setup() 
{
  Serial.begin(9600);
  SerialGPS.begin(9600);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.print("Access Point SSID: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Define the web server route
  server.on("/", handleRoot);
  server.on("/sendGPSData", HTTP_GET, sendGPSDataToESP32);  // New route to send GPS data to ESP32

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  // Handle client requests
  server.handleClient();

  while (SerialGPS.available() > 0) 
  {
    if (gps.encode(SerialGPS.read())) 
    {
      if (gps.location.isValid()) 
      {
        Latitude = gps.location.lat();
        LatitudeString = String(Latitude, 6);
        Longitude = gps.location.lng();
        LongitudeString = String(Longitude, 6);
      }

      if (gps.date.isValid()) 
      {
        DateString = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();

        if (date < 10)
          DateString = '0';
        DateString += String(date);

        DateString += " / ";

        if (month < 10)
          DateString += '0';
        DateString += String(month);
        DateString += " / ";

        if (year < 10)
          DateString += '0';
        DateString += String(year);
      }

      if (gps.time.isValid()) 
      {
        TimeString = "";
        hour = gps.time.hour() + 5; // Adjust UTC
        minute = gps.time.minute();
        second = gps.time.second();

        if (hour < 10)
          TimeString = '0';
        TimeString += String(hour);
        TimeString += " : ";

        if (minute < 10)
          TimeString += '0';
        TimeString += String(minute);
        TimeString += " : ";

        if (second < 10)
          TimeString += '0';
        TimeString += String(second);
      }
    }
  }
  int NumStations = WiFi.softAPgetStationNum();
  Serial.println("Number of connected devices: " + String(NumStations));

  while (NumStations == 0) 
  {
    NumStations = WiFi.softAPgetStationNum(); 
    Serial.println("Number of connected devices in loop: " + String(NumStations));
  }

  Serial.println("Hola");
}
