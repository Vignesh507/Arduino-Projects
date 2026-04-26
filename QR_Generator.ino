#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "QRCodeGenerator.h"

#define TFT_RST   4
#define TFT_CS    5
#define TFT_DC    2

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() 
{
  Serial.begin(115200);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(5)];

  qrcode_initText(&qrcode, qrcodeData, 5, 0, "https://www.youtube.com/watch?v=QQHyo7KlaQk");

  int scale = 3;  
  int offsetX = 25;
  int offsetY = 8;

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

  int qrSize = qrcode.size * scale;
  tft.drawRect(offsetX - 8, offsetY - 8, qrSize + 16, qrSize + 16, ST77XX_WHITE);

  //tft.drawRect(offsetX - 4, offsetY - 4, qrSize + 8, qrSize + 8, ST77XX_WHITE);

 // tft.setTextSize(1);
 // tft.setTextColor(ST77XX_WHITE);
 // tft.setCursor(offsetX, offsetY + qrSize + 8);
  //tft.print("Scan Me");
}

void loop() {}
