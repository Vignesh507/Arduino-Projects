#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// WiFi
#define WIFI_SSID "YOUR_WIFI"
#define WIFI_PASS "YOUR_PASSWORD"

// Adafruit IO
#define AIO_SERVER "io.adafruit.com"
#define AIO_PORT   1883
#define AIO_USER   "YOUR_USERNAME"
#define AIO_KEY    "YOUR_AIO_KEY"

// LEDs
#define LED1 26
#define LED2 27

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_PORT, AIO_USER, AIO_KEY);

// Subscriptions
Adafruit_MQTT_Subscribe led1Feed =
  Adafruit_MQTT_Subscribe(&mqtt, AIO_USER "/feeds/led1");

Adafruit_MQTT_Subscribe led2Feed =
  Adafruit_MQTT_Subscribe(&mqtt, AIO_USER "/feeds/led2");

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

void connectMQTT() {
  while (!mqtt.connected()) {
    if (mqtt.connect() == 0) {
      mqtt.subscribe(&led1Feed);
      mqtt.subscribe(&led2Feed);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  connectWiFi();
}

void loop() {
  connectMQTT();

  mqtt.readSubscription(10);

  digitalWrite(LED1, atoi((char*)led1Feed.lastread) ? HIGH : LOW);
  digitalWrite(LED2, atoi((char*)led2Feed.lastread) ? HIGH : LOW);

  delay(500);
}
