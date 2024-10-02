#include "DHT.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP085.h>
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "Smart Agriculture Protection System"

BlynkTimer timer;
char auth[] = "";
char ssid[] = "POCO M3";
char pass[] = "1883311M3";

#define seaLevelPressure_hPa 1013.25
Adafruit_BMP085 bmp;

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int AirValue = 1100;  
const int WaterValue = 1000;
const int SensorPin = 2;
int soilMoistureValue = 0;
int soilmoisturepercent = 0;
 
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int Relay = 23;
const int Buzzer = 32;

const int Gas_Sensor = 33;
int Air_Quality;
int Air_Percentage;

const int Rain_Sensor = 34;
int Reading;
int Rain_Percentage;

int IN1 = 19;
int IN2 = 18;

void Menu1()
{  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  delay(500);
  if (isnan(h) || isnan(t)) 
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  } 
  Serial.print(F("Humidity : "));
  Serial.print(h);
  Serial.println(" %");
  Serial.print(F("Temperature : "));
  Serial.print(t);
  Serial.println(F(" °C"));

  display.clearDisplay();                            
  display.setTextSize(1.9);                          
  display.setTextColor(WHITE);                     
  display.setCursor(2,5);                         
  display.print("Temperature : ");               
  display.print(t);           
  display.println(" C");  
  display.println(); 
  display.setCursor(2,21);                      
  display.print("Humidity : ");                 
  display.print(h);                        
  display.println(" %");                            
  display.println(); 

  Air_Quality = analogRead(Gas_Sensor);
  Serial.print("Air Quality : ");
  Serial.println(Air_Quality);
  Air_Percentage = map(Air_Quality, 0, 4095, 0, 100);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(2,37);
  display.print("Gas Level : ");
  display.print(Air_Percentage);
  display.println(" %");
  display.display();
 
  if (Air_Quality<=2000)
  {
   digitalWrite(Buzzer, LOW);
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(2,54);
   display.println("Air Quality : Fresh");
   display.display();
   delay(2000);
  }
  else if(Air_Quality>=2000 && Air_Quality<=3500)
  {
   digitalWrite(Buzzer, LOW);
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(5,50);
   display.println("Air Quality : Poor"); 
   display.display();
   delay(2000);
  }
  else if (Air_Quality>=3500 )
  {
   digitalWrite(Buzzer, HIGH);
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(5,50);
   display.println("Air Quality : Danger!");
   display.display();
   delay(2000);
  }

 Reading = analogRead(Rain_Sensor);
 Serial.print("Rain Fall : ");
 Serial.println(Reading);
 Serial.println();
 Rain_Percentage = map(Reading, 0, 4095, 100, -1);

 display.clearDisplay();
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(2,5);
 display.print("Rain Fall : ");
 display.print(Rain_Percentage);
 display.println(" %");
 delay(100);
  
 if(Rain_Percentage > 70)
 {
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(2,21);
   display.print("Door Status : Close");
   display.display();  
   digitalWrite(IN1, HIGH);
   digitalWrite(IN2, LOW); 
   delay(1000);
 } 
 else
 {   
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(2,21);
   display.print("Door Status : Open");
   display.display();
   digitalWrite(IN1, LOW); 
   digitalWrite(IN2, HIGH); 
   delay(1000);
 }

  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, LOW); 
  delay(1000);

soilMoistureValue = analogRead(SensorPin); 
Serial.print("Soil Moisture : ");
Serial.println(soilMoistureValue);
soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);

Serial.println(soilmoisturepercent);
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(2, 37);
display.print("Soil Moisture : ");
display.display();

display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(2, 54);
display.print("Motor Status : ");
display.display();

if(soilmoisturepercent <0)
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(95, 37);
  display.println("0 %");
  display.display();
}
else if(soilmoisturepercent >=0 && soilmoisturepercent <= 100)
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(95, 37);
  display.print(soilmoisturepercent);
  display.println(" %");
  display.display();
} 
else if(soilmoisturepercent > 100)
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(95, 37);
  display.println("100 %");
  display.display();
} 

if (soilmoisturepercent > 80)
{
 digitalWrite(Relay, LOW);
 display.setTextSize(1);
 display.setTextColor(WHITE);
 display.setCursor(93, 54); 
 display.println("Off");
 display.display();
}
else
{
  digitalWrite(Relay, HIGH);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(93, 54);
  display.println("On");
  display.display();
}
delay(1000);

 float pressure = bmp.readPressure() / 100.0; // Get pressure reading in Pascals and convert to millibar
 Serial.print("Pressure: ");
 Serial.print(pressure);
 Serial.println(" mb");
 delay(1000);
}
  
void setup() 
{
  Serial.begin(115200); 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  dht.begin();
  Blynk.begin(auth, ssid, pass);
if (!bmp.begin()) 
  {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
 }
  
  pinMode(Relay, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Gas_Sensor, INPUT);
  pinMode(Rain_Sensor, INPUT);
  pinMode(IN1 ,OUTPUT);
  pinMode(IN2, OUTPUT);
}
  
void loop() 
{ 

  Blynk.run();
  timer.run();
}
