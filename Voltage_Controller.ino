#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define SENSOR1 34
#define SENSOR2 13
#define SENSOR3 12

#define RELAY1 26
#define RELAY2 25

float REF_VOLTAGE = 3.3;
float ADC_RESOLUTION = 4095.0;

float readVoltage(int pin)
{
  int adc = analogRead(pin);
  float adc_voltage = (adc * REF_VOLTAGE) / ADC_RESOLUTION;
  float input_voltage = adc_voltage * 5.0;
  return input_voltage;
}

void setup()
{
  Serial.begin(115200);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);

  Wire.begin(21,22);
  analogSetAttenuation(ADC_11db);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}

void loop()
{
  float v1 = readVoltage(SENSOR1);
  float v2 = readVoltage(SENSOR2);
  float v3 = readVoltage(SENSOR3);

  bool relay1State = false;
  bool relay2State = false;

  if(v2 >= 9.0)
  {
    digitalWrite(RELAY1, LOW);
    relay1State = true;
  }
  else
  {
    digitalWrite(RELAY1, HIGH);
  }

  if(v3 >= 3.0)
  {
    digitalWrite(RELAY2, LOW);
    relay2State = true;
  }
  else
  {
    digitalWrite(RELAY2, HIGH);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(5,5);
  display.print("V1: ");
  display.print(v1,2);
  display.print("V");

  display.setCursor(5,25);
  display.print("V2: ");
  display.print(v2,2);
  display.print("V");

  display.setCursor(85,25);
  display.print("R1:");

  if(relay1State)
  {
    display.print("ON");
  }
  else
  {
    display.print("OFF");
  }

  display.setCursor(5,45);
  display.print("V3: ");
  display.print(v3,2);
  display.print("V");

  display.setCursor(85,45);
  display.print("R2:");

  if(relay2State)
  {
    display.print("ON");
  }
  else
  {
    display.print("OFF");
  }


  display.display();

  delay(500);
}
