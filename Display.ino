void DisplayMenu()
{
  switch(IncrementCounter)
  {
    case 1:
    {
      oled.setTextColor(WHITE);
      oled.setTextSize(1);
      oled.setCursor(0, 0);
      oled.print("> Switch 1 : ");

      oled.setCursor(0, 12);
      oled.print("  Switch 2 : ");

      oled.setCursor(0, 25);
      oled.print("  Switch 3 : ");

      oled.setCursor(0, 38);
      oled.print("  Switch 4 : ");

      oled.setCursor(0, 50);
      oled.print("  Switch 5 : ");

      oled.setCursor(80, 0);
      oled.print(SelectionCounter1);

      oled.setCursor(80, 12);
      oled.print(SelectionCounter2);

      oled.setCursor(80, 25);
      oled.print(SelectionCounter3);

      oled.setCursor(80, 38);
      oled.print(SelectionCounter4);

      oled.setCursor(80, 50);
      oled.print(SelectionCounter5);

      oled.display();
      oled.clearDisplay();
    }
    break;

    case 2:
    {
      oled.setTextColor(WHITE);
      oled.setTextSize(1);
      oled.setCursor(0, 0);
      oled.print("  Switch 1 : ");

      oled.setCursor(0, 12);
      oled.print("> Switch 2 : ");

      oled.setCursor(0, 25);
      oled.print("  Switch 3 : ");

      oled.setCursor(0, 38);
      oled.print("  Switch 4 : ");

      oled.setCursor(0, 50);
      oled.print("  Switch 5 : ");

      oled.setCursor(80, 0);
      oled.print(SelectionCounter1);

      oled.setCursor(80, 12);
      oled.print(SelectionCounter2);

      oled.setCursor(80, 25);
      oled.print(SelectionCounter3);

      oled.setCursor(80, 38);
      oled.print(SelectionCounter4);

      oled.setCursor(80, 50);
      oled.print(SelectionCounter5);

      oled.display();
      oled.clearDisplay();
    }
    break;

    case 3:
    {
      oled.setTextColor(WHITE);
      oled.setTextSize(1);
      oled.setCursor(0, 0);
      oled.print("  Switch 1 : ");

      oled.setCursor(0, 12);
      oled.print("  Switch 2 : ");

      oled.setCursor(0, 25);
      oled.print("> Switch 3 : ");

      oled.setCursor(0, 38);
      oled.print("  Switch 4 : ");

      oled.setCursor(0, 50);
      oled.print("  Switch 5 : ");

      oled.setCursor(80, 0);
      oled.print(SelectionCounter1);

      oled.setCursor(80, 12);
      oled.print(SelectionCounter2);

      oled.setCursor(80, 25);
      oled.print(SelectionCounter3);

      oled.setCursor(80, 38);
      oled.print(SelectionCounter4);

      oled.setCursor(80, 50);
      oled.print(SelectionCounter5);

      oled.display();
      oled.clearDisplay();
    }
    break;

    case 4:
    {
      oled.setTextColor(WHITE);
      oled.setTextSize(1);
      oled.setCursor(0, 0);
      oled.print("  Switch 1 : ");

      oled.setCursor(0, 12);
      oled.print("  Switch 2 : ");

      oled.setCursor(0, 25);
      oled.print("  Switch 3 : ");

      oled.setCursor(0, 38);
      oled.print("> Switch 4 : ");

      oled.setCursor(0, 50);
      oled.print("  Switch 5 : ");

      oled.setCursor(80, 0);
      oled.print(SelectionCounter1);

      oled.setCursor(80, 12);
      oled.print(SelectionCounter2);

      oled.setCursor(80, 25);
      oled.print(SelectionCounter3);

      oled.setCursor(80, 38);
      oled.print(SelectionCounter4);

      oled.setCursor(80, 50);
      oled.print(SelectionCounter5);

      oled.display();
      oled.clearDisplay();
    }
    break;  

    case 5:
    {
      oled.setTextColor(WHITE);
      oled.setTextSize(1);
      oled.setCursor(0, 0);
      oled.print("  Switch 1 : ");

      oled.setCursor(0, 12);
      oled.print("  Switch 2 : ");

      oled.setCursor(0, 25);
      oled.print("  Switch 3 : ");

      oled.setCursor(0, 38);
      oled.print("  Switch 4 : ");

      oled.setCursor(0, 50);
      oled.print("> Switch 5 : ");

      oled.setCursor(80, 0);
      oled.print(SelectionCounter1);

      oled.setCursor(80, 12);
      oled.print(SelectionCounter2);

      oled.setCursor(80, 25);
      oled.print(SelectionCounter3);

      oled.setCursor(80, 38);
      oled.print(SelectionCounter4);

      oled.setCursor(80, 50);
      oled.print(SelectionCounter5);

      oled.display();
      oled.clearDisplay();
    }
    break; 

    case 0:
    {
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      float f = dht.readTemperature(true);

      if (isnan(h) || isnan(t) || isnan(f)) 
      {
       Serial.println(F("Failed to read from DHT sensor!"));
       return;
      }

      oled.setTextSize(1);
      oled.setTextColor(WHITE);
      oled.setCursor(0, 0);
      oled.print("Humidity");

      oled.setTextSize(2);
      oled.setTextColor(WHITE);
      oled.setCursor(20, 15);
      oled.print(h);
      oled.print(" %");

      oled.setTextSize(1);
      oled.setTextColor(WHITE);
      oled.setCursor(0, 35);
      oled.print("Temperature");

      oled.setTextSize(2);
      oled.setTextColor(WHITE);
      oled.setCursor(20, 50);
      oled.print(t);
      oled.print(" C");
      oled.display();
      oled.clearDisplay();
    }
    break;   
  }
}