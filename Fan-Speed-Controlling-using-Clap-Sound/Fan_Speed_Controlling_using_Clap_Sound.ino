/* Code devloped by Jayasurya Obbu on behalf of Immortal Electronics */
/* This code is open-source */
/* Any doubts regarding code, just mail to jay@immortalelectronics.in (or) hello@jayasurya.me */
/* Dated 30 Mar 2018 */
/* Download the Header Files from the root folder */

/* Including Header Files */
#include <Servo.h>

/* Creating Object Instances */
Servo myservo;

/* Declaring Variables */
int clapcount = 0;

/* Setup Function */
void setup() 
{
  Serial.begin(9600);
  myservo.attach(9);
}

/* Loop Function */
void loop() 
{
  int soundReading = analogRead(A0);
  int sound = map(soundReading, 0, 1023, 0, 100);
  if(sound > 97)
  {
    clapcount++;
  }
  switch (clapcount) 
  {
    case 1: 
      myservo.write(36);
      break;
    case 2:    
      myservo.write(72);
      break;
    case 3:    
      myservo.write(108);
      break;
    case 4:    
      myservo.write(144);
      break;
    case 5: 
      myservo.write(180);
      break;
    case 6:    
      myservo.write(144);
      break;
    case 7:    
      myservo.write(108);
      break;
    case 8:    
      myservo.write(72);
      break;
    case 9:    
      myservo.write(36);
      break;
    case 10:    
      myservo.write(0);
      break;
    default:    
      clapcount = 0;
      break;
  }
  delay(100);
}
