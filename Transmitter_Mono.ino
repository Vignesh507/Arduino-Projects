#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <MPU6050.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_I2C_ADDR 0x3C
Adafruit_SSD1306 display(128, 64, &Wire, -1);

RF24 radio(9, 10);
const byte address[6] = "00001"; 

int axis_x = A7;
int axis_y = A6;
int button = 2;
int buzz   = 3;
int select = 4;
int BatteryIndicator = A5;

const int TextSize   = 1;
int DisplayColumn    = 10;
int DisplayFirstRow  = 5;
int DisplaySecondRow = 18;
int DisplayThirdRow  = 33;
int DisplayFourthRow = 48;

String info[4] = {"Joystick Mode", "Gyroscope Mode", "Setting"};

struct joystick
{
  int x;
  int y;
  int SW;
  int SelectButton;
  int speedCounter;
};

joystick joystickData;
joystick control;

int counter, selectionCounter, batteryStatus;
float voltage;
bool buttonPressed = false;
bool selectionButton = false;

int xPos;
int yPos;
int16_t ax, ay, az, gx, gy, gz;

void startSound()
{
  for(int i = 0; i < 3; i++)
  {
    digitalWrite(buzz, HIGH);
    delay(50);
    digitalWrite(buzz, LOW);
    delay(30);
  }
}

void Selection_Sound()
{
  if(joystickData.SW == LOW && !buttonPressed)
  {
    buttonPressed = false;
    digitalWrite(buzz, HIGH);
    delay(30);
    digitalWrite(buzz, LOW);
  }

  if(joystickData.SW == HIGH)
  {
    buttonPressed == true;
  }
}

void setup() 
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address); 
  //radio.setPALevel(RF24_PA_LOW); 
  radio.stopListening();  

  pinMode(axis_x, INPUT);
  pinMode(axis_y, INPUT);
  pinMode(buzz, OUTPUT);
  pinMode(button, INPUT_PULLUP); 
  pinMode(select, INPUT_PULLUP);  

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDR); 
  
  startSound();

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(15, 7);  
  display.print("CodeWire");
  display.display();
  delay(1000);
  display.clearDisplay();
}

void loop() 
{
  menu();
  //batteryLevel();

}

void menu()
{
  joystickData.SW = digitalRead(button);
  joystickData.SelectButton = digitalRead(select);

  if(joystickData.SW == 0)
  {
    counter++;
    Serial.print("Menu option : ");
    Serial.println(counter);
    delay(250);
    if(counter == 3)
    {
      counter = 0;
    }
  }

  if(joystickData.SelectButton == 0)
  {
    selectionCounter++;
    Serial.println(selectionCounter);
    delay(250);
    if(selectionCounter == 3)
    {
      selectionCounter = 0;
    }
  }

  switch(counter)
  {
   case 0:
   {
    Selection_Sound();    
    display.clearDisplay(); 
    display.setTextSize(TextSize);
    display.setTextColor(WHITE);
    display.setCursor(DisplayColumn, DisplayFirstRow);
    display.println("------ Menu ------");
    display.setCursor(DisplayColumn, DisplaySecondRow);
    display.println("> " + info[0]);
    display.setCursor(DisplayColumn, DisplayThirdRow);
    display.println(" " + info[1]);
    display.setCursor(DisplayColumn, DisplayFourthRow);
    display.println(" " + info[2]);
    display.display();    
    joystickControl(); 
    break;
   }
   case 1:
   {
    Selection_Sound();    
    display.clearDisplay(); 
    display.setTextSize(TextSize);
    display.setTextColor(WHITE);
    display.setCursor(DisplayColumn, DisplayFirstRow);
    display.println("------ Menu ------");
    display.setCursor(DisplayColumn, DisplaySecondRow);
    display.println(" " + info[0]);
    display.setCursor(DisplayColumn, DisplayThirdRow);
    display.println("> " + info[1]);
    display.setCursor(DisplayColumn, DisplayFourthRow);
    display.println(" " + info[2]);
    display.display();    
    break;
   }   
   case 2:
   {
    Selection_Sound();    
    display.clearDisplay(); 
    display.setTextSize(TextSize);
    display.setTextColor(WHITE);
    display.setCursor(DisplayColumn, DisplayFirstRow);
    display.println("------ Menu ------");
    display.setCursor(DisplayColumn, DisplaySecondRow);
    display.println(" " + info[0]);
    display.setCursor(DisplayColumn, DisplayThirdRow);
    display.println(" " + info[1]);
    display.setCursor(DisplayColumn, DisplayFourthRow);
    display.println("> " + info[2]);
    display.display();  

    break;
   }      
  }
}

void joystickControl()
{
  while(selectionCounter == 1)
  {
    joystickData.y = analogRead(axis_x);
    joystickData.x = analogRead(axis_y);
    control.SW     = digitalRead(button);
    int exit       = digitalRead(select);

    Serial.print("Joystick X : ");
    Serial.print(joystickData.x);
    Serial.print(" | Y : ");
    Serial.println(joystickData.y);
 
    radio.write(&joystickData, sizeof(joystick));     

    display.clearDisplay();
    display.setTextSize(TextSize);
    display.setTextColor(WHITE);
    display.setCursor(DisplayColumn, DisplayFirstRow);
    display.println("---- Joystick ----");
    display.setCursor(DisplayColumn, 20);
    display.println("X Axis : ");  
    display.setTextSize(1); 
    display.setCursor(60, 20);   
    display.println(joystickData.x);    
    display.setCursor(DisplayColumn, 35);
    display.println("Y Axis : ");  
    display.setTextSize(1); 
    display.setCursor(60, 35);   
    display.println(joystickData.y);   
    graphAnimation();
    enable();     
    display.display();       

    delay(150);

    if(exit == 0)
    {
      selectionCounter = 0;
    }  
  }
}

void gyroscopeControl()
{
  while(selectionCounter == 1)
  {
    if(exit == 0)
    {
      selectionCounter = 0;
    }  
  }
}

void setting()
{
  while(selectionCounter == 1)
  {
    int exit = digitalRead(select);
    display.clearDisplay();
    display.setTextSize(TextSize);
    display.setTextColor(WHITE);
    display.setCursor(DisplayColumn, DisplayFirstRow);
    display.println("---- Setting ----");
    display.display();       
    delay(150);
    if(exit == 0)
    {
      selectionCounter = 0;
    }  
  }
}

void graphAnimation()
{
  xPos = map(joystickData.x, 1023, 0, 95, 120);
  yPos = map(joystickData.y, 0, 1023, 20, 60);
  display.drawLine(88, 18, 88, 60, WHITE);
  display.fillCircle(xPos, yPos, 4, WHITE);
  display.display();
}

void enable()
{
  int pushButton = digitalRead(button);
  if(pushButton == 0)
  {
    joystickData.speedCounter++;
    if(joystickData.speedCounter == 4)
    {
      Selection_Sound();
      joystickData.speedCounter = 0;
    }
  }
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(10, 50);
  display.print("Speed  : ");
  display.print(joystickData.speedCounter);
}

int batteryLevel()
{
  batteryStatus = analogRead(BatteryIndicator);
  voltage = batteryStatus * (4.2 / 1050.0);
  Serial.println(voltage);
  delay(500);
 
  display.clearDisplay();
  display.setTextSize(TextSize);
  display.setTextColor(WHITE);
  display.setCursor(DisplayColumn, DisplayFirstRow);
  display.println(voltage);  
  display.display();

  if(voltage < 3.5)
  {
    digitalWrite(buzz, HIGH);
    delay(100);
    digitalWrite(buzz, LOW);
    delay(50);
  }
}