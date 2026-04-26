#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "QRCodeGenerator.h"

#define SSID     "Vidhya"
#define PASSWORD "9952033054"
#define FIREBASE_HOST "qrgenerator-f5b9e-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "OqCO4NgOED2aR31TfiSeCx8ybzKhDaPwrqAMGEdh"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define TFT_RST   4
#define TFT_CS    5
#define TFT_DC    2

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

String lastQR = "";

void connectWiFi()
{
  WiFi.begin(SSID, PASSWORD);
  Serial.println("WiFi Connecting...");

  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(20, 50);
  tft.print("Connecting...");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nWiFi Connected");
  Serial.println(WiFi.localIP());

  tft.fillScreen(ST77XX_BLACK);
}

void drawQR(String text)
{
  tft.fillScreen(ST77XX_BLACK);

  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(5)];

  qrcode_initText(&qrcode, qrcodeData, 5, 0, text.c_str());

  int scale = 3;
  int qrSize = qrcode.size * scale;

  int offsetX = (160 - qrSize) / 2;
  int offsetY = (128 - qrSize) / 2;

  for (uint8_t y = 0; y < qrcode.size; y++)
  {
    for (uint8_t x = 0; x < qrcode.size; x++)
    {
      if (qrcode_getModule(&qrcode, x, y))
      {
        tft.fillRect(offsetX + x * scale, offsetY + y * scale, scale, scale, ST77XX_WHITE);
      }
    }
  }

  tft.drawRect(offsetX - 4, offsetY - 4, qrSize + 8, qrSize + 8, ST77XX_WHITE);
}

void setup()
{
  Serial.begin(9600);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  connectWiFi();

  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop()
{
  if (Firebase.getString(fbdo, "/Msg"))
  {
    if (fbdo.dataType() == "string")
    {
      String newQR = fbdo.stringData();

      Serial.print("QR Data: ");
      Serial.println(newQR);

      if (newQR != "" && newQR != lastQR)
      {
        drawQR(newQR);
        lastQR = newQR;
      }
    }
  }
  else
  {
    Serial.print("Firebase Error: ");
    Serial.println(fbdo.errorReason());
  }

  delay(2000);
}
