#define BLYNK_TEMPLATE_ID "TMPLA0G9N2e6"
#define BLYNK_DEVICE_NAME "Smart Agriculture System"
#define BLYNK_AUTH_TOKEN "dqLRCG_P50XqO5eDrxw-7MO5dfSOSU_l"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "POCO M3"; 
char pass[] = "1883311M3"; 

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

BlynkTimer timer;

#define DHTPIN 12 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int Buzzer = 0;

int Tank_height = 8; 
int Fill_height = 4;

long t;
int distance_Measured;
int distance_Sensor_to_Fill;

unsigned char a = 0;
unsigned int level = 0;
unsigned int temp = 10;
int tankLevel;


int trigPin = 15;
int echoPin = 13;

void sendSensor()
{

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  t = pulseIn(echoPin, HIGH);
  distance_Measured = t/48;

  tankLevel = map(distance_Measured, distance_Sensor_to_Fill, Tank_height, 100, 0);
  
  if (tankLevel > 100) 
  tankLevel = 100;
  if (tankLevel < 0) 
  tankLevel = 0;

  if(tankLevel < 20)
  {
    digitalWrite(Buzzer, HIGH);
  }
  else
  {
    digitalWrite(Buzzer, LOW);
  }
  
  int value = analogRead(A0);
  value = map(value, 0, 1023, 0, 100);

  if (value > 90) 
  {
    digitalWrite(14, HIGH);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,50);
    display.println("Motor Status : On");
    display.display();
    display.clearDisplay();
  } 
  else 
      {
        digitalWrite(14, LOW);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,50);
        display.println("Motor Status : Off");
        display.display();
        display.clearDisplay();
      }

        
 
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  if (isnan(h) || isnan(t)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);
    Blynk.virtualWrite(V2, value);
    Blynk.virtualWrite(V3, tankLevel);
    
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("    Humidity : ");
    Serial.println(h);
    Serial.println(value);

  Serial.print("Tank Level:");
  Serial.print(tankLevel);
  Serial.println("%");
  
  display.clearDisplay();                            
  display.setTextSize(1.9);                          
  display.setTextColor(WHITE);                     
  display.setCursor(0,0);                         
  display.print("Temperature : ");               
  display.print(t);           
  display.println(" C");  
  display.println();                      
  display.print("Humidity : ");                 
  display.print(h);                        
  display.println(" %");                            
  display.println();  
  display.print("Soil Moisture : ");
  display.print(value);                        
  display.println(" %");                            
  display.println(); 
}
void setup()
{   
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  pinMode(14, OUTPUT);
  pinMode(A0, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(2000);

  display.clearDisplay();
  display.setTextSize(1.9);
  display.setTextColor(WHITE);
  display.setCursor(0,28);
  display.println("Smart Agriculture");
  display.println("");
  display.println("System");
  display.display();
  delay(2000);
  display.clearDisplay();

  pinMode(Buzzer, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

    
  distance_Sensor_to_Fill = Tank_height - Fill_height; 
  
  timer.setInterval(100L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
 }
