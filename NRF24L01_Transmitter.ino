#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

MPU6050 mpu;
RF24 radio(9, 10);

const byte address[6] = "00001";

#define Down_Button 2
#define Select_Button 4
#define buzzer 3

#define X_axis A7
#define Y_axis A6

const float Accel_factor = 16384.0;
const float Gravity = 9.81;

int menuIndex = 0;
const char* menuItems[] = { "Joystick Mode", "Gyroscope Mode", "Setting" };
const int totalItems = 3;
int selectedScreen = -1;
int level = -1;
int state = 0;

struct SensorData {
  int joyX;
  int joyY;
  int counter;
  float accel_x, accel_y, accel_z;
} dataToSend;

void playStartup() {
  int melody[] = { 262, 330, 392, 523 };
  int duration[] = { 200, 200, 200, 400 };

  for (int i = 0; i < 4; i++) {
    tone(buzzer, melody[i], duration[i]);
    delay(duration[i] + 50);
  }
}

int stateLevel = false;

void setup() {
  pinMode(Down_Button, INPUT_PULLUP);
  pinMode(Select_Button, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  Serial.begin(115200);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 OLED failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(15, 7);
  display.print("CodeWire");
  display.display();
  delay(1000);
  display.clearDisplay();

  playStartup();

  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1)
      ;
  }

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
}

void loop() {
  if (digitalRead(Down_Button) == LOW) {
    if (selectedScreen == -1) {
      tone(buzzer, 1500, 100);
      menuIndex = (menuIndex + 1) % totalItems;
      Serial.println(menuIndex);
      delay(200);
    }
  }

  if (digitalRead(Select_Button) == LOW) {
    if (selectedScreen == menuIndex) {
      tone(buzzer, 1500, 100);
      selectedScreen = -1;
    } else {
      tone(buzzer, 1500, 100);
      selectedScreen = menuIndex;
    }
    delay(500);
  }

  Menu();
}

void Menu() {
  bool success = radio.write(&dataToSend, sizeof(SensorData));

  if (success == HIGH && !stateLevel) {
    stateLevel = true;
    for (int i = 0; i < 3; i++) {
      digitalWrite(buzzer, HIGH);
      delay(50);
      digitalWrite(buzzer, LOW);
      delay(30);
    }
  }
  if (success == LOW) {
    stateLevel = false;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  if (selectedScreen == 0) {
    dataToSend.joyX = analogRead(X_axis);
    dataToSend.joyY = analogRead(Y_axis);

    Serial.print("X Axis : ");
    Serial.print(dataToSend.joyX);

    Serial.print(" | Y Axis : ");
    Serial.println(dataToSend.joyY);

    enable();

    display.setCursor(36, 3);
    display.print("Joystick");
    display.drawLine(5, 13, 123, 13, WHITE);
    display.setCursor(5, 20);
    display.print("X Axis : ");
    display.println(dataToSend.joyX);
    display.setCursor(5, 30);
    display.print("Y Axis : ");
    display.println(dataToSend.joyY);
    display.setCursor(5, 40);
    display.print("Speed  : ");
    display.print(dataToSend.counter);
    display.setCursor(5, 50);
    display.print("Mode   : ");
    graphAnimation();
    delay(150);
  } else if (selectedScreen == 1) {
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);

    dataToSend.accel_x = (ax / Accel_factor) * Gravity;
    dataToSend.accel_y = (ay / Accel_factor) * Gravity;
    //dataToSend.accel_z = (az / Accel_factor) * Gravity;
    enable();

    display.setCursor(36, 3);
    display.print("Gyroscope");
    display.drawLine(5, 13, 123, 13, WHITE);
    display.setCursor(5, 20);
    display.print("X : ");
    display.println(dataToSend.accel_x);
    display.setCursor(5, 30);
    display.print("Y : ");
    display.println(dataToSend.accel_y);
    display.setCursor(5, 40);
    display.print("Speed  : ");
    display.print(dataToSend.counter);
    //display.print("Z : ");
    //display.println(dataToSend.accel_z);
    delay(150);
    radio.write(&dataToSend, sizeof(SensorData));
  } else if (selectedScreen == 2) {
    display.setCursor(36, 3);
    display.print("Setting");
    display.drawLine(5, 13, 123, 13, WHITE);
  } else {
    //display.drawLine(0, 0, 128, 0, WHITE);
    display.setCursor(35, 3);
    display.print("Main Menu");
    display.drawLine(10, 13, 118, 13, WHITE);

    for (int i = 0; i < totalItems; i++) {
      if (i == menuIndex) {
        display.setCursor(10, 20 + (i * 15));
        display.print("> ");
      } else {
        display.setCursor(20, 20 + (i * 15));
      }
      display.println(menuItems[i]);
    }
  }
  display.display();
}

void graphAnimation() {
  int xPos = map(dataToSend.joyY, 1023, 0, 95, 120);
  int yPos = map(dataToSend.joyX, 0, 1023, 20, 60);
  display.drawLine(88, 18, 88, 60, WHITE);
  display.fillCircle(xPos, yPos, 4, WHITE);
}

void enable() {
  int ButtonState = digitalRead(Down_Button);
  if (ButtonState == LOW) {
    dataToSend.counter++;
    tone(buzzer, 5000, 50);
    //Serial.print("Speed : ");
    //Serial.println(dataToSend.counter);
    delay(200);
    if (dataToSend.counter == 4) {
      dataToSend.counter = 0;
    }
  }
}
