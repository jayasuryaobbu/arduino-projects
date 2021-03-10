/* Code devloped by Jayasurya Obbu on behalf of Immortal Electronics*/
/* This code is open-source */
/* Any doubts regarding code, just mail to jay@immortalelectronics.in (or) hello@jayasurya.me */
/* Dated 04 Oct 2015 */
/* Header Files can be downloaded from the root folder */

/* Including Header Files */
#include <Password.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

/* Creating Object Instances */
Servo myservo;

/* Declaring Variables and Initializing Values */
int greenLED = 22;
int redLED = 23;
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
Password password = Password( "1111" );
int currentPosition = 0;
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {
    '1','2','3','A',  }
  ,
  {
    '4','5','6','B',  }
  ,
  {
    '7','8','9','C',  }
  ,
  {
    '*','0','#','D',  }
};
byte rowPins[ROWS] = {
  40, 41, 42, 43}; 
byte colPins[COLS] = {
  47, 46, 45, 44}; 
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

/* Setup Function */
void setup(){
  lcd.begin(16, 2);
  Serial.begin(9600);
  keypad.setDebounceTime(50);
  displayCodeEntryScreen();
  keypad.addEventListener(keypadEvent); 
  myservo.attach(2);
  myservo.write(180);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
}

/* User Defined Function */
void displayCodeEntryScreen()
{
  password.reset();
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Enter Code:");
  lcd.setCursor(0,1);
  keypad.addEventListener(keypadEvent); 
  //setup and turn off both LEDs
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
}

/* Loop Function */
void loop(){
  keypad.getKey();
}

/* User Defined Function */
void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
  case PRESSED:
    lcd.print(eKey);
    switch (eKey){
    case '#': 
      checkPassword();
      break;
    case '*': 
         displayCodeEntryScreen(); break;
    default:
      password.append(eKey);
    }
 switch (keypad.getState()){
   case PRESSED:
   switch (eKey){
   case 'D': myservo.write(180); displayCodeEntryScreen();
    }
  }
  }
}

/* User Defined Function */
void checkPassword(){
  if (password.evaluate()){
    digitalWrite(greenLED, HIGH);
    lcd.clear();
    delay(30);
    lcd.setCursor(1, 0);
    lcd.print("Acces Granted");
    lcd.setCursor(4, 1);
    lcd.print("Welcome");
    unlockdoor();
    delay(2500);
    displayCodeEntryScreen();
  }
  else{
    loop(); {
      redlight();
    }
    lcd.clear();
    delay(10);
    lcd.setCursor(2, 0);
    lcd.print("Acces Denied");
    delay(2500);
    lcd.clear();
    displayCodeEntryScreen();
  }
}

/* User Defined Function */
void unlockdoor(){ 
  myservo.write(90);
  digitalWrite(greenLED, HIGH);
  delay(5000);
}

/* User Defined Function */
void redlight()
{
  int f=0;
  for(f=0;f<=8;f++)
  {
    digitalWrite(redLED, HIGH);
    delay(100);
    digitalWrite(redLED, LOW);
    delay(100);
  }
}



