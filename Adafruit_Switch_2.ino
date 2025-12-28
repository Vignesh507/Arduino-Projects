#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/* ================= USER CONFIG ================= */

// WiFi
#define WIFI_SSID   "YOUR_WIFI_NAME"
#define WIFI_PASS   "YOUR_WIFI_PASSWORD"

// Adafruit IO
#define AIO_SERVER  "io.adafruit.com"
#define AIO_PORT    1883
#define AIO_USER    "YOUR_ADAFRUIT_USERNAME"
#define AIO_KEY     "YOUR_ADAFRUIT_IO_KEY"

// GPIOs
#define LED1_PIN 26
#define LED2_PIN 27

/* =============================================== */

// Network clients
WiFiClient wifiClient;
Adafruit_MQTT_Client mqtt(&wifiClient, AIO_SERVER, AIO_PORT, AIO_USER, AIO_KEY);

// MQTT subscriptions
Adafruit_MQTT_Subscribe led1Feed =
  Adafruit_MQTT_Subscribe(&mqtt, AIO_USER "/feeds/led1");

Adafruit_MQTT_Subscribe led2Feed =
  Adafruit_MQTT_Subscribe(&mqtt, AIO_USER "/feeds/led2");

/* ================= FUNCTIONS ================= */

void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected");
}

void connectMQTT() {
  if (mqtt.connected()) return;

  Serial.print("Connecting to Adafruit IO");
  while (mqtt.connect() != 0) {
    Serial.print(".");
    delay(3000);
  }
  Serial.println(" Connected");

  mqtt.subscribe(&led1Feed);
  mqtt.subscribe(&led2Feed);
}

/* ================= SETUP ================= */

void setup() {
  Serial.begin(115200);

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  connectWiFi();
}

/* ================= LOOP ================= */

void loop() {
  connectWiFi();
  connectMQTT();

  Adafruit_MQTT_Subscribe *subscription;

  while ((subscription = mqtt.readSubscription(10))) {

    if (subscription == &led1Feed) {
      int state = atoi((char *)led1Feed.lastread);
      digitalWrite(LED1_PIN, state ? HIGH : LOW);
      Serial.println(state ? "LED1 ON" : "LED1 OFF");
    }

    else if (subscription == &led2Feed) {
      int state = atoi((char *)led2Feed.lastread);
      digitalWrite(LED2_PIN, state ? HIGH : LOW);
      Serial.println(state ? "LED2 ON" : "LED2 OFF");
    }
  }
}
