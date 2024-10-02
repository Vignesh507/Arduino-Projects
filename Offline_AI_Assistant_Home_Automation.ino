#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3m41f_OOZ"
#define BLYNK_TEMPLATE_NAME "Home Automation"
#define BLYNK_AUTH_TOKEN "xrR3SeSUTZiQgUbCDrDYPQuah7la0cC9"

#include "DFRobot_DF2301Q.h"
#include <IRremote.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define SCREEN_I2C_ADDR 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RST_PIN -1

DFRobot_DF2301Q_I2C asr;

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "POCO M3";
char pass[] = "1883311M3";

int Relay[5] = {15, 4, 19, 18, 2};
int button_Up = 32;
int button_Select = 33;
const int IR_Sensor = 25;

int Up_arrow;
int Select;
int IncrementCounter;

int SelectionCounter1;
int SelectionCounter2;
int SelectionCounter3;
int SelectionCounter4;
int SelectionCounter5;

unsigned long Pervious = 0;
const long interval = 1000;
bool relayState[5] = {false, false, false, false, false};
bool lastState[5] = {false, false, false, false, false};

#define DHTPIN 13

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

IRrecv Ir_Receiver(IR_Sensor);
decode_results results;

TaskHandle_t Task1;
TaskHandle_t Task2;

Adafruit_SSD1306 oled(128, 64, &Wire, OLED_RST_PIN);

void setup() 
{
  Serial.begin(115200); 

  for(int i = 0; i < 5; i++)
  {
    pinMode(Relay[i], OUTPUT);
    digitalWrite(Relay[i], LOW);
  }

  pinMode(button_Up, INPUT_PULLUP);
  pinMode(button_Select, INPUT_PULLUP);
  Ir_Receiver.enableIRIn();

  while (!(asr.begin())) 
  {
    Serial.println("Please check wire connection");
    delay(2000);
  }
  Serial.println("Device connected");
  
  asr.setVolume(7);
  asr.setMuteMode(0);
  asr.setWakeTime(15);
  dht.begin();

  oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDR);
  xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, &Task1, 0);                          
  xTaskCreatePinnedToCore(Task2code, "Task2", 10000, NULL, 1, &Task2, 1);
}

void menu()
{
  Up_arrow = digitalRead(button_Up);
  Select = digitalRead(button_Select);
  
  if(Up_arrow == 0)
  {
    IncrementCounter++;
    Serial.print("Menu Option : ");
    Serial.println(IncrementCounter);
    delay(250);
    if(IncrementCounter == 6)
    {
      IncrementCounter = 0;
    }
  }
  
  switch(IncrementCounter)
  {
    case 1:
    {
     if(Select == 0)
     {
      SelectionCounter1++;
      Serial.print("Selection : ");
      Serial.println(SelectionCounter1);
      delay(250);
      if(SelectionCounter1 == 2)
      {
       SelectionCounter1 = 0;
      }
     }

      if(SelectionCounter1 == 1)
      {
        digitalWrite(Relay[0], HIGH);
      }
      else if(SelectionCounter1 == 0)
      {
        digitalWrite(Relay[0], LOW);
      }
    }
    break;

    case 2:
    {
     if(Select == 0)
     {
      SelectionCounter2++;
      Serial.print("Selection : ");
      Serial.println(SelectionCounter2);
      delay(250);
      if(SelectionCounter2 == 2)
      {
       SelectionCounter2 = 0;
      }
     }

      if(SelectionCounter2 == 1)
      {
        digitalWrite(Relay[1], HIGH);
      }
      else if(SelectionCounter2 == 0)
      {
        digitalWrite(Relay[1], LOW);
      }
    }
    break;

    case 3:
    {
     if(Select == 0)
     {
      SelectionCounter3++;
      Serial.print("Selection : ");
      Serial.println(SelectionCounter3);
      delay(250);
      if(SelectionCounter3 == 2)
      {
       SelectionCounter3 = 0;
      }
     }

      if(SelectionCounter3 == 1)
      {
        digitalWrite(Relay[2], HIGH);
      }
      else if(SelectionCounter3 == 0)
      {
        digitalWrite(Relay[2], LOW);
      }
    }
    break;

    case 4:
    {
     if(Select == 0)
     {
      SelectionCounter4++;
      Serial.print("Selection : ");
      Serial.println(SelectionCounter4);
      delay(250);
      if(SelectionCounter4 == 2)
      {
       SelectionCounter4 = 0;
      }
     }

      if(SelectionCounter4 == 1)
      {
        digitalWrite(Relay[3], HIGH);
      }
      else if(SelectionCounter4 == 0)
      {
        digitalWrite(Relay[3], LOW);
      }
    }
    break;

    case 5:
    {
     if(Select == 0)
     {
      SelectionCounter5++;
      Serial.print("Selection : ");
      Serial.println(SelectionCounter5);
      delay(250);
      if(SelectionCounter5 == 2)
      {
       SelectionCounter5 = 0;
      }
     }

      if(SelectionCounter5 == 1)
      {
        digitalWrite(Relay[4], HIGH);
      }
      else if(SelectionCounter5 == 0)
      {
        digitalWrite(Relay[4], LOW);
      }
    }
    break;  
  }
}

void IRSensor()
{
 if(Ir_Receiver.decode(&results))
  {
    Serial.println(results.value, HEX);
    if(results.value == 0x1767AF71)
    {
      bool currentState1 = !lastState[0];
      lastState[0] = currentState1;
      if(currentState1)
      {
        relayState[0] = !relayState[0];
        SelectionCounter1 = relayState[0];
        digitalWrite(Relay[0], relayState[0] ? HIGH : LOW);
      }
    }
    else if(results.value == 0x318A151)
    {
      bool currentState2 = !lastState[1];
      lastState[1] = currentState2;
      if(currentState2)
      {
        relayState[1] = !relayState[1];
        SelectionCounter2 = relayState[1];
        digitalWrite(Relay[1], relayState[1] ? HIGH : LOW);
      }
    }
    else if(results.value == 0xFCA418D1)
    {
      bool currentState3 = !lastState[2];
      lastState[2] = currentState3;
      if(currentState3)
      {
        relayState[2] = !relayState[2];
        SelectionCounter3 = relayState[2];
        digitalWrite(Relay[2], relayState[2] ? HIGH : LOW);
      }
    }
    else if(results.value == 0xAFD97D5)
    {
      bool currentState4 = !lastState[3];
      lastState[3] = currentState4;
      if(currentState4)
      {
        relayState[3] = !relayState[3];
        SelectionCounter4 = relayState[3];
        digitalWrite(Relay[3], relayState[3] ? HIGH : LOW);
      }
    }
    else if(results.value == 0xE7AF0F51)
    {
      bool currentState4 = !lastState[4];
      lastState[4] = currentState4;
      if(currentState4)
      {
        relayState[4] = !relayState[4];
        SelectionCounter5 = relayState[4];
        digitalWrite(Relay[4], relayState[4] ? HIGH : LOW);
      }
    }
    Ir_Receiver.resume();
  }  
}

void Task1code( void * pvParameters )
{
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;)
  {
  uint8_t CMDID = asr.getCMDID();
  switch(CMDID)
  {
    case 5:
    digitalWrite(Relay[0], HIGH);
    Serial.println("Relay 1 ON");
    SelectionCounter1 = 1;
    break;

    case 6:
    digitalWrite(Relay[0], LOW);
    Serial.println("Relay 1 OFF");
    SelectionCounter1 = 0;
    break;

    case 7:
    digitalWrite(Relay[1], HIGH);
    Serial.println("Relay 2 ON");
    SelectionCounter2 = 1;
    break;

    case 8:
    digitalWrite(Relay[1], LOW);
    Serial.println("Relay 2 OFF");
    SelectionCounter2 = 0;
    break;

    case 9:
    digitalWrite(Relay[2], HIGH);
    Serial.println("Relay 3 ON");
    SelectionCounter3 = 1;
    break;

    case 10:
    digitalWrite(Relay[2], LOW);
    Serial.println("Relay 3 OFF");
    SelectionCounter3 = 0;
    break;

    case 11:
    digitalWrite(Relay[3], HIGH);
    Serial.println("Relay 4 ON");
    SelectionCounter4 = 1;
    break;

    case 12:
    digitalWrite(Relay[3], LOW);
    Serial.println("Relay 4 OFF");
    SelectionCounter4 = 0;
    break;

    case 13:
    digitalWrite(Relay[4], HIGH);
    Serial.println("Relay 5 ON");
    SelectionCounter5 = 1;
    break;

    case 14:
    digitalWrite(Relay[4], LOW);
    Serial.println("Relay 5 OFF");
    SelectionCounter5 = 0;
    break;    

    default:
    if (CMDID != 0) 
    {
      Serial.print("CMDID = ");  
      Serial.println(CMDID);
    }
  }
  delay(200);
  } 
}

void Task2code( void * pvParameters )
{
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;)
  {
    menu();
    IRSensor();
    DisplayMenu();
    //blynk();
  }
}

void loop() 
{
 
}

