#ifndef gy521_h
#define gy521_h

#include <Arduino.h>
#include <Wire.h>

#define MPU_address      0x68 //Address of MPU-6050
#define HMC6352_address  0x21 //Address of compass HMC6352


#define gyro_config 0x1B

#define acX_H 0x3B
#define acY_H 0x3D
#define acZ_H 0x3F

#define tmp_H 0x41

#define gyX_H 0x43
#define gyY_H 0x45
#define gyZ_H 0x47


#define cmp_H 0x41
#define cmp_L 0x42

class gy521
{
  public:

    double dt;
    unsigned long now,pre;
    int scaleRange;
    float k_gyro;
    float zero;
    int axis;
    
    gy521(char a)
    {
      Wire.begin();
      
      switch(a)
      {
        case 'x': axis = gyX_H; break;
        case 'y': axis = gyY_H; break;
        case 'z': axis = gyZ_H;
      }
      
    }

    void set(float value)
    {
      Wire.beginTransmission(MPU_address);
      Wire.write(0x6B);  // PWR_MGMT_1 register
      Wire.write(0); // wakes up MPU-6050
      Wire.endTransmission(true);

      scaleRange=value;
      k_gyro= scaleRange/32768.0f;          //set k_gyro for scale select
      Wire.beginTransmission(MPU_address);
      Wire.write(gyro_config);  //Set scale range

      switch(scaleRange){
        case 250 : Wire.write(0x00); break;
        case 500 : Wire.write(0x08); break;
        case 1000: Wire.write(0x10); break;
        case 2000: Wire.write(0x18); break;
      }
      Wire.endTransmission(true);

      delay(800); //wait for the sensor to be ready
      pre=micros();
    }
    
    float setGyro(){
      
      float value=0;
      
      for( int i=0 ; i<1000 ; i++)
      {
        value =value+ readValue(MPU_address,axis);
        delay(5);
      }
      value= value/1000;
      
      pre=micros();
      
      return -value;
    }
    
    void setComp()
    {
      int16_t value;
      delay(15);
      value=readValue(HMC6352_address, cmp_H);
      
      zero=float (value) / 10;
    }

    float gyroW(float offset)
    {
      int16_t value;

      value = readValue(MPU_address, axis);

      float res = (((float) value) + offset) * k_gyro; 
      
      return res;
    }

    float gyroA(float alfa, float offset)
    {
      float w=0;

      now=micros();
      dt=float((now-pre))/1000000;

      w=gyroW(offset);
      alfa=alfa+(w*dt);

      pre=now;

      if(alfa> 180)  alfa-=360;
      if(alfa<-180)  alfa+=360;

      return alfa;
    }

    float temp()
    {
      int16_t value;

      value=readValue(MPU_address, tmp_H);
      
      return  float(value/340.00f)+36.53f  ;
    }

    float compass()
    {

      int16_t asdf;
      float value;
      
      asdf=readValue(HMC6352_address, cmp_H);
      value= float(asdf )/ 10;
      delay(1);

      value-=zero;
      if(value<0) value+=360;
      
      if(value> 180)      value-=360;
      else if(value<-180)  value+=360;
      
      return -value;
    }
    
  private:
    int16_t readValue(int16_t deviceAddress, byte address)
    {
      int16_t value = 0;
      
      Wire.beginTransmission(deviceAddress);
      Wire.write(address); // register to read
      Wire.endTransmission(false);
      
      Wire.requestFrom(deviceAddress, 2, true); // read a byte
      
      byte MSB = Wire.read();
      byte LSB = Wire.read();

      return value = ((MSB << 8) | LSB);

    }
};

#endif
