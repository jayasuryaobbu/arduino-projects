/* Code devloped by Jayasurya Obbu on behalf of Immortal Electronics */
/* This code is open-source */
/* Any doubts regarding code, just mail to jay@immortalelectronics.in (or) hello@jayasurya.me */
/* Dated 16 Mar 2018 */
/* Download the Header Files from the root folder */

/* Including Header Files */
#include <VirtualWire.h>
#include <Wire.h>
#include <SPI.h>

/* Initializing Pin Numbers */
const int receive_pin = 11;

/* Setup Function */
void setup()
{
    delay(1000);
    Serial.begin(9600);  
    vw_set_rx_pin(receive_pin);
   
    vw_set_ptt_inverted(true);
    vw_setup(2000); 

    vw_rx_start();     
}

/* Loop Function */
void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    if (vw_get_message(buf, &buflen)) 
    {
      int i;
      buf[buflen] = '\0';
      Serial.print("Got: ");
      Serial.print(buflen);
      Serial.print("Got: ");
      for (i = 0; i < buflen; i++)
      {
        Serial.write(buf[i]);    
      }
      Serial.println();
      char c[4]={buf[0],buf[1],buf[2],'\0'};
      char p[6]={buf[4],buf[5], buf[6],buf[7],'\0'};
      char t[4]={buf[11],buf[12],buf[13],'\0'};
      char u[3]={buf[15],buf[16],'\0'};
      char d[5]={buf[18],buf[19],buf[20],buf[21],'\0'};
    
 
      Serial.print(c);
      Serial.println("C");
      Serial.print(p);
      Serial.println("pH");
      Serial.print(t);
      Serial.println("nm");
      Serial.print(u);
      Serial.println("cm");
      Serial.print(d);
      Serial.println("*c");

 
      delay(100); 
  }   
}
