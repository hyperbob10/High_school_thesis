#ifndef IR_h
#define IR_h

#include <Arduino.h>

class IR
{
  int sens_an[9] ;
  char sens_d[9] = {0,0,0,0,0,0,0,0,0};
  char sum_dx, sum_sx;
  char i;
  float pesi[9]={-5.6, -4.2, -2.8, -1.4, 0, 1.4, 2.8, 4.2, 5.6};
  float num=0, somma=0, media=0;
  
  public:

     void read()
       {
         num = 0;
         somma =  0;
         for(i=0;i<9;i++)
          {
            sens_an[i] = 1023 - analogRead(i);
            
            if(sens_an[i] < 100) {sens_an[i]=0; sens_d[i]=0;}
            else if (sens_an[i] > 200)  sens_d[i] = 1;
            else sens_d[i]=sens_d[i];
          }
        }
    
      

      float position()
        {
          read();
          num = 0;
          somma =  0;
          for(i=0;i<9;i++)
           {
             num = num + (float)sens_an[i]*pesi[i];
             somma = somma + (float)sens_an[i];
            }

           if(somma == 0) media = 0;
           else media = num / somma;
          
            return media;
        }


        char digital_position()
        {

          sum_sx = 0;
          sum_dx = 0;  
        
          for(i=0;i<5;i++)  sum_sx=sum_sx+sens_d[i];   
          for(i=4;i<9;i++)  sum_dx=sum_dx+sens_d[i];

          if(sum_sx > 3 && sum_dx<3) i = 2;
          else if(sum_dx > 3 && sum_sx<3 ) i = 3;
          else if (sum_dx>3 && sum_sx>3) i=1;
          else i = 4;

          return i;
         }


  
  };
#endif
