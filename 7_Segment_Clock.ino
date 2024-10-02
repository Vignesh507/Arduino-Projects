#include "SevSeg.h"
#include <DS1307RTC.h>
SevSeg Segment;
const int secPin =  13;
unsigned int number;
unsigned long currentMillis;
unsigned int Hour;
int ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 500;
void setup()
{
  pinMode(secPin, OUTPUT);
  byte numDigits = 4;
  byte digitPins[] = {5, 4, 3, 2};
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, A0 };
  byte dispType = COMMON_ANODE;
  Segment.begin(dispType, numDigits, digitPins, segmentPins);
  Segment.setBrightness(90);
}
void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(secPin, ledState);
  }
  tmElements_t tm;
  if (RTC.read(tm))
  {
    Hour = tm.Hour;
    if (Hour > 12)
    {
      Hour = Hour - 12;
    }
    else
    {
      if (Hour == 0) Hour = 12;
    }
  }
  number = Hour * 100 + tm.Minute;
  Segment.setNumber(number);
  Segment.refreshDisplay();
}
