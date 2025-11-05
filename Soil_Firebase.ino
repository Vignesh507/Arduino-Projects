#include <WiFi.h>
#include <FirebaseESP32.h>
#include <SoftwareSerial.h>

#define SSID "POCO M3"
#define PASS "9952033054"
#define FIREBASE_HOST "soil-hydration-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "HQ5bxuyOPSo4ImlLZ8nfvccEAIwxucwYRr026JjH"

#define RX_PIN 12  
#define TX_PIN 13 

SoftwareSerial MegaSerial(RX_PIN, TX_PIN);

String inputString = "";
int sensor1, sensor2, sensor3, sensor4, sensor5;
int sensor6, sensor7, sensor8, sensor9, sensor10;

int sensorCount = 0;

FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;

void wifiIndicate(int ms_delay)
{
  digitalWrite(15, HIGH); delay(ms_delay);
  digitalWrite(15, LOW); delay(ms_delay);
}

void wifiStatus()
{
  WiFi.begin(SSID, PASS);
  Serial.print("WiFi Connecting....");
  while(WiFi.status() != WL_CONNECTED)
  {
    wifiIndicate(200);
  }
  Serial.println("WiFi Connected");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
}

void setup() 
{
  Serial.begin(115200);
  MegaSerial.begin(9600);
  pinMode(15, OUTPUT);

  wifiStatus();

  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("ESP32 Ready");
}

void loop() 
{
  while(MegaSerial.available()) 
  {
    char c = MegaSerial.read();
    if (c == '\n') 
    {             
      inputString.trim();

      if (inputString.length() > 0) 
      {
        int value = inputString.toInt();
        sensorCount++;

        switch (sensorCount) 
        {
          case 1: sensor1 = value; break;
          case 2: sensor2 = value; break;
          case 3: sensor3 = value; break;
          case 4: sensor4 = value; break;
          case 5: sensor5 = value; break;
          case 6: sensor6 = value; break;
          case 7: sensor7 = value; break;
          case 8: sensor8 = value; break;
          case 9: sensor9 = value; break;
          case 10:
            sensor10 = value;
            printAllSensors(); 
            upadateTofirebase();
            sensorCount = 0;   
            break;
        }
      }
      inputString = ""; 
    } 
    else 
    {
      inputString += c; 
    }
  }
}

void printAllSensors() 
{
  Serial.println("\n------ Soil Moisture Data Received ------");
  Serial.print("Sensor 1: "); Serial.println(sensor1);
  Serial.print("Sensor 2: "); Serial.println(sensor2);
  Serial.print("Sensor 3: "); Serial.println(sensor3);
  Serial.print("Sensor 4: "); Serial.println(sensor4);
  Serial.print("Sensor 5: "); Serial.println(sensor5);
  Serial.print("Sensor 6: "); Serial.println(sensor6);
  Serial.print("Sensor 7: "); Serial.println(sensor7);
  Serial.print("Sensor 8: "); Serial.println(sensor8);
  Serial.print("Sensor 9: "); Serial.println(sensor9);
  Serial.print("Sensor 10: "); Serial.println(sensor10);
  Serial.println("------------------------------------------\n");
}

void upadateTofirebase()
{
  Firebase.setInt(fbdo, "/SoilSensor/Sensor_1", sensor1);
  Firebase.setInt(fbdo, "/SoilSensor/Sensor_2", sensor2);
  Firebase.setInt(fbdo, "/SoilSensor/Sensor_3", sensor3);
  Firebase.setInt(fbdo, "/SoilSensor/Sensor_4", sensor4);
  Firebase.setInt(fbdo, "/SoilSensor/Sensor_5", sensor5);
  Firebase.setInt(fbdo, "/SoilSensor/Sensor_6", sensor6);
  Firebase.setInt(fbdo, "/SoilSensor/Sensor_7", sensor7);
  Firebase.setInt(fbdo, "/SoilSensor/Sensor_8", sensor8);
  Firebase.setInt(fbdo, "/SoilSensor/Sensor_9", sensor9);
  Firebase.setInt(fbdo, "/SoilSensor/Sensor_10", sensor10);
}