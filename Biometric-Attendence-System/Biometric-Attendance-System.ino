/* Code devloped by Jayasurya Obbu on behalf of Immortal Electronics */
/* This code is open-source */
/* Any doubts regarding code, just mail to jay@immortalelectronics.in (or) hello@jayasurya.me */
/* Dated 28 Sep 2017 */
/* Download the Header Files from the root folder */

/* Including Header Files */
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

/* Declaring Variables */
#define CompareBit_Loc 1023

/* Keypad Keys Assigning */
#define R1 4
#define R2 5
#define R3 6
#define R4 7
#define C1 8
#define C2 9
#define C3 10
#define ROWS 4
#define COLS 3

/* Calling User Defined Function */
int getFingerprintIDez();

/* Arranging the Key Values in Keypad */
static char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

/* Delaring Pin Numbers */
SoftwareSerial mySerial(2, 3);  // pin 2 for Tx and pin 3 for Rx of Finger print
SoftwareSerial gsm(11, 12);
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

/* Creating Object Instance */
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

/* Variable Declartion */
unsigned long PTime = 0;
unsigned char EnrollAuthorisation = 0;
unsigned long EnrollmentDelay = 0;

/* Setup Function */
void setup()
{
  Serial.begin(9600);
  gsm.begin(9600);
  finger.begin(9600);
  lcd.begin(16, 2);
  KeypadInit();
  lcd.clear();
  mySerial.listen();
  mySerial.listen();
  if (finger.verifyPassword())
  {
    lcd.print(F("Sensor is Working"));
  }
  else
  {
    lcd.print(F("FPSensor NotWork"));
    while (1);
  }
  gsm.listen();
  gsm.println(F("AT"));
  delay(1000);
  gsm.println(F("AT+CMGF=1"));
  delay(3000);
  gsm.println(F("AT+CNMI=2,2,0,0,0"));
  delay(3000);
  lcd.clear();
}

/* Loop Function */
void loop()
{
  char Key = GetKey();
  PTime = millis();
  while (Key)
  {
    mySerial.listen();
    char EnrollInd = GetKey();
    if (EnrollInd == '*')
    {
      lcd.clear();
      if (!EnrollAuthorisation)
      {
        lcd.clear();
        lcd.print(F("Plz Enter PSWD"));      
        if (CompareString(IntToStr(GetEnterNo(0)),Read_Mob_Number(1000)))//(Password == EnterPSWD)
        {
          lcd.clear();
          lcd.print(F("PSWD Match"));
          delay(3000);
          lcd.clear();
          EnrollAuthorisation = 1;
          EnrollmentDelay = millis();
          goto EnrollStart;
        }
        else
        {
          lcd.print(F("PSWD Not Match"));
          delay(3000);
          lcd.clear();
        }
      }
      else
      {
EnrollStart:
        int EnrollID = 0;
        lcd.setCursor(0, 0);
        lcd.print(F("Enter user ID"));
        EnrollID = GetEnterNo(0);
        lcd.clear();
        lcd.print(F("Enter Mob No"));
        StoreToEeprom((EnrollID-1),IntToStr(GetEnterNo(0)));
        Serial.println(Read_Mob_Number(EnrollID-1));
        lcd.clear();
        lcd.print(F("No Stored"));
        delay(3000);
        lcd.clear();
        lcd.print(F("Place ur Finger"));      
        while (! getFingerprintEnroll(EnrollID) );
        lcd.clear();
      }
    }
    else
    {
      int id = getFingerprintIDez();
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(F("Place ur Finger"));
      if (id != -1)
      {
        if (id == 0)
        {
          lcd.setCursor(0, 0);
          lcd.print(F("Not Enroll"));
          delay(3000);
        }
        else
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(F("ID "));
          lcd.print(id);
          lcd.setCursor(0, 1);
          lcd.print(F("Sending SMS.."));
          gsm.listen();
          msg_send("Person on Bus", Read_Mob_Number(id-1));
          delay(2000);         
        }
        lcd.clear();
        gsm.listen();
      }
    }
    if (millis() > (PTime + 60000))
    {
      Key = 0;
      lcd.clear();
    }
  }
  if (EnrollAuthorisation)
  {
    if (millis() > EnrollmentDelay + 3600000)
      EnrollAuthorisation = 0;
  }
}

/* User Defined Function */
int getFingerprintIDez()
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    lcd.clear();
    lcd.print(F("Not matched")) ;
    return 0;
  }
  //Serial.println(finger.confidence);
  return finger.fingerID;
}

/* User Defined Function */
uint64_t GetEnterNo(unsigned char No)
{
  char KeyData, x;
  uint64_t Ent_Amt = 0;
  x = No;
  while (KeyData != '#')
  {
    KeyData = GetKey();
    if (KeyData)
    {
      if (KeyData == '#')
        return (Ent_Amt);
      else if (KeyData == '*' && No > x)
      {
        No--;
        lcd.setCursor(No, 1);
        lcd.print(F("  "));
        Ent_Amt /= 10;
      }
      else
      {
        Ent_Amt *= 10;
        Ent_Amt += (KeyData - '0');
        lcd.setCursor(No, 1);
        lcd.print(KeyData);
        lcd.print(F(" "));
        No++;
      }
      delay(30);
    }
  }
}

/* User Defined Function */
void KeypadInit()
{
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(C1, INPUT_PULLUP);
  pinMode(C2, INPUT_PULLUP);
  pinMode(C3, INPUT_PULLUP);

}

/* User Defined Function */
char GetKey()
{
  digitalWrite(R1, HIGH);
  digitalWrite(R2, HIGH);
  digitalWrite(R3, HIGH);
  digitalWrite(R4, HIGH);
  digitalWrite(R1, LOW);
  if (!digitalRead(C1))
  {
    while (!digitalRead(C1));
    delay(100);
    return keys[0][0];
  }
  else if (!digitalRead(C2))
  {
    while (!digitalRead(C2));
    delay(100);
    return keys[0][1];
  }
  else if (!digitalRead(C3))
  {
    while (!digitalRead(C3));
    delay(100);
    return keys[0][2];
  }
  digitalWrite(R2, LOW);
  digitalWrite(R1, HIGH);
  if (!digitalRead(C1))
  {
    while (!digitalRead(C1));
    delay(100);
    return keys[1][0];
  }
  else if (!digitalRead(C2))
  {
    while (!digitalRead(C2));
    delay(100);
    return keys[1][1];
  }
  else if (!digitalRead(C3))
  {
    while (!digitalRead(C3));
    delay(100);
    return keys[1][2];
  }
  digitalWrite(R3, LOW);
  digitalWrite(R2, HIGH);
  if (!digitalRead(C1))
  {
    while (!digitalRead(C1));
    delay(100);
    return keys[2][0];
  }
  else if (!digitalRead(C2))
  {
    while (!digitalRead(C2));
    delay(100);
    return keys[2][1];
  }
  else if (!digitalRead(C3))
  {
    while (!digitalRead(C3));
    delay(100);
    return keys[2][2];
  }
  digitalWrite(R4, LOW);
  digitalWrite(R3, HIGH);
  if (!digitalRead(C1))
  {
    while (!digitalRead(C1));
    delay(100);
    return keys[3][0];
  }
  else if (!digitalRead(C2))
  {
    while (!digitalRead(C2));
    delay(100);
    return keys[3][1];
  }
  else if (!digitalRead(C3))
  {
    while (!digitalRead(C3));
    delay(100);
    return keys[3][2];
  }
  digitalWrite(R4, HIGH);
  return ('\0');
}

/* User Defined Function */
void msg_send(char *msgTosend, char *Num)
{
  gsm.listen();
  gsm.println(F("AT+CMGF=1"));
  delay(1000);
  gsm.print(F("AT+CMGS=\""));
  gsm.print(Num);
  gsm.println("\"");
  delay(2000);
  gsm.println(msgTosend);
//  Serial.println(msgTosend);
  delay(100);
  gsm.write(0x1a);
  delay(4000);
}

/* User Defined Function */
void StoreToEeprom(int Location, /*String Number*/char *Number)
{
  unsigned char i;
  if (Location == 1000)
  {
    for (i = 0; Number[i] != '\0'; i++)
    {
      EEPROM.write(Location + i, Number[i]);
    }
    EEPROM.write(Location + i, ',');
//    Serial.println(F("Store PSWD"));
  }
  else
  {
//    Serial.println(Location);
    for (i = 0; Number[i] != '\0'; i++)
    { 
      delay(10);     
      EEPROM.write((Location * 10 + i), Number[i]);
//      Serial.print(Number[i]);
    }
//    Serial.println(F("Store Number"));
//    Serial.println(Number);
  }
}

/* User Defined Function */
char *Read_Mob_Number(int Nu)
{
  int i = 0;
  static char p[11];
  if(Nu == 1000)
  {
    while (1)
    {
      char x = EEPROM.read(i + Nu);
      if (x == ',')
      {
        p[i] = '\0';
        return p;
      }
      else
      {
        p[i] = x;
      }
      i++;
    }
  }
  else
  {
    Nu = 10*Nu;
    while (i < 10)
    {
      char x = EEPROM.read(Nu+i);
      p[i] = x;
      i++;
    }
    p[i] = '\0';
    return p;
  }
}

/* User Defined Function */
uint8_t getFingerprintEnroll(uint8_t id)
{
  uint8_t p = -1;
  //  lcd.clear();
  //  lcd.print(F("Place Ur Finger"));
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
//        Serial.println(F("Image taken"));
        break;
      case FINGERPRINT_NOFINGER:
//        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
//        Serial.println(F("Communication error"));
        break;
      case FINGERPRINT_IMAGEFAIL:
//        Serial.println(F("Imaging error"));
        break;
      default:
//        Serial.println(F("Unknown error"));
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
//      Serial.println(F("Image converted"));
      break;
    case FINGERPRINT_IMAGEMESS:
//      Serial.println(F("Image too messy"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
//      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_FEATUREFAIL:
//      Serial.println(F("Could not find fingerprint features"));
      return p;
    case FINGERPRINT_INVALIDIMAGE:
//      Serial.println(F("Could not find fingerprint features"));
      return p;
    default:
//      Serial.println(F("Unknown error"));
      return p;
  }

  lcd.clear();
  lcd.print(F("Remove finger"));
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  p = -1;
  lcd.clear();
  lcd.print(F("Place same finger again"));
  delay(1000);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
//        Serial.println(F("Image taken"));
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(F("."));
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
//        Serial.println(F("Communication error"));
        break;
      case FINGERPRINT_IMAGEFAIL:
//        Serial.println(F("Imaging error"));
        break;
      default:
//        Serial.println(F("Unknown error"));
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
//      Serial.println(F("Image converted"));
      break;
    case FINGERPRINT_IMAGEMESS:
//      Serial.println(F("Image too messy"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
//      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_FEATUREFAIL:
//      Serial.println(F("Could not find fingerprint features"));
      return p;
    case FINGERPRINT_INVALIDIMAGE:
//      Serial.println(F("Could not find fingerprint features"));
      return p;
    default:
//      Serial.println(F("Unknown error"));
      return p;
  }

  // OK converted!
  p = finger.createModel();
  if (p == FINGERPRINT_OK)
  {
    Serial.println(F("Prints matched!"));
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(F("Communication error"));
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println(F("Fingerprints did not match"));
    return p;
  } else {
    Serial.println(F("Unknown error"));
    return p;
  }
  Serial.print(id);
  p = finger.storeModel(id);
  lcd.clear();
  if (p == FINGERPRINT_OK)
  {
    lcd.print(F("Stored!"));
    return id;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.print(F("Communication error"));
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    lcd.print(F("Could not store in that location"));
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    lcd.print(F("Error writing to flash"));
    return p;
  } else {
    lcd.print(F("Unknown error"));
    return p;
  }
}

/* User Defined Function */
char *IntToStr(uint64_t Nu)
{
  unsigned char temp[11], Rem, i = 0, j = 0;
  static char p[11];
  do
  {
    Rem = Nu % 10;
    Nu = Nu / 10;
    temp[i] = Rem + '0';
    i++;
  }
  while (Nu);
  while (i > 0)
  {
    i--;
    p[j] = temp[i];
    j++;
  }
  p[j] = '\0';
  return p;
}

/* User Defined Function */
bool CompareString(char *ST1, char *ST2)
{
  unsigned char ByteNo;
  for(ByteNo = 0; ST1[ByteNo]!='\0'; ByteNo++)
  {
    if(ST1[ByteNo]!= ST2[ByteNo])
      return 0;
  }
  if(ST2[ByteNo]=='\0')
    return 1;
}

