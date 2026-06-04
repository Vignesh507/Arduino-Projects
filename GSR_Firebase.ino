#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "Vidhya"
#define WIFI_PASSWORD "9952033054"

#define API_KEY "1aN768cYAyRfFvpMhNrDiNjYIvnJ9bqQuxM6rnb2"

#define DATABASE_URL "https://hydration-76081-default-rtdb.asia-southeast1.firebasedatabase.app/"

#define GSR_PIN A0

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

int gsrValue;
float voltage;
String hydrationStatus;

unsigned long sendDataPrevMillis = 0;

void setup()
{
  Serial.begin(115200);

  analogReadResolution(12);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.println(WiFi.localIP());

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("Firebase Ready");
}

void loop()
{
  long sum = 0;

  for (int i = 0; i < 20; i++)
  {
    sum += analogRead(GSR_PIN);
    delay(5);
  }

  gsrValue = sum / 20;

  voltage = (gsrValue * 3.3) / 4095.0;

  if (gsrValue < 1200)
    hydrationStatus = "Severe Dehydration";
  else if (gsrValue < 1800)
    hydrationStatus = "Mild Dehydration";
  else if (gsrValue < 2600)
    hydrationStatus = "Normal Hydration";
  else
    hydrationStatus = "Well Hydrated";

  Serial.print("GSR: ");
  Serial.print(gsrValue);
  Serial.print(" Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" Status: ");
  Serial.println(hydrationStatus);

  if (Firebase.ready() &&
      millis() - sendDataPrevMillis > 5000)
  {
    sendDataPrevMillis = millis();

    Firebase.RTDB.setInt(&fbdo,
                         "/Hydration/GSR",
                         gsrValue);

    Firebase.RTDB.setFloat(&fbdo,
                           "/Hydration/Voltage",
                           voltage);

    Firebase.RTDB.setString(&fbdo,
                            "/Hydration/Status",
                            hydrationStatus);

    Serial.println("Data Uploaded");
  }

  delay(1000);
}
