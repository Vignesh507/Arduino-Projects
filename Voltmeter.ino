#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BluetoothSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);
BluetoothSerial Blt;

const int oneWireBus = 13;     
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

#define VOLTAGE_SENSOR_1 32
#define VOLTAGE_SENSOR_2 34

#define REF_VOLTAGE     12.0
#define ADC_RESOLUTION  2400.0
#define R2              7500.0
#define R1              30000.0

void setup()
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Blt.begin("Voltmeter");
  sensors.begin();
  analogSetAttenuation(ADC_11db);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(12, 20);
  display.print("VOLTMETER");
  display.display();

  delay(2000);

  for(int i=0;i<=100;i+=20){
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(20,10);
    display.print("System Starting");

    display.drawRect(10, 35, 108, 10, WHITE);
    display.fillRect(10, 35, i, 10, WHITE);

    display.display();
    delay(300);
  }

  delay(500);
}

void loop()
{
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);

  int adc_value_1 = analogRead(VOLTAGE_SENSOR_1);
  float voltage_adc_1 = (adc_value_1 * REF_VOLTAGE) / ADC_RESOLUTION;
  float voltage_in_1 = voltage_adc_1 * (R1 + R2) / R2;

  int adc_value_2 = analogRead(VOLTAGE_SENSOR_2);
  float voltage_adc_2 = ((float)adc_value_2 * REF_VOLTAGE) / ADC_RESOLUTION;
  float voltage_in_2 = voltage_adc_2 * (R1 + R2) / R2;  

  Serial.println("----------- Voltmeter -----------");
  Serial.print("Voltage 1 : ");
  Serial.print(voltage_adc_1, 2);
  Serial.println(" V");
  Serial.print("Voltage 2 : ");
  Serial.print(voltage_adc_2, 2);
  Serial.println(" V");
  Serial.print("Temperature : ");
  Serial.print(temperatureC);
  Serial.print(" ºC");
  Serial.print(" | ");
  Serial.print(temperatureF);
  Serial.println(" ºF");
  Serial.println("---------------------------------");
  Serial.println();

  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(10, 5);
  display.print("Voltage 1 : ");
  display.print(voltage_adc_1, 2);
  display.print("V");

  display.setCursor(10, 20);
  display.print("Voltage 2 : ");
  display.print(voltage_adc_2, 2);
  display.print("V");

  display.setCursor(10, 35);
  display.print("Temp : ");
  display.print(temperatureC, 2);
  display.print(" C");
  display.setCursor(51, 50);
  display.print(temperatureF, 2);
  display.print(" F");
  display.display();

  Blt.print("V1 : ");
  Blt.print(voltage_adc_1, 2);
  Blt.println(" V");
  Blt.print("V2 : ");
  Blt.print(voltage_adc_2, 2);
  Blt.println(" V");
  Blt.print("Temp : ");
  Blt.print(temperatureC);
  Blt.print(" C");
  Blt.print(" | ");
  Blt.print(temperatureF);
  Blt.println(" F");
  Blt.println();

  delay(1000);

}

