#include <LedControl.h>

#define DIN_PIN 2
#define CS_PIN 3
#define CLK_PIN 4

#define TRIG_PIN 7
#define ECHO_PIN 8

LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);

byte heart[] = {
  B00000000,
  B01100110,
  B11111111,
  B11111111,
  B11111111,
  B01111110,
  B00111100,
  B00011000
};

byte smiley[] = {
 B00000000,
 B01101100,
 B01101100,
 B00000000,
 B11000110,
 B01111100,
 B00111000,
 B00000000,
};

byte sad[] = {
 B00000000,
 B01101100,
 B01101100,
 B00000000,
 B00111000,
 B01111100,
 B11000110,
 B00000000,
};

void setup() 
{
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() 
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  float duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;

  if (distance <= 20) 
  {
    for (int row = 0; row < 8; row++) 
    {
      lc.setRow(0, row, heart[row]);
    }
    delay(500);
  }
  
  else if (distance <= 40) 
  {
    for (int row = 0; row < 8; row++) 
    {
      lc.setRow(0, row, smiley[row]);
    }
    delay(500);
  }
  
  else if (distance <= 60) 
  {
    for (int row = 0; row < 8; row++) 
    {
      lc.setRow(0, row, sad[row]);
    }
    delay(500);
  }
  else if(distance <= )
}
