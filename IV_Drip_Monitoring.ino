#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DRIP_SENSOR_PIN 13
#define BUZZER_PIN      14
#define SERVO_PIN       15

Servo ivServo;
bool servoActivated = false;

volatile int dripCount = 0;
volatile unsigned long lastDripTime = 0;

unsigned long startTime = 0;
unsigned long lastRateTime = 0;

#define BTN_INC 0
#define BTN_DEC 2

unsigned long lastButtonTime = 0;

int dripRate = 0;

float dripFactor = 20.0;        
float bottleCapacity = 50.0;   


float volumeInfused = 0.0;
float remainingVolume = 500.0;
float bottlePercentage = 100.0;
float estimatedTimeLeft = 0.0;

void IRAM_ATTR countDrip()
{
  dripCount++;
  lastDripTime = millis();
}

void setup()
{
  Serial.begin(115200);

  pinMode(DRIP_SENSOR_PIN, INPUT_PULLUP);
  pinMode(BTN_INC, INPUT_PULLUP);
  pinMode(BTN_DEC, INPUT_PULLUP);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  attachInterrupt(digitalPinToInterrupt(DRIP_SENSOR_PIN), countDrip, FALLING);

  ivServo.attach(SERVO_PIN);
  ivServo.write(0);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED initialization failed");
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(15, 25);
  display.println("IV DRIP MONITOR");
  display.display();

  delay(2000);

  startTime = millis();
  lastRateTime = millis();
  lastDripTime = millis();
}

void loop()
{
  unsigned long currentTime = millis();

 if (millis() - lastButtonTime > 300)
 {
  if (digitalRead(BTN_INC) == LOW)
  {
    bottleCapacity += 50;

    if (bottleCapacity > 2000)
      bottleCapacity = 2000;

    lastButtonTime = millis();

    Serial.print("Bottle Capacity Increased: ");
    Serial.print(bottleCapacity);
    Serial.println(" mL");
  }

  if (digitalRead(BTN_DEC) == LOW)
  {
    bottleCapacity -= 50;

    if (bottleCapacity < 50)
      bottleCapacity = 50;

    lastButtonTime = millis();

    Serial.print("Bottle Capacity Decreased: ");
    Serial.print(bottleCapacity);
    Serial.println(" mL");
  }
 }

  if (currentTime - lastRateTime >= 1000)
  {
    dripRate = dripCount * 60;
    volumeInfused += ((float)dripCount / dripFactor);

    dripCount = 0;
    remainingVolume = bottleCapacity - volumeInfused;

    if (remainingVolume < 0)
      remainingVolume = 0;

    bottlePercentage = (remainingVolume / bottleCapacity) * 100.0;
    float flowRate_ml_min = (float)dripRate / dripFactor;

    if (flowRate_ml_min > 0)
    {
      estimatedTimeLeft =
          remainingVolume / flowRate_ml_min;
    }
    else
    {
      estimatedTimeLeft = 0;
    }

    unsigned long infusionMinutes = (currentTime - startTime) / 60000;

    if (bottlePercentage <= 10 && !servoActivated)
    {
      ivServo.write(90);
      servoActivated = true;

      Serial.println("LOW IV LEVEL!");
      Serial.println("Servo Rotated to 90 Degrees");
    }

    if (bottlePercentage >= 95 && servoActivated)
    {
      ivServo.write(0);
      servoActivated = false;

      Serial.println("NEW BOTTLE DETECTED");
      Serial.println("Servo Reset to 0 Degrees");
    }

    Serial.println("================================");

    Serial.print("Drip Rate : ");
    Serial.print(dripRate);
    Serial.println(" DPM");

    Serial.print("Volume Infused : ");
    Serial.print(volumeInfused, 1);
    Serial.println(" mL");

    Serial.print("Remaining Volume : ");
    Serial.print(remainingVolume, 1);
    Serial.println(" mL");

    Serial.print("Bottle Level : ");
    Serial.print(bottlePercentage, 0);
    Serial.println("%");

    Serial.print("Estimated Time Left : ");
    Serial.print(estimatedTimeLeft, 1);
    Serial.println(" min");

    Serial.print("Infusion Time : ");
    Serial.print(infusionMinutes);
    Serial.println(" min");

    Serial.println("================================");

    display.clearDisplay();

    display.setCursor(0, 0);
    display.print("Rate:");
    display.print(dripRate);
    display.print("DPM");

    display.setCursor(0, 10);
    display.print("Inf:");
    display.print(volumeInfused, 1);
    display.print("mL");

    display.setCursor(0, 20);
    display.print("Rem:");
    display.print(remainingVolume, 1);
    display.print("mL");

    display.setCursor(0, 30);
    display.print("Lvl:");
    display.print(bottlePercentage, 0);
    display.print("%");

    display.setCursor(0, 40);
    display.print("Left:");
    display.print(estimatedTimeLeft, 0);
    display.print("m");

    display.drawRect(100, 5, 20, 50, SSD1306_WHITE);
    int fillHeight = map((int)bottlePercentage, 0, 100, 0, 48);
    display.fillRect(101, 54 - fillHeight, 18, fillHeight, SSD1306_WHITE);

    bool noDrip = (currentTime - lastDripTime > 15000);
    bool lowBottle = (bottlePercentage <= 10);

    if (noDrip)
    {
      display.setCursor(0, 54);
      display.print("NO DRIP!");

      if ((millis() / 500) % 2)
        digitalWrite(BUZZER_PIN, HIGH);
      else
        digitalWrite(BUZZER_PIN, LOW);
    }
    else if (lowBottle)
    {
      display.setCursor(0, 54);
      display.print("LOW IV LEVEL");

      if ((millis() / 300) % 2)
        digitalWrite(BUZZER_PIN, HIGH);
      else
        digitalWrite(BUZZER_PIN, LOW);
    }
    else
    {
      digitalWrite(BUZZER_PIN, LOW);
    }

    display.display();

    lastRateTime = currentTime;
  }
}
