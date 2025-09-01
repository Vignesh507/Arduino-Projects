#include <WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>

#define SSID       "EIOT"
#define PASS       "GsBt@333"
#define APP_KEY    "813fcf44-e4d1-47a8-ac94-c92adbf052d4"
#define APP_SECRET "285c2b02-edd1-48ca-bdb3-5a76de11fbd0-7a508fad-cbe3-4638-847e-f7d3bcfcd24e"
#define SWITCH_ID  "68b51e31b26fa2c215e4ae22"

int led = 2;

bool value(const String &deviceID, bool &state)
{
  digitalWrite(led, state ? HIGH : LOW);
  Serial.println(state);
  return true;
}

void setupWiFi()
{
  WiFi.begin(SSID, PASS);
  Serial.println("WiFi Connecting.......");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  setupWiFi();

  SinricProSwitch &mySwitch = SinricPro[SWITCH_ID];
  mySwitch.onPowerState(value);
  
  SinricPro.begin(APP_KEY, APP_SECRET);
}

void loop()
{
  SinricPro.handle();
}