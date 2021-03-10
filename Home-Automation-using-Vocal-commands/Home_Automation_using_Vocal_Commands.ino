/* Code devloped by Jayasurya Obbu on behalf of Immortal Electronics */
/* This code is open-source */
/* Any doubts regarding code, just mail to jay@immortalelectronics.in (or) hello@jayasurya.me */
/* Dated 04 Aug 2016 */
/* No Header Files Used */


/* Declaring Variables */
String voice;

/* Initializing Pin Numbers */
int
Relay1 = 12, 
Relay2 = 11, 
Relay3 = 10, 
Relay4 = 9;

/* User Defined Function */
void allon()
 {
     digitalWrite(Relay1, HIGH);
     digitalWrite(Relay2, HIGH);
     digitalWrite(Relay3, HIGH);
     digitalWrite(Relay4, HIGH);
 }

/* User Defined Function */
void alloff()
 {
     digitalWrite(Relay1, LOW);
     digitalWrite(Relay2, LOW);
     digitalWrite(Relay3, LOW);
     digitalWrite(Relay4, LOW);
 }

/* Setup Function */
void setup()
 {
     Serial.begin(9600);
     pinMode(Relay1, OUTPUT);
     pinMode(Relay2, OUTPUT);
     pinMode(Relay3, OUTPUT);
     pinMode(Relay4, OUTPUT);
 }

/* Loop Function */
void loop() 
 {
     while (Serial.available())
      {  
          delay(10); 
          char c = Serial.read(); 
          if (c == '#') {break;} 
          voice += c; 
      } 
     if (voice.length() > 0) 
      {
          Serial.println(voice);
          if(voice == "*all on") {allon();}  
          else if(voice == "*all off"){alloff();} 
          else if(voice == "*light on") {digitalWrite(Relay1, HIGH);}
          else if(voice == "*light off") {digitalWrite(Relay1, LOW);}
          else if(voice == "*fan on") {digitalWrite(Relay2, HIGH);}
          else if(voice == "*fan off") {digitalWrite(Relay2, LOW);}
          else if(voice == "*television on") {digitalWrite(Relay3, HIGH);}
          else if(voice == "*television off") {digitalWrite(Relay3, LOW);}
          else if(voice == "*charger on") {digitalWrite(Relay4, HIGH);}
          else if(voice == "*charger off") {digitalWrite(Relay4, LOW);}
          voice="";
      }
 }
