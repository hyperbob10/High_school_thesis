#include <LiquidCrystal_I2C.h>
#include "motori.h"
#include "costanti.h"
#include "IR.h"
#include "tcs.h"
#include "gy521.h"
#include "ultrasonic.h"
#include "sensorbar.h"
#include "motori.h"

#define dist 10

LiquidCrystal_I2C lcd(0x27, 16, 2);
motor motor;
IR IR;
TCS tcs;
ultra ultra;
sensorbar sensorbar;
gy521 gyro('z');
gy521 y('y');

const float KP=4;
const float KD=0.075;
const float KI=0.100;
const float KTOT=350;
const float aim=0;

int avg_speed = 160; 
int i, on=1,a;
char f;
unsigned long timeA=0, timeB;
float line, error, error1=0, P, I=0, D, dt, angle=0, angle_off=0, angle1=0, offset, offset_y, e;
int PID, ostacolo, dx, sx;

char in[5]={DX_OUT,SX_OUT,ULTRA_FWD,ULTRA_SX,ULTRA_DX};
char out[13]={M1INA,M1INB,M2INA,M2INB,PWM1,PWM2,SX_S2,SX_S3,DX_S2,DX_S3,TRIG_FWD,TRIG_DX,TRIG_SX};
int v_dx[4]={-300,300,300,-300};
int times[4]={2600,6500,2400,1};
int times_dx[4]={2600,6200,2700,1};

float greenAccumulator = 0;

void setup()
{
    //set input pins
    for(i=0;i<5;i++) 
      pinMode(in[i],INPUT);

    //set output pins
    for(i=0;i<13;i++) 
      pinMode(out[i],OUTPUT);

    //Serial.begin(9600);
    lcd.begin();
    lcd.backlight();
    lcd.setCursor(0,0); lcd.print("Calibrating");
    lcd.setCursor(0,1); lcd.print("gyroscope");
     
    //stopping the robot
    motor.speed12(0,0);
    
    //offset=-10.17;
    offset = gyro.setGyro();
    //offset_y = 19.7;
    gyro.set(1000);
    y.set(1000);

    //print on LCD display
    lcd.clear(); lcd.setCursor(0,0); lcd.print("FOLLOWING LINE"); delay(200);
    
}


void loop()
{
  ostacolo = ultra.FWD();
  timeB = millis();
  //compute the time step
  dt = ((float)(timeB-timeA))/1000;      
  //check if obstacle is near to the robot
  if (ostacolo >= dist)
  {
    e = IR.position();
    f = IR.digital_position();

    int h=1023-analogRead(3);
    int i=1023-analogRead(4);
    int l=1023-analogRead(5);

    //************** LINE DETECTION *********//
    if (f != 4)
    {
      switch(f) 
      {
        case(1) :  {line = 0; motor.speed12(300,300); /*delay(50); motor.speed12(0,0);*/} break;;
        case(2) :  line=300; break;
        case(3) :  line=-300;   
      }
    }
    
    else 
      line=(-1)*e;

    if (line != 0) 
    {
      //Compute the PID
      error=aim - line;
      I=((error + error1) * dt) / 2 + I;   
      D=KD * (error1 - error) / dt;         
      P=KP * error;                     
      PID=(int) (P + I * KI + D) * KTOT;
      error1 = error;

      motor.speed12((avg_speed-PID),(avg_speed+PID));
    } 
       
    else 
    {
      motor.speed12(avg_speed,avg_speed);
      line=0;
    }
              
  }


  else
  {
    lcd.clear(); lcd.setCursor(0,0); lcd.print("THERE'S AN"); lcd.setCursor(0,1); lcd.print("OBSTACLE!");
    //stop for 200 ms
    motor.speed12(0,0);   
    delay(200);

    //read the two lateral ultrasound sensors
    dx = ultra.DX();     
    sx = ultra.SX();

    if(sx > 20)  
      i=-1;
    else 
      i=1; 

    //bypass the obstacle in front of the robot
    for(a=0 ; a<4 ; a++)
    {
      //reference angle
      angle_off = gyro.gyroA(angle_off,offset);    
      //current rotation angle
      angle = gyro.gyroA(angle_off,offset);                   
      float rot = abs((angle - angle_off));
      
      //do a 90 degrees turn
      while(rot < 90) 
      {
        motor.speed12(i*(-1)*v_dx[a],i*v_dx[a]); delay(50); motor.speed12(0,0); delay(0);
        angle = gyro.gyroA(angle,offset);
        rot = abs(angle - angle_off);
      }

      motor.speed12(200,200); 
      if(i == -1) 
        delay(times[a]); 
      else 
        delay(times_dx[a]); 
                
      motor.speed12(0,0); delay(200);
    }
    I=0; D=0;
    lcd.clear(); lcd.setCursor(0,0); lcd.print("FOLLOWING LINE");
  }

  timeA=timeB;     
}

