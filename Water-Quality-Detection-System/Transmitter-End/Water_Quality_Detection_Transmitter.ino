/* Code devloped by Jayasurya Obbu on behalf of Immortal Electronics */
/* This code is open-source */
/* Any doubts regarding code, just mail to jay@immortalelectronics.in (or) hello@jayasurya.me */
/* Dated 16 Mar 2018 */
/* Download the Header Files from the root folder */

/* Including Header Files */
#include "VirtualWire.h"
#include "string.h"
#include <OneWire.h>
#include <DallasTemperature.h>

/* Initializing Pin Numbers */
const int transmit_pin = 12; //TX
const int AOUTpin=A0; //Co2
const int analogInPin = A1; //pH
const int TurbidityPin = A2; // Turbidity
const int trigPin = 9; // UltraSonic
const int echoPin = 10; // UltraSonic
#define ONE_WIRE_BUS 7 //Temperature

/* Declaring Variables */
int sensorValue = 0; //pH
unsigned long int avgValue; //pH
float b; //pH
int buf[10],temp; //pH
int value; //Co2
char buffC[20], buffP[20], buffT[20], buffU[20], buffD[20], buff[100];

/* Creating Object Instances */
OneWire oneWire(ONE_WIRE_BUS); // Temperature
DallasTemperature sensors(&oneWire); //TEmperature

/* Setup Function */
void setup()
{
   vw_set_tx_pin(transmit_pin); //Tx
   vw_set_ptt_inverted(true); //Tx
   vw_setup(2000);   //Tx
   Serial.begin(9600); //Serial Data
   pinMode(trigPin, OUTPUT);// Ultra Sonic
   pinMode(echoPin, INPUT); // Ultra Sonic
   sensors.begin(); // Temperature
}

/* Loop Function */
void loop()
{
  value= analogRead(AOUTpin);//Co2
  Serial.print("Co2 Value = ");
  Serial.println(value);
  dtostrf(value, 3,0, buffC);
  Serial.print("Co2 Value= "); 
  Serial.println(buffC);
  
  for(int i=0;i<10;i++) //pH
  { 
    buf[i]=analogRead(analogInPin);
    delay(10);
  }
  for(int i=0;i<9;i++)
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)
     avgValue+=buf[i];
  float pHVol=(float)avgValue*5.0/1024/6;
  float phValue = -5.70 * pHVol + 21.34;
  Serial.print("pH Value = ");
  Serial.println(phValue);
  delay(20);
  dtostrf(phValue, 5,4, buffP);
  Serial.print("pH Value= "); 
  Serial.println(buffP);

  int turbidity = analogRead(A2); //Turbidity
  Serial.print("Turbidity Value = ");
  Serial.println(turbidity);
  delay(500);
  dtostrf(turbidity, 3,0, buffT);
  Serial.print("Turbidity Value= "); 
  Serial.println(buffT);

  long duration, inches, cm; // UltraSonic
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(100);
  dtostrf(cm, 2,0, buffU);
  Serial.print("Cm= "); 
  Serial.println(buffU);
  
  sensors.requestTemperatures(); //Temperature
  Serial.print("Temperature is: ");
  Serial.print(sensors.getTempCByIndex(0));
  delay(1000);
  dtostrf(sensors.getTempCByIndex(0), 4,2, buffD);
  Serial.print("Temperature = "); 
  Serial.println(buffD);

  buff[0]='\0';
  strcat(buff, buffC);
  strcat(buff, " ");
  strcat(buff, buffP);
  strcat(buff, " ");
  strcat(buff, buffT);
  strcat(buff, " ");
  strcat(buff, buffU);
  strcat(buff, " ");
  strcat(buff, buffD);

  Serial.print("Readings= "); 
  Serial.println(buff);

  vw_send((byte *)buff, strlen(buff));
  vw_wait_tx();
  delay(5000);
}

/* User Defined Function */
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}

