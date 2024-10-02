const int a = 8;  
const int b = 7;  
const int c = 6;  
const int d = 5;  
const int e = 4;  
const int f = 3;  
const int g = 2; 

bool bPress = false;
const int buttonPin = 9;

int buttonPushCounter = 0;   
int buttonState = 0;         
int lastButtonState = 0;     

void setup() 
{
  pinMode(a, OUTPUT);  //A
  pinMode(b, OUTPUT);  //B
  pinMode(c, OUTPUT);  //C
  pinMode(d, OUTPUT);  //D
  pinMode(e, OUTPUT);  //E
  pinMode(f, OUTPUT);  //F
  pinMode(g, OUTPUT);  //G

  pinMode( buttonPin , INPUT_PULLUP );
  Serial.begin(9600);
  displayDigit(buttonPushCounter);
}

void loop() {

   buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState) 
  {
    if (buttonState == LOW) 
    {      
    bPress = true;
    buttonPushCounter++;
    if( buttonPushCounter > 9) buttonPushCounter =0 ;
    Serial.println("on");
    } 
    
    else 
    {
        Serial.println("off");
    }
        delay(50);
  }
    lastButtonState = buttonState;
  if( bPress ){
     turnOff();
     displayDigit(buttonPushCounter);
  }
}

void displayDigit(int digit)
{
 if(digit!=1 && digit != 4)
 digitalWrite(a,HIGH);

 if(digit != 5 && digit != 6)
 digitalWrite(b,HIGH);

 if(digit !=2)
 digitalWrite(c,HIGH);

 if(digit != 1 && digit !=4 && digit !=7)
 digitalWrite(d,HIGH);

 if(digit == 2 || digit ==6 || digit == 8 || digit==0)
 digitalWrite(e,HIGH);

 if(digit != 1 && digit !=2 && digit!=3 && digit !=7)
 digitalWrite(f,HIGH);

 if (digit!=0 && digit!=1 && digit !=7)
 digitalWrite(g,HIGH);

}
void turnOff()
{
  digitalWrite(a,LOW);
  digitalWrite(b,LOW);
  digitalWrite(c,LOW);
  digitalWrite(d,LOW);
  digitalWrite(e,LOW);
  digitalWrite(f,LOW);
  digitalWrite(g,LOW);
}