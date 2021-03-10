/* Code devloped by Jayasurya Obbu on behalf of Immortal Electronics */
/* This code is open-source */
/* Any doubts regarding code, just mail to jay@immortalelectronics.in (or) hello@jayasurya.me */
/* Dated 13 Apr 2018 */
/* Download the Header Files from the root folder */


#define USE_ARDUINO_INTERRUPTS true // Setting up Low-Level Interrupts for getting accurate results from Pulse Sensor  

/* Including Header Files */
#include <PulseSensorPlayground.h>    
#include "SevenSegmentTM1637.h"

/* Initializing Pin Numbers */
const int PulseWire = 0;                     
const byte PIN_CLK = 9;   
const byte PIN_DIO = 10; 

/* Declaring Variables */
int Threshold = 550;

/* Creating Object Instances */
SevenSegmentTM1637    display(PIN_CLK, PIN_DIO);                                     
PulseSensorPlayground pulseSensor;  

/* Setup Function */
void setup() 
{   
  Serial.begin(9600); 
  display.begin();            
  display.setBacklight(100);        
  pulseSensor.analogInput(PulseWire);         
  pulseSensor.setThreshold(Threshold);   
  
  if (pulseSensor.begin()) 
  {
      display.print("BP COUnT");      
      delay(1000);  
  }
}

/* Loop Function */
void loop() 
{
 int myBPM = pulseSensor.getBeatsPerMinute();  

 if (pulseSensor.sawStartOfBeat() && (myBPM >60 && myBPM < 90)) 
 {            
    display.print(myBPM);                     
    display.print("c1");
 }
 if (pulseSensor.sawStartOfBeat() && myBPM < 60) 
 {            
    display.print(myBPM);                     
    display.print("c2");
 }
 if (pulseSensor.sawStartOfBeat() && myBPM >90) 
 {            
    display.print(myBPM);                     
    display.print("c3");
 }
 delay(20);                  
}
