/* Code devloped by Jayasurya Obbu on behalf of Immortal Electronics */
/* This code is open-source */
/* Any doubts regarding code, just mail to jay@immortalelectronics.in (or) hello@jayasurya.me */
/* Dated 30 Mar 2018 */
/* Download the used Header File from -- https://github.com/arduino-libraries/LiquidCrystal */


/* Including Header Files */
#include <LiquidCrystal.h>

/* Initializing Pin Numbers */
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#define DT 3
#define SCK 2
#define sw 12
#define buzzer 13
const int motorPin1  = 10; 
const int motorPin2  = 11; 

/* Declaring Variables */
long sample=0;
float val=0;
long count=0;

/* User Defined Function */
unsigned long readCount(void)
{
  unsigned long Count;
  unsigned char i;
  pinMode(DT, OUTPUT);
  digitalWrite(DT,HIGH);
  digitalWrite(SCK,LOW);
  Count=0;
  pinMode(DT, INPUT);
  while(digitalRead(DT));
  for (i=0;i<24;i++)
  {
    digitalWrite(SCK,HIGH);
    Count=Count<<1;
    digitalWrite(SCK,LOW);
    if(digitalRead(DT)) 
    Count++;
  }
  digitalWrite(SCK,HIGH);
  Count=Count^0x800000;
  digitalWrite(SCK,LOW);
  return(Count);
}

/* Setup Function */
void setup()
{
  Serial.begin(9600);
  pinMode(SCK, OUTPUT);
  pinMode(sw, INPUT_PULLUP);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  lcd.begin(16, 2);
  lcd.print("    Weight ");
  lcd.setCursor(0,1);
  lcd.print(" Measurement ");
  delay(1000);
  lcd.clear();
  calibrate();
}

/* Loop Function */
void loop()
{
  count= readCount();
  int w=(((count-sample)/val)-2*((count-sample)/val));
  Serial.print("weight:");
  Serial.print((int)w);
  Serial.println("g");
  lcd.setCursor(0,0);
  lcd.print("Weight            ");
  lcd.setCursor(0,1);
  lcd.print(w);
  lcd.print("g             ");
  
  if(digitalRead(sw)==0)
  {
    val=0;
    sample=0;
    w=0;
    count=0;
    calibrate();
  }
  
  if(w>=200)
  {
    digitalWrite(buzzer,HIGH);
    lcd.setCursor(0,0);
    lcd.print("Attention");
    lcd.setCursor(0,1);
    lcd.print("Person Drained");
    SendMessage();
    MotorMovement();    
  }
  if(w<200)
  {
    digitalWrite(buzzer,LOW);
  }
}

/* User Defined Function */
void SendMessage()
{
  Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial.println("AT+CMGS=\"+917036631604\"\r"); // Replace x with mobile number
  delay(1000);
  Serial.println("Person drowned in water, please attend immediatly");// The SMS text you want to send
  delay(100);
  Serial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

/* User Defined Function */
void MotorMovement()
{
    digitalWrite(motorPin1, HIGH);
    digitalWrite(motorPin2, LOW);
    delay(2000); 
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
    delay(10000); 
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, HIGH);
    delay(2000);
    digitalWrite(motorPin1, LOW);
    digitalWrite(motorPin2, LOW);
}

/* User Defined Function */
void calibrate()
{
  lcd.clear();
  lcd.print("Calibrating...");
  lcd.setCursor(0,1);
  lcd.print("Please Wait...");
  for(int i=0;i<100;i++)
  {
    count=readCount();
    sample+=count;
    Serial.println(count);
  }
  sample/=100;
  Serial.print("Avg:");
  Serial.println(sample);
  lcd.clear();
  lcd.print("Put 100g & wait");
  count=0;
  while(count<1000)
  {
    count=readCount();
    count=sample-count;
    Serial.println(count);
  }
  lcd.clear();
  lcd.print("Please Wait....");
  delay(2000);
  for(int i=0;i<100;i++)
  {
    count=readCount();
    val+=sample-count;
    Serial.println(sample-count);
  }
  val=val/100.0;
  val=val/100.0;        
  lcd.clear();
}
