#ifndef TCS_h
#define TCS_h


#include <Arduino.h>
#include "costanti.h"

class TCS
{ 
  public:
  
  unsigned long blue, green, red;
  float FG, FR, FB;

  
  void pin()
  {
  
   pinMode(SX_S2,OUTPUT);
   pinMode(SX_S3,OUTPUT);
   pinMode(SX_OUT,INPUT);
   pinMode(DX_S2,OUTPUT);
   pinMode(DX_S3,OUTPUT);
   pinMode(DX_OUT,INPUT);
  }
 /*
 void scale(char scale)
  {
   switch(scale)
   {
        case 0 :  {digitalWrite(S0,0); digitalWrite(S1,0);} break;
        case 2 :  {digitalWrite(S0,0); digitalWrite(S1,1);} break;
        case 20:  {digitalWrite(S0,1); digitalWrite(S1,0);} break;
        case 100: {digitalWrite(S0,1); digitalWrite(S1,1);} break;   
   }    
  }*/
  
  void setRed_D()
 {
   digitalWrite(DX_S2, LOW);  
   digitalWrite(DX_S3, LOW);  
 }
 
  void setGreen_D()
 {
   digitalWrite(DX_S2, HIGH);  
   digitalWrite(DX_S3, HIGH);  
 }
 
  void setBlue_D()
 {
   digitalWrite(DX_S2, LOW);  
   digitalWrite(DX_S3, HIGH);  
 }

 void setRed_S()
 {
   digitalWrite(SX_S2, LOW);  
   digitalWrite(SX_S3, LOW);  
 }
 
  void setGreen_S()
 {
   digitalWrite(SX_S2, HIGH);  
   digitalWrite(SX_S3, HIGH);  
 }
 
  void setBlue_S()
 {
   digitalWrite(SX_S2, LOW);  
   digitalWrite(SX_S3, HIGH);  
 }
  
  
  
  float Red_DX()  
 {    
  setRed_D();
  //count OUT, pRed, RED  
  red = 2*pulseIn(DX_OUT, digitalRead(DX_OUT) == HIGH ? LOW : HIGH);  
  FR=(1/(float)red)*1000000;
  return FR; 
 }
 
 float Blue_DX()
 {
  setBlue_D();
  //count OUT, pBLUE, BLUE  
  blue =2* pulseIn(DX_OUT, digitalRead(DX_OUT) == HIGH ? LOW : HIGH);  
  FB=(1/(float)blue)*1000000;
  return FB;
 }
 
 float Green_DX()
 {
  setGreen_D();  
  //count OUT, pGreen, GREEN  
  green = 2*pulseIn(DX_OUT, digitalRead(DX_OUT) == HIGH ? LOW : HIGH);  //calculate the period of the square wave
  FG=(1/(float)green)*1000000; //calculate ther frequency
  return FG;
 }


  float Red_SX()  
 {    
  setRed_S();
  //count OUT, pRed, RED  
  red = 2*pulseIn(SX_OUT, digitalRead(SX_OUT) == HIGH ? LOW : HIGH);  
  FR=(1/(float)red)*1000000;
  return FR; 
 }
 
 float Blue_SX()
 {
  setBlue_S();
  //count OUT, pBLUE, BLUE  
  blue =2* pulseIn(SX_OUT, digitalRead(SX_OUT) == HIGH ? LOW : HIGH);  
  FB=(1/(float)blue)*1000000;
  return FB;
 }
 
 float Green_SX()
 {
  setGreen_S();  
  //count OUT, pGreen, GREEN  
  green = 2*pulseIn(SX_OUT, digitalRead(SX_OUT) == HIGH ? LOW : HIGH);  //calculate the period of the square wave
  FG=(1/(float)green)*1000000; //calculate ther frequency
  return FG;
 }


 
 
};
#endif
   
   
 
