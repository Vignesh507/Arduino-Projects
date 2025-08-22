#include <Otto.h>
#include <SoftwareSerial.h>
#include <PlayRtttl.hpp>

Otto Otto;
SoftwareSerial bluetooth(9, 8);

#define LeftLeg   4 
#define RightLeg  5 
#define LeftFoot  2 
#define RightFoot 3 
#define Buzzer    12
#define battery   A0
#define trigPin   11
#define echoPin   10
#define indicate  13
#define BatLED    6

char  bluetooth_data;
int   battery_volt;
float voltage;
float voltage_status;
long  duration;
long  cm;

unsigned long previousMillis  = 0;
unsigned long previousMillis1 = 0;
const long interval           = 100;
const long interval1          = 500;
uint8_t activated             = 1;
int speed                     = 1000;
bool soundPlayed              = false;

void Otto_moveLegs(int T, int posLegL, int posLegR, int posFootL, int posFootR) 
{   
	int posLegs[] = { posLegL,posLegR,posFootL,posFootR};   
	Otto._moveServos(T,posLegs); 
}

void battery_status()
{
  battery_volt = analogRead(battery);
  voltage = battery_volt * (5.0 / 1023.0);
  voltage_status = voltage * 2.00;
  Serial.print("Battery Voltage : ");
  Serial.print(voltage_status);
  Serial.println("V");
  Serial.println();
    
  if(voltage_status < 7.5)
  {
    digitalWrite(BatLED, HIGH);
    delay(100);
    digitalWrite(BatLED, LOW);
    delay(80);
  }
}

void obstacle_Mode()
{
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = microsecondsToCentimeters(duration); 
  Serial.print(cm);
  Serial.println(" cm");
  delay(100);

	if(cm >= 0 && cm <= 8)
	{
    if(!soundPlayed) { Otto.sing(S_OhOoh2); soundPlayed = true; }
    Otto.turn(1, 800, -1);
  }
  else if(cm >= 9 && cm <= 25)
  {
    if(!soundPlayed) { Otto.sing(S_OhOoh); soundPlayed = true; }
    Otto.walk(1, 1000, -1);
  }
	else if(cm > 26)
	{
		Otto_moveLegs(200, 90, 90, 90, 90);
    soundPlayed = false;
	}
}

long microsecondsToCentimeters(long microseconds) 
{
   return microseconds * 0.0343 / 2;
}

void setup()
{
  Serial.begin(9600);
  bluetooth.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(indicate, OUTPUT);
  pinMode(BatLED, OUTPUT);
  Otto.init(LeftLeg, RightLeg, LeftFoot, RightFoot, true, Buzzer);
  Otto.home();
  Otto.sing(S_connection);
}

void loop()
{
 unsigned long currentMillis = millis(); 
 if(currentMillis - previousMillis >= interval)  { previousMillis = currentMillis; battery_status(); }
 
 if(bluetooth.available())
 {
  bluetooth_data = bluetooth.read();
  Serial.print("Bluetooth Receive : ");
  Serial.println(bluetooth_data);
  
  if(bluetooth_data == '1')      { speed = 800; Otto.sing(S_disconnection);}
  else if(bluetooth_data == '2') { speed = 500; Otto.sing(S_disconnection);}  
  else if(bluetooth_data == '0') { speed = 1000; Otto.sing(S_disconnection);}

  if(bluetooth_data == '@'){ setup(); }

  if(bluetooth_data == 'P')
  { 
    Otto.sing(S_mode2);
    digitalWrite(indicate, HIGH);
    activated = 0;
  }
  else if(bluetooth_data == 'Q') { delay(200); Otto_moveLegs(200, 90, 90, 90, 90); }

  while(activated == 0)
  {
    unsigned long currentMillis1 = millis(); 
    if(currentMillis1 - previousMillis1 >= interval1){ previousMillis1 = currentMillis1; obstacle_Mode(); }
    
    bluetooth_data = bluetooth.read();
    if(bluetooth_data == 'Q') { Otto.sing(S_mode2); digitalWrite(indicate, LOW); activated = 1; }
  }

  switch(bluetooth_data)
  {
    case 'S':
    Otto.walk(4, speed, -1);
    Otto_moveLegs(200, 90, 90, 90, 90);
    delay(100);
    break;

    case 'W':
    Otto.walk(4, speed, 1);
    Otto_moveLegs(200, 90, 90, 90, 90);
    delay(100);
    break;

    case 'A':
    Otto.turn(4, speed, -1);
    Otto_moveLegs(200, 90, 90, 90, 90);
    delay(100);
    break;

    case 'D':
    Otto.turn(4, speed, 1);
    Otto_moveLegs(200, 90, 90, 90, 90);
    delay(100);
    break;  

    case 'B':
    Otto.playGesture(OttoLove);
    Otto_moveLegs(200, 90, 90, 90, 90);
    delay(100);
    break;

    case 'C':
    Otto.playGesture(OttoSuperHappy);
    Otto_moveLegs(200, 90, 90, 90, 90);
    delay(100);
    break;   

    case 'E':
    Otto.playGesture(OttoVictory);
    Otto_moveLegs(200, 90, 90, 90, 90);
    delay(100);
    break;

    case 'F':
    Otto.playGesture(OttoMagic);
    Otto_moveLegs(200, 90, 90, 90, 90);
    delay(100);
    break;      

    case 'G':
    Otto.playGesture(OttoConfused);
    Otto_moveLegs(200, 90, 90, 90, 90);
    delay(100);
    break;

    case 'H':
    Otto.playGesture(OttoSleeping);
    Otto_moveLegs(200, 90, 90, 90, 90);
    delay(100);
    break;   

    case 'I':
    Otto.playGesture(OttoSad);
    Otto_moveLegs(200, 90, 90, 90, 90);
    delay(100);
    break;

    case 'O':
    Otto.playGesture(OttoAngry);
    Otto_moveLegs(200, 90, 90, 90, 90);
    delay(100);
    break;      

    case '4':
    Otto.shakeLeg(1, 1000, -1);
    playRtttlBlockingPGM(12, (char*)TakeOnMe);   
    Otto.shakeLeg(1, 1000, -1);
    Otto_moveLegs(200, 90, 90, 90, 90); 
    break;  

    case '3':
    Otto.moonwalker(4, 1000, 25, 1);
    playRtttlBlockingPGM(12, (char*)MissionImp);
    Otto_moveLegs(200, 90, 90, 90, 90);
    break;    

    case '5':
    Otto.sing(S_happy);
    Otto.bend(1, 1000, 1); Otto.bend(1, 1000, -1); Otto.bend(1, 1000, 1); Otto.bend(1, 1000, -1);   
    Otto.shakeLeg(1, 1000, -1); Otto.shakeLeg(1, 1000, 1); Otto.shakeLeg(1, 1000, -1); Otto.shakeLeg(1, 1000, 1);   
    Otto.jump(5, 1000); 
    Otto.sing(S_happy);
    break;   

    case '6':
    Otto.crusaito(1, 1000, 25, 1);
    delay(200);
    Otto.flapping(1, 1000, 25, -1);
    delay(200);
    Otto.crusaito(1, 1000, 25, -1);
    delay(200);
    Otto.flapping(1, 1000, 25, 1);
    Otto_moveLegs(200, 90, 90, 90, 90);
    break;     

    case '7':
    Otto.sing(S_superHappy);
    Otto.swing(1, 800, 30);
    delay(200);
    Otto.jitter(1, 1000, 25);
    delay(200);    
    Otto.updown(1, 800, 25);
    delay(200);
    Otto.tiptoeSwing(1, 1000, 25);
    delay(200);
    Otto.swing(1, 800, 30);
    delay(200);
    Otto.jitter(1, 1000, 25);
    delay(200);
    Otto.ascendingTurn(1, 1000, 30);

    Otto.sing(S_happy);
    Otto.swing(1, 500, 30);
    delay(200);
    Otto.jitter(1, 1000, 25);
    delay(200);    
    Otto.updown(1, 800, 25);
    delay(200);
    Otto.tiptoeSwing(1, 1000, 25);
    delay(200);
    Otto.swing(1, 800, 30);
    delay(200);
    Otto.jitter(1, 1000, 25);
    delay(200);
    Otto.ascendingTurn(1, 500, 30);    
    delay(200);
    Otto.moonwalker(4, 1000, 25, -1);
    delay(200);
    Otto.moonwalker(4, 1000, 25, 1);
    Otto.sing(S_cuddly);

    Otto.crusaito(1, 1000, 25, 1);
    delay(200);
    Otto.flapping(1, 1000, 25, -1);
    delay(200);
    Otto.crusaito(1, 1000, 25, -1);
    delay(200);
    Otto.flapping(1, 1000, 25, 1); 
    Otto_moveLegs(200, 90, 90, 90, 90);
    break;

    case '8':
    Otto.moonwalker(4, speed, 25, -1);
    delay(100);
    Otto.sing(S_OhOoh);
    Otto.moonwalker(4, speed, 25, 1);
    delay(100);
    Otto.sing(S_OhOoh);
    Otto_moveLegs(200, 90, 90, 90, 90);
  }
 }
}