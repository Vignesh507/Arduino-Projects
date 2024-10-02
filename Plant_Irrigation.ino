int value = A0; 
int sensorValue = 0; 
int motor = 11;

void setup() 
{
  Serial.begin(9600); 
  pinMode(motor, OUTPUT);
}

void loop() 
{
  int value = analogRead(A0);
  value = map(value, 0, 1023, 0, 100);

  if (value > 90) 
  {
    digitalWrite(11, HIGH);
  } 
  else 
  {
    digitalWrite(11, LOW);
  }
}
