



int brilhoLed = 0;
String cmd = "";
bool debugControle = true;




// Header file includes
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "Font_Data.h"
#define MAX_DEVICES 4
#define CLK_PIN   43 // 52 13 laranja
#define DATA_PIN  42 // 51 11 verde
#define CS_PIN    41 // 53 10 amarelo
//--RTC
#include <MD_DS1307.h>
#include <Wire.h>
//--RTC

// Hardware SPI connection
//MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);
// Arbitrary output pins
 MD_Parola P = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#define SPEED_TIME  75
#define PAUSE_TIME  0

#define MAX_MESG  20

// Turn on debug statements to the serial output
#define DEBUG 0

// Global variables
char szTime[9];    // mm:ss\0



void getTime(char *psz, bool f = true)
// Code for reading clock time
{

  RTC.readTime();
  sprintf(psz, "%02d%c%02d", RTC.h, (f ? ':' : ' '), RTC.m);

}


void setup(void)
{
  // ---------------- PAROLA ---------------------
  P.begin(2);
  P.setInvert(false);
  P.setZone(1, MAX_DEVICES-4, MAX_DEVICES-1);
  P.setFont(1, numeric7Seg);
  P.displayZoneText(1, szTime, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
  // ---------------- RTC ---------------------
  RTC.control(DS1307_CLOCK_HALT, DS1307_OFF);
  RTC.control(DS1307_12H, DS1307_OFF);
  getTime(szTime);
  // -------------------------------------------------
  
}

void loop(void)
{

  /////// RECEBE SERIAL //-----------------------------------------------------------------------------------------------------------
  if(Serial.available() > 0) {
    while(Serial.available() > 0){
      cmd += char(Serial.read());
      delay(10);
    }
    Serial.println(cmd); 
  }
  if (cmd.length() >0) { 
    // switch debug
    
    if(cmd == "debug-on"){debugControle = true;}
    if(cmd == "debug-off"){debugControle = false;}


    if(cmd == "brilho+"){controleBrilho('+');}
    if(cmd == "brilho-"){controleBrilho('-');}

    
    cmd = "";
  }
  //-----------------------------------------------------------------------------------------------------------------------------


  static uint32_t lastTime = 0; // millis() memory
  static uint8_t  display = 0;  // current display mode
  static bool flasher = false;  // seconds passing flasher
  P.setIntensity(brilhoLed);
  P.displayAnimate();
  // Finally, adjust the time string if we have to
  if (millis() - lastTime >= 1000){
    lastTime = millis();
    getTime(szTime, flasher);
    flasher = !flasher;
    P.displayReset(1);
  }
}


void controleBrilho(char tipo){
  if(tipo == '+'){
      if(brilhoLed < 15){
        brilhoLed = brilhoLed + 1;  
      }
      Serial.println(brilhoLed);
   }else if(tipo == '-'){
      if(brilhoLed >= 1){
        brilhoLed = brilhoLed - 1;
      }
      Serial.println(brilhoLed);
   }else{
     Serial.print("Caiu no else: "); 
     Serial.println(tipo);
   }

}
