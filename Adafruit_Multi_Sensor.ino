#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// ---------- WiFi ----------
#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASS "YOUR_WIFI_PASSWORD"

// ---------- Adafruit IO ----------
#define AIO_SERVER     "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME   "YOUR_ADAFRUIT_USERNAME"
#define AIO_KEY        "YOUR_ADAFRUIT_IO_KEY"

// ---------- Pins ----------
#define GAS1_PIN   34
#define GAS2_PIN   35
#define RELAY_PIN  26

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT,
                          AIO_USERNAME, AIO_KEY);

// ---------- Feeds ----------
Adafruit_MQTT_Publish gas1Feed =
  Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/gas1");

Adafruit_MQTT_Publish gas2Feed =
  Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/gas2");

Adafruit_MQTT_Subscribe switchFeed =
  Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/switch");

// ---------- WiFi ----------
void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
}

// ---------- MQTT ----------
void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("Connecting to Adafruit IO...");
    if (mqtt.connect() == 0) {
      Serial.println(" Connected");
      mqtt.subscribe(&switchFeed);   // subscribe switch
    } else {
      Serial.println(" Failed, retrying...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // OFF initially
  connectWiFi();
}

void loop() {
  connectMQTT();

  // -------- Read Gas Sensors --------
  int gas1 = analogRead(GAS1_PIN);
  int gas2 = analogRead(GAS2_PIN);

  gas1Feed.publish((int32_t)gas1);
  gas2Feed.publish((int32_t)gas2);

  // -------- Read Switch --------
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(10))) {
    if (subscription == &switchFeed) {
      int switchState = atoi((char *)switchFeed.lastread);

      if (switchState == 1) {
        digitalWrite(RELAY_PIN, HIGH);
        Serial.println("Switch ON");
      } else {
        digitalWrite(RELAY_PIN, LOW);
        Serial.println("Switch OFF");
      }
    }
  }

  delay(3000);
}
