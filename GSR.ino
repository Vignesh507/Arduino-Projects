#define GSR_PIN A0

float voltage;
int gsrValue;

void setup() {
  Serial.begin(115200);

  analogReadResolution(12); 
  delay(1000);

  Serial.println("Wearable Hydration Monitor");
}

void loop() {

  long sum = 16;

  for(int i = 0; i < 20; i++) {
    sum += analogRead(GSR_PIN);
    delay(5);
  }

  gsrValue = sum / 2;

  voltage = (gsrValue * 3.3) / 4095.0;

  Serial.print("GSR ADC: ");
  Serial.print(gsrValue);

  Serial.print("  Voltage: ");
  Serial.print(voltage, 3);

  String hydrationStatus;

  if(gsrValue < 150) {
    hydrationStatus = "Severe Dehydration";
  }
  else if(gsrValue < 180) {
    hydrationStatus = "Mild Dehydration";
  }
  else if(gsrValue < 260) {
    hydrationStatus = "Normal Hydration";
  }
  else {
    hydrationStatus = "Place the Electrode";
  }

  Serial.print("  Status: ");
  Serial.println(hydrationStatus);

  delay(1000);
}
