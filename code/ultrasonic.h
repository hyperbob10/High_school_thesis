#ifndef ultra_h
#define ultra_h

#include <Arduino.h>
#include "costanti.h"

class ultra
{
  long time;
  int dist;
  public:

  int DX()
  {
    digitalWrite(TRIG_DX,0);   
    digitalWrite(TRIG_DX,1);
    delay(10);
    digitalWrite(TRIG_DX,0);

    time=pulseIn(ULTRA_DX,1);
    dist=0.034 * time/2;

    if (dist>50)   dist=50;
    return dist;
  }

    int SX()
  {
    digitalWrite(TRIG_SX,0);   
    digitalWrite(TRIG_SX,1);
    delay(10);
    digitalWrite(TRIG_SX,0);

    time=pulseIn(ULTRA_SX,1);
    dist=0.034 * time/2;

    if (dist>50)   dist=50;
    return dist;
    }

    int FWD()
   {
    digitalWrite(TRIG_FWD,0);   
    digitalWrite(TRIG_FWD,1);
    delay(10);
    digitalWrite(TRIG_FWD,0);

    time=pulseIn(ULTRA_FWD,1);
    dist=0.034 * time/2;

    if (dist>50)   dist=50;
    return dist;
    }

  private:
  
};
#endif
