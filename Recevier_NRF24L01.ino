#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);
const byte address[6] = "00001";

int motor_1  = 2;
int motor_2  = 3;
int enable_1 = 5;

int motor_3  = 7;
int motor_4  = 8;
int enable_2 = 6;

struct joystick
{
  int y;
  int x;
  int speedCounter;
  float accel_x, accel_y, accel_z;
};

joystick joystickData;

int listRange[4] = {0, 30, 80, 255};

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address); 
  //radio.setPALevel(RF24_PA_LOW); 
  radio.startListening();    

  pinMode(motor_1, OUTPUT);
  pinMode(motor_2, OUTPUT);
  pinMode(motor_3, OUTPUT);
  pinMode(motor_4, OUTPUT);
  pinMode(enable_1, OUTPUT);
  pinMode(enable_2, OUTPUT);     

  digitalWrite(motor_1, LOW);
  digitalWrite(motor_2, LOW);
  digitalWrite(motor_3, LOW);   
  digitalWrite(motor_4, LOW); 
  digitalWrite(enable_1, 0);
  digitalWrite(enable_1, 0);    
}

void forward(int speed)
{
  analogWrite(enable_1, speed);
  digitalWrite(motor_1, HIGH);
  digitalWrite(motor_2, LOW);

  analogWrite(enable_2, speed);
  digitalWrite(motor_3, LOW);
  digitalWrite(motor_4, HIGH);  
}

void reverse(int speed)
{
  analogWrite(enable_1, speed);
  digitalWrite(motor_1, LOW);
  digitalWrite(motor_2, HIGH);

  analogWrite(enable_2, speed);
  digitalWrite(motor_3, HIGH);
  digitalWrite(motor_4, LOW); 
}

void right(int speed)
{
  analogWrite(enable_1, speed);
  digitalWrite(motor_1, LOW);
  digitalWrite(motor_2, HIGH);

  analogWrite(enable_2, speed);
  digitalWrite(motor_3, LOW);
  digitalWrite(motor_4, HIGH);   
}

void left(int speed)
{
  analogWrite(enable_1, speed);
  digitalWrite(motor_1, HIGH);
  digitalWrite(motor_2, LOW);

  analogWrite(enable_2, speed);
  digitalWrite(motor_3, HIGH);
  digitalWrite(motor_4, LOW);  
}


void loop()
{
  if(radio.available())
  {
    radio.read(&joystickData, sizeof(joystick));   
    Serial.println(joystickData.accel_x);
  }
  if(joystickData.y < 508 || joystickData.accel_y < -7.00)
  {
    forward(listRange[joystickData.speedCounter]);
  }
  else if(joystickData.y > 580 || joystickData.accel_y > 7.00)
  {
    reverse(listRange[joystickData.speedCounter]);
  }
  else if(joystickData.x < 508)
  {
    left(listRange[joystickData.speedCounter]);
  }
  else if(joystickData.x > 580)
  {
    right(listRange[joystickData.speedCounter]);
  }
  else
  {
    forward(0);
    reverse(0);
    left(0);
    right(0);
  }
}