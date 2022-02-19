#include <TimerOne.h>

#include "sineTable.h"
int i = 0;
int x = 0;
bool OK=false;
float h = 0;
 int volvalue=0;
 int battvalue=1024; 
 int temp=0;
int counter;
void sineWave();
void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  Timer1.initialize(100);  // 100uS
  Timer1.pwm(9, 100);
  Timer1.pwm(10, 100);
  Timer1.attachInterrupt(sineWave);
    cli();
  TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (1<<WGM21) | (0<<WGM20);
  sei();
}

void loop() {

}

void sineWave() {
  counter++;
  if (counter == 200) counter = 0;
  if (counter < 100) {
    Timer1.setPwmDuty(9, sineVal[0][counter]);
    Timer1.setPwmDuty(10, 0);
  }
  else {
    Timer1.setPwmDuty(9, 0);
    Timer1.setPwmDuty(10, sineVal[0][counter - 100]);
  }
}

 int Inverter()
{
  temp=0;
  battvalue=1024;
  h=0.1;
  TCCR2A=(1<<COM2A1) | (0<<COM2A0) | (1<<COM2B1) | (0<<COM2B0) | (1<<WGM21) | (0<<WGM20);
  TIMSK2=0x01;
  OCR2A=OCR2B=0x00;
  while(1)
   {
    if(battvalue<575)   //Low batt protect
    temp++;
    else temp=0;
    if(temp>10)
    {
      TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (1<<WGM21) | (0<<WGM20);
      TIMSK2=0x00;
      OCR2A=OCR2B=0x00;
      
    }

   
          while(1)
          {
            TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (1<<WGM21) | (0<<WGM20);
            TIMSK2=0x00;
            OCR2A=OCR2B=0x00;
           
        }
     if(h>0.99 && volvalue<100)  //Short protect
        {
         
          TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (1<<WGM11) | (0<<WGM10);
          TIMSK1=0x00;
          OCR1A=OCR1B=0x00;
        
       }
   }
   
}
ISR(TIMER2_COMPB_vect) {
  
  if (i > 175) {
    i = 0;
    OK = !OK;
    if(OK==0) TCCR2A=(1<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (1<<WGM21) | (0<<WGM20);
    else      TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (1<<COM2B1) | (0<<COM2B0) | (1<<WGM21) | (0<<WGM20);
    
    if(volvalue>512 && h>0.1)  h-=0.01;
    if(volvalue<512 && h<0.99) h+=0.01;
    battvalue=analogRead(A0);
  }
  int a=sineVal[i];
  x = (int)h * a;
  i = i + 1;
  if (OK == 0) {
    OCR2A = x;
  }
  if (OK == 1) {
    OCR2B = x;
  }
  if(i==150)
  {
    volvalue=analogRead(A1);
  }
}
