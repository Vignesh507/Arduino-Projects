#include<SoftwareSerial.h>
SoftwareSerial mySerial(15,2);

#define Relay1 4
#define Relay2 5
#define Relay3 6
#define Relay4 7

char val;
void setup() 
{
  pinMode(Relay1,OUTPUT);
  pinMode(Relay2,OUTPUT);
  pinMode(Relay3,OUTPUT);
  pinMode(Relay4,OUTPUT);

  digitalWrite(Relay1,HIGH);
  digitalWrite(Relay2,HIGH);
  digitalWrite(Relay3,HIGH);
  digitalWrite(Relay4,HIGH);

  mySerial.begin(9600);
  Serial.begin(9600);
}

void loop() 
{
 if(mySerial.available() > 0)
 {
 val = mySerial.read();
 Serial.println(val);
 }

 if(val == '1')
 {
   digitalWrite(Relay1,LOW);
 }
 else if(val == '2')
 {
   digitalWrite(Relay2,LOW);
 }
 else if(val == '3')
 {
   digitalWrite(Relay3,LOW);
 }
 else if(val == '4')
 {
   digitalWrite(Relay4,LOW);
 }
 else if(val == '9')
 {
   digitalWrite(Relay1,LOW);
   digitalWrite(Relay2,LOW);
   digitalWrite(Relay3,LOW);
   digitalWrite(Relay4,LOW);
 }

 else if(val == 'A')
 {
   digitalWrite(Relay1,HIGH);
 }
 else if(val == 'B')
 {
   digitalWrite(Relay2,HIGH);
 }
 else if(val == 'C')
 {
   digitalWrite(Relay3,HIGH);
 }
 else if(val == 'D')
 {
   digitalWrite(Relay4,HIGH);
 }
 else if(val == 'I')
 {
   digitalWrite(Relay1,HIGH);
   digitalWrite(Relay2,HIGH);
   digitalWrite(Relay3,HIGH);
   digitalWrite(Relay4,HIGH);
 }
}
