#include <Otto.h>
Otto Otto;

#define LeftLeg 2
#define RightLeg 3 
#define LeftFoot 4 
#define RightFoot 5 
#define Buzzer 13 

void setup() 
{
  Otto.init(LeftLeg, RightLeg, LeftFoot, RightFoot, true, Buzzer);
  Otto.home();
}

void loop()
{
    for (int count=0 ; count<2 ; count++) 
    {
      Otto.walk(1,1000,1); 
      Otto.walk(1,1000,-1); 
    }
    for (int count=0 ; count<2 ; count++) 
    {
      Otto.shakeLeg(1,1000,-1);
      Otto.shakeLeg(1,1000,1);
    }
    Otto.jump(1,1000);
    Otto.playGesture(OttoSuperHappy);
    Otto.playGesture(OttoVictory);
    Otto.playGesture(OttoConfused);
    Otto.playGesture(OttoLove);
    Otto.playGesture(OttoAngry);
    Otto.playGesture(OttoFail);
}
