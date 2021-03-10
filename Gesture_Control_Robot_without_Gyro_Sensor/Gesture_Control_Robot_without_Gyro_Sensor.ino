/* Code devloped by Jayasurya Obbu */
/* This code is open-source */
/* Any doubts regarding code, just mail to jay@immortalelectronics.in (or) hello@jayasurya.me */
/* Dated 04 Oct 2015 */

const int buttonPin1 = 10;
const int buttonPin2 = 11;
int senPin=A0;

int M101 = 2;
int M102 = 3;
int M201 = 4;
int M202 = 5;

void setup() 
{
pinMode(M101,OUTPUT);
pinMode(M102,OUTPUT);
pinMode(M201,OUTPUT);
pinMode(M202,OUTPUT);
pinMode(buttonPin1,INPUT);
pinMode(buttonPin2,INPUT);
pinMode(senPin,INPUT);
Serial.begin(9600);
}

void loop() 
{
int senVal=analogRead(senPin);
int buttonState1 = digitalRead(buttonPin1);
int buttonState2 = digitalRead(buttonPin2);

if(buttonState1==0){
 Serial.println("one");
 
  if(senVal<400){
    digitalWrite(M101, HIGH);
    digitalWrite(M102, LOW);
    digitalWrite(M201, LOW);
    digitalWrite(M202, HIGH);
    Serial.println("front left");
  }
  else if(senVal>850){
    digitalWrite(M101, LOW);
    digitalWrite(M102, HIGH);
    digitalWrite(M201, HIGH);
    digitalWrite(M202, LOW);
    Serial.println("front right");
  }
  else{
     digitalWrite(M101, HIGH);
    digitalWrite(M102, LOW);
    digitalWrite(M201, HIGH);
    digitalWrite(M202, LOW);
    Serial.println("front ");
  }  
}
else if(buttonState2==0){
   Serial.println("two");
 if(senVal<400){
   
     digitalWrite(M101, LOW);
    digitalWrite(M102, HIGH);
    digitalWrite(M201, HIGH);
    digitalWrite(M202, LOW);
    Serial.println("back left");
  }
else if(senVal>850){
  digitalWrite(M101, HIGH);
    digitalWrite(M102, LOW);
    digitalWrite(M201, LOW);
    digitalWrite(M202, HIGH);
    Serial.println("baxck right");
  }
  else{
     digitalWrite(M202, HIGH);
    digitalWrite(M201, LOW);
    digitalWrite(M102, HIGH);
    digitalWrite(M101, LOW);
    Serial.println("bCK");
  }
}
else if(buttonState1==1 || buttonState2==1){
  Serial.println("n");
     digitalWrite(M201, LOW);
    digitalWrite(M202, LOW);
    digitalWrite(M101, LOW);
    digitalWrite(M102, LOW);
//Serial.println("STOP");
}
Serial.println(senVal);
//delay(1000);
}
