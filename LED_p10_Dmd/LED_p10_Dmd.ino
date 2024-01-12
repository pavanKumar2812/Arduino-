#include <DMD.h>
#include <TimerOne.h>
#include "SystemFont5x7.h"
#include "Arial_black_16.h"
#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include <TimeLib.h>

RtcDateTime targetDate = RtcDateTime(2023, 11, 5, 12, 0, 0);

ThreeWire myWire(5,4,2); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

#define DISPLAYS_ACROSS 2
#define DISPLAYS_DOWN 1
String str;
String timeStr1;
String timeStr2;
String timeStr3;
String timeStr4;
char b[30];
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

unsigned long previousMillis = 0;        // Will store last time LCD was updated

// constants won't change:
const long updateInterval = 1000;           // interval at which to update LCD (milliseconds)

void ScanDMD(){
  dmd.scanDisplayBySPI();
}

time_t tmConvert_t(int YYYY, byte MM, byte DD, byte hh, byte mm, byte ss)
{
  tmElements_t tmSet;
  tmSet.Year = YYYY - 1970;
  tmSet.Month = MM;
  tmSet.Day = DD;
  tmSet.Hour = hh;
  tmSet.Minute = mm;
  tmSet.Second = ss;
  return makeTime(tmSet); 
}

void setup() {
  Serial.begin(9600);
  Timer1.initialize(5000);
  Timer1.attachInterrupt(ScanDMD);
  dmd.clearScreen(true);
}

void loop() {
  unsigned long currentMillis = millis();

  // Retrieve the current time
  RtcDateTime now = Rtc.GetDateTime();

  if (currentMillis - previousMillis >= updateInterval) {
    // Save the last time you updated the LCD
    previousMillis = currentMillis;

    // Calculate the time difference manually
    long targetUnixTime = targetDate.TotalSeconds();
    long nowUnixTime = now.TotalSeconds();
    long timeDiff = targetUnixTime - nowUnixTime;

    // Extract days, hours, minutes, and seconds from the time difference
    int daysLeft = timeDiff / 86400; // 86400 seconds in a day
    int hoursLeft = (timeDiff % 86400) / 3600; // 3600 seconds in an hour
    int minutesLeft = (timeDiff % 3600) / 60; // 60 seconds in a minute
    int secondsLeft = timeDiff % 60;

    // Create a formatted string for the countdown timer
    timeStr1 = String(daysLeft) ;
    timeStr2 = String(hoursLeft);
    timeStr3 = String(minutesLeft);
    timeStr4 = String(secondsLeft); 
  }

  int slen = 0;
  dmd.selectFont(SystemFont5x7);

  // Display the first row
  String row1 = "Indus";
  slen = row1.length() + 1;
  row1.toCharArray(b, slen);
  dmd.drawString(1, 0, b,strlen(b), GRAPHICS_NORMAL);

  String row12 = "GoLive";
  slen = row12.length() + 1;
  row12.toCharArray(b, slen);
  dmd.drawString(33, 0, b,strlen(b), GRAPHICS_NORMAL);

  // Display the second row (countdown timer)
  slen = timeStr1.length() + 1;
  timeStr1.toCharArray(b, slen);
  dmd.drawString(0, 8, b,strlen(b), GRAPHICS_NORMAL);
  dmd.drawLine(13,9,13, 10, GRAPHICS_NORMAL);
  dmd.drawLine(12,9,12, 10, GRAPHICS_NORMAL);
  dmd.drawLine(13,12,13,13, GRAPHICS_NORMAL);
  dmd.drawLine(12,12,12,13, GRAPHICS_NORMAL);

  slen = timeStr2.length() + 1;
  timeStr2.toCharArray(b, slen);
  dmd.drawString(15, 8, b,strlen(b), GRAPHICS_NORMAL);
  dmd.drawLine(28,9,28, 10, GRAPHICS_NORMAL);
  dmd.drawLine(27,9,27, 10, GRAPHICS_NORMAL);
  dmd.drawLine(28,12,28, 13, GRAPHICS_NORMAL);
  dmd.drawLine(27,12,27, 13, GRAPHICS_NORMAL);

  slen = timeStr3.length() + 1;
  timeStr3.toCharArray(b, slen);
  dmd.drawString(30, 8, b,strlen(b), GRAPHICS_NORMAL);
  dmd.drawLine(43,9,43, 10, GRAPHICS_NORMAL);
  dmd.drawLine(42,9,42, 10, GRAPHICS_NORMAL);
  dmd.drawLine(43,12,43, 13, GRAPHICS_NORMAL);
  dmd.drawLine(42,12,42, 13, GRAPHICS_NORMAL);

  slen = timeStr4.length() + 1;
  timeStr4.toCharArray(b, slen);
  dmd.drawString(45, 8, b,strlen(b), GRAPHICS_NORMAL);
  
}