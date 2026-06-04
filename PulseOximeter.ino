#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS 1000

PulseOximeter pox;
uint32_t tsLastReport = 0;

void onBeatDetected()
{
  Serial.println("Beat Detected!");
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("MAX30100 Heart Rate & SpO2 Monitor");

  Wire.begin(6, 7);      
  Wire.setClock(100000);

  if (!pox.begin())
  {
    Serial.println("MAX30100 Initialization FAILED");
    while (1);
  }

  Serial.println("MAX30100 Initialization SUCCESS");

  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
  pox.update();

  if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {
    float hr = pox.getHeartRate();
    float spo2 = pox.getSpO2();

    Serial.println("--------------------------------");

    Serial.print("Heart Rate : ");
    Serial.print(hr);
    Serial.println(" BPM");

    Serial.print("SpO2       : ");
    Serial.print(spo2);
    Serial.println(" %");

    if (hr > 0)
    {
      if (hr < 60)
        Serial.println("Status     : Bradycardia");
      else if (hr > 100)
        Serial.println("Status     : Tachycardia");
      else
        Serial.println("Status     : Normal");
    }
    else
    {
      Serial.println("Status     : Place Finger Properly");
    }

    tsLastReport = millis();
  }
}
