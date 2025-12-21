#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// ---------- WiFi ----------
#define WIFI_SSID     "Vidhya"
#define WIFI_PASS     "9952033054"

// ---------- Adafruit IO ----------
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "_Vignesh_"
#define AIO_KEY         ""

// ---------- Gas Sensor ----------
#define GAS_PIN 34   // ESP32 ADC pin (GPIO 34)

// WiFi client
WiFiClient client;

// MQTT client
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// 🔹 FEED NAME CONFIRMED AS "gas"
Adafruit_MQTT_Publish gasFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/gas");

// ---------- WiFi connect ----------
void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

// ---------- MQTT connect ----------
void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Connecting to Adafruit IO...");
    if (mqtt.connect() == 0) {
      Serial.println(" Connected!");
    } else {
      Serial.println(" Failed, retrying...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  connectWiFi();
}

void loop() {
  connectMQTT();

  int gasValue = analogRead(GAS_PIN);  // 0–4095

  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  // Publish to Adafruit IO
  gasFeed.publish((int32_t)gasValue);

  delay(1000);  // send every 5 seconds
}
