/* Code devloped by Jayasurya Obbu*/
/* This code is open-source */
/* Any doubts regarding code, just mail to jay@immortalelectronics.in (or) hello@jayasurya.me */
/* Dated 11 Apr 2017 */

/* Initializing Pin Numbers */
int Relay = 7; 
int Buzzer = 8; 
int mq3_analogPin = A0; 
int isObstaclePin = 13; 

/* Declaring Variables */
int isObstacle = HIGH; 

/* Setup Function */
void setup()  
{ 
pinMode(isObstaclePin, INPUT); 
pinMode(Relay, OUTPUT);
pinMode(Buzzer, OUTPUT); 
Serial.begin(9600); 
} 

/* Loop Function */
void loop() 
{ 
int mq3_value = analogRead(mq3_analogPin); 
   isObstacle = digitalRead(isObstaclePin); 
   
   if (mq3_value > 15) 
   {   
      digitalWrite(Relay, HIGH); 
      digitalWrite(Buzzer, HIGH); 
   } 
 
   else if(mq3_value <= 15) 
   { 
        if (isObstacle == LOW ) 
          { 
             digitalWrite(Relay, HIGH); 
             digitalWrite(Buzzer, HIGH);  
          } 
          
       else if(isObstacle == HIGH) 
          {  
      digitalWrite(Relay, LOW); 
             digitalWrite(Buzzer, LOW); 
          }  
   } 
  delay(2000); 
}
