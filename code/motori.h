#ifndef motor_h
#define motor_h

#include <Arduino.h>
#include "costanti.h"

class motor
{
  public: 

  char reverse=0, reverse1=0;
  
  void speedM1(int speed)
  {
    reverse1=0;
    
    if (speed<0) 
      {
        speed=-speed;
        reverse1=1;
      }
      
     if(speed==0)
      {
        digitalWrite(M1INA,0);
       digitalWrite(M1INB,0);
      }
    
     if (speed>400) 
        speed=400;
      
    if (reverse1==0)  {digitalWrite(M1INA,1);  digitalWrite(M1INB,0);}
    if (reverse1==1)  {digitalWrite(M1INA,0);  digitalWrite(M1INB,1);}
    
    analogWrite(PWM1,speed*41/80);
    }
  
  void speedM2(int speed)
  {
    reverse=0;
    
    if (speed<0) 
      {
        speed=-speed;
      reverse=1;
      }
      
     if(speed==0)
      {
        digitalWrite(M2INA,0);
      digitalWrite(M2INB,0);
      }
    
     if (speed>400) 
        speed=400;
      
    if (reverse==0)  {digitalWrite(M2INA,1);  digitalWrite(M2INB,0);}
    if (reverse==1)  {digitalWrite(M2INA,0);  digitalWrite(M2INB,1);}
    
    analogWrite(PWM2,speed*51/80);
  }

  void speed12(int speed1,int speed2)
  {
    speedM1(speed1);
    speedM2(speed2);
  }
  
  
  private:
  
  
};
#endif
