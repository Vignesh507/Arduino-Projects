#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
#include "DHT.h"

int Buzzer = 12;
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;

const bool FORMAT_12 = true;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

byte Bell[8] = 
{
 0b00100,
 0b01110,
 0b01110,
 0b01110,
 0b11111,
 0b00000,
 0b00100,
 0b00000
};


void setup ()
{
  while (!Serial);
  lcd.init();

  Serial.begin(9600);
  dht.begin();
  
  pinMode(Buzzer,OUTPUT);
  lcd.createChar(1, Bell);
  
  if (! rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning())
  {
    Serial.println("RTC is NOT running!");
  }
  // rtc.adjust(DateTime(2023, 9, 14, 9, 7, 0));
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop()
{
  unsigned int AnalogValue;

  AnalogValue = analogRead(A0);

  Serial.println(AnalogValue);
  if(AnalogValue<100)
  {
    lcd.noBacklight();
  }
  else
  {
    lcd.backlight();
  }
  Time();
  Date();
  Temp_Hum();

  
}

  
void Date()
{
  DateTime now = rtc.now();

  lcd.setCursor(0, 1);
  lcd.print("Date : ");
  lcd.print(now.day(), DEC);
  lcd.print("/");
  lcd.print(now.month(), DEC);
  lcd.print("/");
  lcd.print(now.year(), DEC);

  lcd.print(" ");
  lcd.write(byte(1));

  lcd.setCursor(0,2);
  lcd.print("Day  : ");
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
}

void Time()
{
  DateTime now = rtc.now();
  if (FORMAT_12)
  {
    lcd.setCursor(0, 0);
    lcd.print("Time : ");
    if (now.twelveHour() < 10)
    lcd.print(0);
    lcd.print(now.twelveHour(), DEC);
  }
  else
  {
    Serial.print(now.hour(), DEC);
  }

  lcd.print(':');
  if (now.minute() < 10)
  lcd.print(0);
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  if (now.second() < 10)
  lcd.print(0);
  lcd.print(now.second(), DEC);
  lcd.print(' ');


  if (FORMAT_12)
  {
    if (now.isPM())
    {
      lcd.print("PM");
    }
    else
    {
      lcd.print("AM");
    }
  }
  Serial.println();

    if (now.minute()==00)
  { 
    digitalWrite(Buzzer,HIGH);
    delay(100);
    digitalWrite(Buzzer,LOW);
    delay(60000);
  }
}

void Temp_Hum()
{
   lcd.setCursor(0,3);
   lcd.print("Temp : ");
   lcd.print((float)dht.readTemperature());
   lcd.print(" C");
   delay(2000); 
   lcd.setCursor(0,3);
   lcd.print("Hum  : ");
   lcd.print((float)dht.readHumidity());
   lcd.print(" %");
   delay(2000); 
}
