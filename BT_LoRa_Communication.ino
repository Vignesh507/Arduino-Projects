#include <Wire.h>
#include <U8g2lib.h>
#include "BluetoothSerial.h"
#include <Preferences.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled!
#endif

BluetoothSerial SerialBT;
Preferences preferences;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0);

#define LORA_BAUD 9600
#define Buzz 18
#define SW1  23
#define SW2  5

String menuPage2[3] = { "Node Select", "Message", "Exit" };
String nodeList[4]  = { "Node A", "Node B", "Node C", "Node D" };

bool previousState = false;
String BTStatus = "DISC";
String location = "13.212536, 80.147043";

int bluetoothRSSI = -45;

String selectedNode = "Node A";

uint8_t counter = 0;
uint8_t nodeCounter = 0;

bool currentPage = 0;
bool messagePage = false;
bool nodePage    = false;

unsigned long lastClick = 0;

bool emergencyBuzzer = false;
unsigned long buzzerStartTime = 0;
static unsigned long lastLoRaTime = 0;
String loraLink = "Wait";
String rssidBm = "-20";

String chatLine1 = "";
String chatLine2 = "";
String chatLine3 = "";
String chatLine4 = "";

void loadingScreen()
{
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB14_tr);
  display.drawStr(5, 35, "LoRa Bridge");
  display.sendBuffer();
  delay(1500);

  for(int i = 0; i <= 100; i += 10)
  {
    display.clearBuffer();
    display.setFont(u8g2_font_6x12_tf);
    display.drawStr(20, 15, "Starting System");
    display.drawFrame(10, 30, 108, 12);
    display.drawBox(12, 32, i, 8);
    display.setCursor(48, 55);
    display.print(i);
    display.print("%");
    display.sendBuffer();
    delay(150);
  }

  display.clearBuffer();
  display.drawFrame(5, 15, 118, 30);
  display.setFont(u8g2_font_ncenB10_tr);
  display.drawStr(20, 35, "Node Ready");
  display.sendBuffer();
  tone(Buzz, 2000, 200);
  delay(1000);
}

void StatusPage()
{
  display.clearBuffer();
  display.setFont(u8g2_font_6x12_tf);
  display.drawFrame(0, 0, 128, 64);

  display.drawStr(5, 14, "Bluetooth  :");
  display.setCursor(85, 14);
  display.print(BTStatus);

  display.drawStr(5, 28, "LoRa Link  :");
  display.setCursor(85, 28);
  display.print(loraLink);

  display.drawStr(5, 42, "LoRa RSSI  :");
  display.setCursor(85, 42);
  display.print(rssidBm);

  display.drawStr(5, 56, "Node ID    :");
  display.setCursor(85, 56);
  display.print(selectedNode);

  display.sendBuffer();
}

void MenuPage()
{
  uint8_t navigate = digitalRead(SW1);
  if(navigate == LOW)
  {
    counter++;
    if(counter >= 3)
    {
      counter = 0;
    }
    tone(Buzz, 1800, 50);
    delay(250);
  }

  display.clearBuffer();
  display.setFont(u8g2_font_7x14_tf);
  display.drawFrame(0, 0, 128, 64);
  display.drawStr(50, 15, "MENU");
  display.drawLine(0, 18, 128, 18);

  for(int i = 0; i < 3; i++)
  {
    if(i == counter)
    {
      display.drawBox(3, (i * 13) + 22, 122, 12);
      display.setDrawColor(0);
      display.setCursor(8, (i * 13) + 32);
      display.print(menuPage2[i]);
      display.setDrawColor(1);
    }
    else
    {
      display.setCursor(8, (i * 13) + 32);
      display.print(menuPage2[i]);
    }
  }

  display.sendBuffer();

  uint8_t selectBtn = digitalRead(SW2);

  if(selectBtn == LOW)
  {
    tone(Buzz, 2500, 100);
    if(counter == 0)
    {
      nodePage = true;
    }
    if(counter == 1)
    {
      messagePage = true;
    }
    if(counter == 2)
    {
      currentPage = 0;
    }
    delay(300);
  }
}

void NodePage()
{
  uint8_t navigate = digitalRead(SW1);
  if(navigate == LOW)
  {
    nodeCounter++;

    if(nodeCounter >= 4)
    {
      nodeCounter = 0;
    }

    tone(Buzz, 1800, 50);

    delay(250);
  }

  display.clearBuffer();
  display.setFont(u8g2_font_6x12_tf);
  display.drawFrame(0, 0, 128, 64);
  display.drawStr(28, 10, "NODE SELECT");
  display.drawLine(0, 12, 128, 12);

  for(int i = 0; i < 4; i++)
  {
    if(i == nodeCounter)
    {
      display.drawBox(2, (i * 12) + 12, 124, 12);
      display.setDrawColor(0);
      display.setCursor(10, (i * 12) + 22);
      display.print(nodeList[i]);
      display.setDrawColor(1);
    }
    else
    {
      display.setCursor(10, (i * 12) + 22);
      display.print(nodeList[i]);
    }
  }

  display.sendBuffer();

  if(digitalRead(SW2) == LOW)
  {
    selectedNode = nodeList[nodeCounter];
    preferences.putString("NodeID", selectedNode);
    SerialBT.end();
    delay(200);

    SerialBT.begin(selectedNode);
    Serial.print("Saved Node : ");
    Serial.println(selectedNode);
    tone(Buzz, 2500, 100);
    nodePage = false;
    delay(300);
  }
}

void MessagePage()
{
  display.clearBuffer();
  display.setFont(u8g2_font_5x8_tf);
  display.drawFrame(0, 0, 128, 64);
  display.drawBox(0, 0, 128, 10);
  display.setDrawColor(0);
  display.drawStr(52, 8, "CHAT");
  display.setDrawColor(1);

  display.drawStr(2, 20, chatLine1.c_str());
  display.drawStr(2, 32, chatLine2.c_str());
  display.drawStr(2, 44, chatLine3.c_str());
  display.drawStr(2, 56, chatLine4.c_str());
  display.sendBuffer();

  if(digitalRead(SW2) == LOW)
  {
    delay(300);
    if(digitalRead(SW2) == LOW)
    {
      messagePage = false;
      tone(Buzz, 1000, 100);
      delay(300);
    }
  }
}

void buttonControl()
{
  uint8_t selectBtn = digitalRead(SW2);

  if(selectBtn == LOW && currentPage == 0)
  {
    unsigned long currentTime = millis();
    if(currentTime - lastClick < 400)
    {
      currentPage = 0;
      tone(Buzz, 1000, 100);
      delay(300);
    }
    else
    {
      currentPage = 1;
      tone(Buzz, 2500, 100);
      delay(300);
    }
    lastClick = currentTime;
  }
}

void setup()
{
  Serial.begin(115200);
  preferences.begin("NodeData", false);
  selectedNode = preferences.getString("NodeID", "Node A");
  SerialBT.begin(selectedNode);
  Serial2.begin(LORA_BAUD, SERIAL_8N1, 16, 17);

  display.begin();

  pinMode(Buzz, OUTPUT);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);

  loadingScreen();
}

void loop()
{

  bool currentState = SerialBT.hasClient();
  if(currentState == true && previousState == false)
  {
    BTStatus = "CONN";
    tone(Buzz, 2000, 100);
  }

  if(currentState == false && previousState == true)
  {
    BTStatus = "DISC";
    tone(Buzz, 1000, 100);
  }

  previousState = currentState;

  if(SerialBT.available())
  {
    while(SerialBT.available())
    {
      String data = SerialBT.readStringUntil('\n');
      data.trim();

      String sendData = selectedNode + " : " + data;

      chatLine1 = chatLine2;
      chatLine2 = chatLine3;
      chatLine3 = chatLine4;
      chatLine4 = "TX: " + sendData;

      if(data == "201")
      {
        Serial2.println(location);
      }
      if(data == "-202")
      {
        noTone(Buzz);
        emergencyBuzzer = false;
      }      
      else
      {
        Serial2.println(sendData);
      }
      Serial.println(sendData);
    }
  }

  if(Serial2.available())
  {
    while(Serial2.available())
    {
      String data = Serial2.readStringUntil('\n');
      data.trim();
      
      chatLine1 = chatLine2;
      chatLine2 = chatLine3;
      chatLine3 = chatLine4;
      chatLine4 = "RX: " + data;

      SerialBT.println(data);
      Serial.println(data);
      tone(Buzz, 1500, 100);

      lastLoRaTime = millis();
      loraLink = "Active";

      if(data.indexOf("202") >= 0)
      {
       emergencyBuzzer = true;
       buzzerStartTime = millis();
       Serial.println("Emergency Alert Received");
      }
    }
  }
  buttonControl();

  if(millis() - lastLoRaTime > 5000)
  {
   loraLink = "Wait";
  }

  if(nodePage == true)
  {
    NodePage();
  }
  else if(messagePage == true)
  {
    MessagePage();
  }
  else
  {
    if(currentPage == 0)
    {
      StatusPage();
    }
    else
    {
      MenuPage();
    }
  }

  if(emergencyBuzzer == true)
  {
   static unsigned long beepTimer = 0;
   static bool buzState = false;

   if(millis() - beepTimer >= 100)
   {
    beepTimer = millis();
    buzState = !buzState;

    if(buzState)
    {
      tone(Buzz, 5000);
    }
    else
    {
      noTone(Buzz);
    }
   } 
   if(millis() - buzzerStartTime >= 60000)
   {
    noTone(Buzz);
    emergencyBuzzer = false;
   }
  }
  delay(10);
}
