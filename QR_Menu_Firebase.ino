#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <Preferences.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "QRCodeGenerator.h"

#define ST77XX_DARKGREY 0x7BEF

#define SSID     "Vidhya"
#define PASSWORD "9952033054"
#define FIREBASE_HOST "qrgenerator-f5b9e-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "OqCO4NgOED2aR31TfiSeCx8ybzKhDaPwrqAMGEdh"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
Preferences prefs;

#define TFT_RST 4
#define TFT_CS  5
#define TFT_DC  2

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define BTN_UP    25
#define BTN_DOWN  26
#define BTN_SEL   27
#define BTN_BACK  14

int currentScreen = 0;
int selectedIndex = 0;

#define MENU_COUNT 4
String menuName[MENU_COUNT];
String menuData[MENU_COUNT];

unsigned long lastPress = 0;
int debounceDelay = 200;

String lastQRData = "";
bool isOnline = false;

void loadOfflineMenu() 
{
  prefs.begin("menu", true);

  for (int i = 0; i < MENU_COUNT; i++) 
  {
    menuName[i] = prefs.getString(("name" + String(i)).c_str(), "Menu");
    menuData[i] = prefs.getString(("data" + String(i)).c_str(), "No Data");
  }
  prefs.end();
}

void saveMenuOffline() 
{
  prefs.begin("menu", false);

  for (int i = 0; i < MENU_COUNT; i++) 
  {
    prefs.putString(("name" + String(i)).c_str(), menuName[i]);
    prefs.putString(("data" + String(i)).c_str(), menuData[i]);
  }
  prefs.end();
}

void connectWiFi() 
{
  WiFi.begin(SSID, PASSWORD);

  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setCursor(20, 60);
  tft.print("Connecting....");

  int timeout = 0;

  while(WiFi.status() != WL_CONNECTED && timeout < 20) 
  {
    delay(500);
    timeout++;
  }
  if (WiFi.status() == WL_CONNECTED) 
  {
    isOnline = true;
  } 
  else 
  {
    isOnline = false;
    loadOfflineMenu();
  }  

  tft.fillScreen(ST77XX_BLACK);
}

void loadMenuOnline() 
{
  for (int i = 0; i < MENU_COUNT; i++) 
  {
    String pathName = "/menu/" + String(i + 1) + "/name";
    String pathData = "/menu/" + String(i + 1) + "/data";

    if (Firebase.getString(fbdo, pathName))
      menuName[i] = fbdo.stringData();

    if (Firebase.getString(fbdo, pathData))
      menuData[i] = fbdo.stringData();
  }

  saveMenuOffline(); 
}

void drawMenu() 
{
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextSize(1);

  for (int i = 0; i < MENU_COUNT; i++) 
  {
    int y = 12 + (i * 25);

    if (i == selectedIndex) 
    {
      tft.fillRoundRect(8, y - 5, 144, 22, 5, ST77XX_YELLOW);
      tft.setTextColor(ST77XX_BLACK);
    } 
    else 
    {
      tft.drawRoundRect(8, y - 5, 144, 22, 5, ST77XX_WHITE);
      tft.setTextColor(ST77XX_WHITE);
    }

    int16_t x1, y1;
    uint16_t w, h;

    tft.getTextBounds(menuName[i], 0, 0, &x1, &y1, &w, &h);

    int textX = (160 - w) / 2;
    int textY = y + (h / 3);

    tft.setCursor(textX, textY);
    tft.print(menuName[i]);
  }

  tft.fillRect(0, 110, 160, 18, ST77XX_DARKGREY);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, 115);
  tft.print("UP/DOWN   SELECT   BACK");
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

  tft.fillRoundRect(offsetX - 6, offsetY - 6, qrSize + 12, qrSize + 12, 6, ST77XX_WHITE);

  for (uint8_t y = 0; y < qrcode.size; y++) 
  {
    for (uint8_t x = 0; x < qrcode.size; x++) 
    {
      if (qrcode_getModule(&qrcode, x, y)) 
      {
        tft.fillRect(offsetX + x * scale, offsetY + y * scale, scale, scale, ST77XX_BLACK);
      }
    }
  }
}

void drawSettings() 
{
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(1);

  tft.drawRoundRect(10, 40, 140, 60, 6, ST77XX_WHITE);

  tft.setTextColor(ST77XX_GREEN);
  tft.setCursor(25, 55);
  tft.print(isOnline ? "Online" : "Offline");

  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(25, 80);
  tft.print(WiFi.localIP());
}

void setup() 
{
  Serial.begin(9600);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SEL, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  connectWiFi();
  drawSettings();

  if(isOnline) 
  {
    config.host = FIREBASE_HOST;
    config.signer.tokens.legacy_token = FIREBASE_AUTH;

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    loadMenuOnline();
  }
  drawMenu();
}

void loop() 
{
  static unsigned long lastUpdate = 0;

  if(millis() - lastUpdate > 5000 && isOnline) 
  {
    loadMenuOnline();

    if(currentScreen == 0) 
    {
      drawMenu();
    }

    else if(currentScreen == 1) 
    {
      String newData = menuData[selectedIndex];

      if (newData != lastQRData) 
      {
        drawQR(newData);
        lastQRData = newData;
      }
    }

    lastUpdate = millis();
  }

  if(millis() - lastPress < debounceDelay) return;

  if(currentScreen == 0) 
  {

    if(digitalRead(BTN_UP) == LOW) 
    {
      selectedIndex--;
      if (selectedIndex < 0) selectedIndex = MENU_COUNT - 1;
      drawMenu();
      lastPress = millis();
    }

    if(digitalRead(BTN_DOWN) == LOW) 
    {
      selectedIndex++;
      if (selectedIndex >= MENU_COUNT) selectedIndex = 0;
      drawMenu();
      lastPress = millis();
    }

    if(digitalRead(BTN_SEL) == LOW) 
    {
      if(menuData[selectedIndex] == "settings") 
      {
        drawSettings();
        currentScreen = 2;
      } 
      else
      {
        drawQR(menuData[selectedIndex]);
        lastQRData = menuData[selectedIndex];
        currentScreen = 1;
      }

      lastPress = millis();
    }
  }

  else if(currentScreen == 1) 
  {
    if(digitalRead(BTN_BACK) == LOW) 
    {
      drawMenu();
      currentScreen = 0;
      lastPress = millis();
    }
  }

  else if(currentScreen == 2) 
  {
    if(digitalRead(BTN_BACK) == LOW) 
    {
      drawMenu();
      currentScreen = 0;
      lastPress = millis();
    }
  }
}
