#ifndef sensorbar_h
#define sensorbar_h
#include <Arduino.h>
#include <SPI.h>
#include "costanti.h"

#define CS 53


class sensorbar
{
  
  int sensor[7]={0,1,2,3,6,5,4};
  int value[7]={0,0,0,0,0,0,0};
  char peso[7]={64,32,16,8,4,2,1};
  char variabile=0,posizione;

  public:
  
  void init()
  {    
    pinMode(CS,OUTPUT);
    SPI.begin();
   }


char position(int Vbianco)
{
  for (char i=0;i<7;i++) 
  {
  value[i]=AD7888_read(sensor[i]);
  if(value[i]>Vbianco) value[i]=1;
  else value[i]=0; 
  variabile=variabile+peso[i]*(char)value[i];
  }

  posizione=(-30*value[0]-20*value[1]-10*value[2]+10*value[4]+20*value[5]+30*value[6])/(value[0]+value[1]+value[2]+value[3]+value[4]+value[5]+value[6]);

  return posizione;
}





private:


uint8_t AD7888_buildCTRLreg(int channel, int ref, int powerMode)
{
  // Based on AD7888 datasheet
  uint8_t ctrlReg = 0x00;
  if (ref) ctrlReg |= (1 << 2);     // Set REF bit for external reference
  ctrlReg |= (channel & 0x07) << 3;   // Set the ADD2, ADD1 and ADD0 bits to set the channel
  ctrlReg |= (powerMode & 0x03) << 0; // Set PM1 and PM0 bits to set power mode
  ctrlReg &= ~(1 << 6);         // Clear the ZERO bit to ensure normal operation
  return ctrlReg;
}

int AD7888_read(int channel)
{
  uint8_t tx[2];
  uint8_t rx[2];

  tx[0] = AD7888_buildCTRLreg(channel, 1, 0);
  tx[1] = 0x00;

  // Invio il settaggio del registro di controllo dell'ADC per leggere il canale voluto
  digitalWrite(CS, LOW);
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  // Invia 2 bytes dal buffer tx
  SPI.transfer(&tx[0], 2);
  // Chiudo transazione
  SPI.endTransaction();
  digitalWrite(CS, HIGH);

  // Aspetto giusto un attimo per far acquisire il CS
  delayMicroseconds(10);

  // Invia un payload dummy per leggere il risultato della conversione
  digitalWrite(CS, LOW);
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  // Invio da tx e ricevo in rx (2 bytes)
  rx[0] = SPI.transfer(tx[0]);
  rx[1] = SPI.transfer(tx[1]);
  // Chiudo transazione
  SPI.endTransaction();
  digitalWrite(CS, HIGH);

  // Ora nel buffer rx ho il risultato
  // Ricombino i 2 byte in un intero 16 bit
  uint16_t d = ((((uint16_t) rx[0]) << 8) & 0xFF00) | (((uint16_t) rx[1]) & 0x00FF);

  // Strip dei primi 4 bit che non contengono informazioni utili (per sicurezza)
  d = d & 0x0FFF;

  // Ritorno il valore
  return ((int) d);
}
 
};
 #endif




  
