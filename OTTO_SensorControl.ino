#include <Wire.h>
#include <SoftwareSerial.h>
#include <Otto.h>
#include "DFRobotDFPlayerMini.h"
#include "paj7620.h"

SoftwareSerial softwareSerial(11, 10);
DFRobotDFPlayerMini player;
Otto Otto;

#define GES_REACTION_TIME		500				
#define GES_ENTRY_TIME			200			 
#define GES_QUIT_TIME			  1000

const int modeButton = 6;
uint8_t incrementCounter;
uint8_t buttonState;

const int echo = 8;
const int trig = 7;

long cm;
long duration;
char command;

#define LeftLeg   13 
#define RightLeg  9 
#define LeftFoot  12 
#define RightFoot 4 
#define Buzzer    5

void Otto_moveLegs(int T, int posLegL, int posLegR, int posFootL, int posFootR) 
{   
	int posLegs[]={ posLegL,posLegR,posFootL,posFootR};   
	Otto._moveServos(T,posLegs); 
}

void setup()
{
  Serial.begin(9600);
  softwareSerial.begin(9600);
  Otto.init(LeftLeg, RightLeg, LeftFoot, RightFoot, true, Buzzer);
	Otto.home();

  pinMode(modeButton, INPUT_PULLUP);

  if(player.begin(softwareSerial))
  {
    Serial.println("Connected");
    player.volume(30);
    player.play(1);
  }
  else
  {
    Serial.println("Disconnected");
  }

  uint8_t error = 0;
	error = paj7620Init();			
	if (error) 
	{
		Serial.print("Initialize Error : ");
		Serial.println(error);
	}
	else
	{
		Serial.println("Initialize ok");
	}
	Serial.println("Please input your gestures:\n");
}

void loop()
{
 buttonState = digitalRead(modeButton);

 if(buttonState == 1)
 {
   incrementCounter++;
   Serial.print("ButtonState : ");
   Serial.println(incrementCounter);
   delay(250);
   if(incrementCounter == 3)
   {
    incrementCounter = 0;
   }

	 switch(incrementCounter)
	 {
		case 1:
		player.play(2);
		command = 'A';
		break;

		case 2:
		player.play(3);
		command = 'B';
		break;

		case 0:
		player.play(4);
    command = 'C';
		break;
	 }
 }

 if(command == 'A')
 {
   Human_Following();
 }
 else if(command == 'B')
 {
   gesture_Control();
	 Otto_moveLegs(200, 90, 90, 90, 90);
 }
 else if(command == 'C')
 {
   gesture_Assistant();
 }
}

void Human_Following()
{
  pinMode(trig, OUTPUT);
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  pinMode(echo, INPUT);
  duration = pulseIn(echo, HIGH);
  cm = microsecondsToCentimeters(duration); 
  Serial.print(cm);
  Serial.println(" cm");
  delay(100);

	if(cm >= 10 && cm <= 25)
	{
		Otto.walk(1, 1000, 1);
	}
	else if(cm > 26)
	{
		Otto_moveLegs(200, 90, 90, 90, 90);
	}
}

long microsecondsToCentimeters(long microseconds) 
{
   return microseconds * 0.0343 / 2;
}

void gesture_Control()
{
	uint8_t data = 0, data1 = 0, error;
	error = paj7620ReadReg(0x43, 1, &data);		
	if (!error) 
	{
		switch (data) 							
		{
			case GES_RIGHT_FLAG:
			delay(GES_ENTRY_TIME);
			paj7620ReadReg(0x43, 1, &data);
			if(data == GES_FORWARD_FLAG) 
			{
			 Serial.println("Forward");
			 delay(GES_QUIT_TIME);
			}
			else if(data == GES_BACKWARD_FLAG) 
			{
			 Serial.println("Backward");
			 delay(GES_QUIT_TIME);
			}
			else
			{
			 Serial.println("right");
			 player.stop();
			 Otto.walk(4, 1000, -1);
			}          
			break;

			case GES_LEFT_FLAG: 
			delay(GES_ENTRY_TIME);
			paj7620ReadReg(0x43, 1, &data);
			if(data == GES_FORWARD_FLAG) 
			{
			 Serial.println("Forward");
			 delay(GES_QUIT_TIME);
			}
			else if(data == GES_BACKWARD_FLAG) 
			{
  		 Serial.println("Backward");
			 delay(GES_QUIT_TIME);
			}
			else
			{
			 Serial.println("Left");
			 player.stop();
			 Otto.walk(4, 1000, 1);
			}          
			break;

			case GES_UP_FLAG:
			delay(GES_ENTRY_TIME);
			paj7620ReadReg(0x43, 1, &data);
			if(data == GES_FORWARD_FLAG) 
			{
		   Serial.println("Forward");
			 delay(GES_QUIT_TIME);
			}
			else if(data == GES_BACKWARD_FLAG) 
			{
			 Serial.println("Backward");
			 delay(GES_QUIT_TIME);
			}
			else
			{
			 Serial.println("Up");
			 player.stop();
			 Otto.turn(4, 1000, -1);
			}          
			break;

			case GES_DOWN_FLAG:
			delay(GES_ENTRY_TIME);
			paj7620ReadReg(0x43, 1, &data);
			if(data == GES_FORWARD_FLAG) 
			{
			 Serial.println("Forward");
			 delay(GES_QUIT_TIME);
			}
			else if(data == GES_BACKWARD_FLAG) 
			{
			 Serial.println("Backward");
			 delay(GES_QUIT_TIME);
			}
			else
			{
			 Serial.println("Down");
			 player.stop();
			 Otto.turn(4, 1000, 1);
			}          
			break;

			case GES_FORWARD_FLAG:
			Serial.println("Forward");
			delay(GES_QUIT_TIME);
			break;

			case GES_BACKWARD_FLAG:		  
			Serial.println("Backward");
			delay(GES_QUIT_TIME);
			break;

			case GES_CLOCKWISE_FLAG:
			Serial.println("Clockwise");
			player.play(9);
			Otto.moonwalker(4, 1000, 30, 1);
			delay(200);
			Otto.flapping(4, 1000, 30, 1);
			delay(200);
			Otto.moonwalker(4, 1000, 30, -1);
			delay(200);
			Otto.flapping(4, 1000, 30, -1);
			delay(200);
			Otto.shakeLeg(2, 1000, -1);
			break;

			case GES_COUNT_CLOCKWISE_FLAG:
			Serial.println("anti-clockwise");
			player.play(15);
			break;  

			default:
			paj7620ReadReg(0x44, 1, &data1);
			if (data1 == GES_WAVE_FLAG) 
			{
			 Serial.println("wave");
			}
			break;
		}
	}
	delay(100);
}

void gesture_Assistant()
{

}




