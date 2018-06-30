
#include <Bounce2.h>
#define BUTTON_PIN_1 3
#define BUTTON_PIN_2 4
Bounce debouncer1 = Bounce(); // Instantiate a Bounce object
Bounce debouncer2 = Bounce(); // Instantiate another Bounce object



#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "Font_Data.h"

#define MAX_ZONES 2
#define ZONE_SIZE 4
#define MAX_DEVICES (MAX_ZONES * ZONE_SIZE)
#define SCROLL_SPEED  30

#define ZONE_UPPER  1
#define ZONE_LOWER  0

#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

// HARDWARE SPI
MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);
// SOFTWARE SPI
//MD_Parola P = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
char *msg[] =
{
  "<<<<<<<<",
  ">>>>>>>>",
  "45 k",
  "-",
  "1,5m ",
  "mantenha distancia",
  "Respeite 1 carro a MENOS",

};




#define LED_PIN 5

#define DEBUG 1
















void setup() {

#if DEBUG
  Serial.begin(9600);
  Serial.println("Debug BIKE-SMART-BOX v1");
#endif

  pinMode(BUTTON_PIN_1,INPUT_PULLUP); // Setup the first button with an internal pull-up :
  debouncer1.attach(BUTTON_PIN_1);// After setting up the button, setup the Bounce instance :
  debouncer1.interval(5); // interval in ms
  pinMode(BUTTON_PIN_2,INPUT_PULLUP);// Setup the second button with an internal pull-up :
  debouncer2.attach(BUTTON_PIN_2);// After setting up the button, setup the Bounce instance :
  debouncer2.interval(5); // interval in ms





  P.begin(MAX_ZONES);// initialise the LED display

  // Set up zones for 2 halves of the display
  // Each zone gets a different font, making up the top
  // and bottom half of each letter
  P.setZone(ZONE_LOWER, 0, ZONE_SIZE - 1);
  P.setFont(ZONE_LOWER, BigFontLower);

  P.setZone(ZONE_UPPER, ZONE_SIZE, MAX_DEVICES-1);
  P.setFont(ZONE_UPPER, BigFontUpper);
  P.setCharSpacing(P.getCharSpacing() * 2); // double height --> double spacing


  


  //Setup the LED :
  pinMode(LED_PIN,OUTPUT);

}

void loop() {

  static uint32_t  lastTime = 0; // millis() memory
  static bool flasher = false;  // seconds passing flasher
  
  debouncer1.update();// Update the Bounce instances :
  debouncer2.update();
  int btEsq = debouncer1.read(); // Get the updated value :
  int btDir = debouncer2.read();

  





















  // Turn on the LED if either button is pressed :
  if ( btEsq == LOW) {
    digitalWrite(LED_PIN, HIGH );
    Serial.println("bt-esq");
    
    P.displayAnimate();
    if (P.getZoneStatus(ZONE_LOWER) && P.getZoneStatus(ZONE_UPPER)){
        P.setFont(ZONE_LOWER, BigFontLower);
        P.setFont(ZONE_UPPER, BigFontUpper);
        P.displayZoneText(ZONE_LOWER, msg[0], PA_RIGHT, SCROLL_SPEED, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
        P.displayZoneText(ZONE_UPPER, msg[0], PA_LEFT, SCROLL_SPEED, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
        // synchronise the start
        P.displayClear();
        P.synchZoneStart();
    }

  }else if (btDir == LOW ) {
    digitalWrite(LED_PIN, HIGH );
    Serial.println("bt-dir");
    
    P.displayAnimate();
    if (P.getZoneStatus(ZONE_LOWER) && P.getZoneStatus(ZONE_UPPER)){
      
        P.setFont(ZONE_LOWER, BigFontLower);
        P.setFont(ZONE_UPPER, BigFontUpper);
        P.displayZoneText(ZONE_LOWER, msg[1], PA_RIGHT, SCROLL_SPEED, 0, PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
        P.displayZoneText(ZONE_UPPER, msg[1], PA_LEFT, SCROLL_SPEED, 0, PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
        // synchronise the start
        P.displayClear();
        P.synchZoneStart();
    }

  }else {
    P.displayClear();
    P.displayZoneText(ZONE_LOWER, "", PA_RIGHT, SCROLL_SPEED, 0, PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
    P.displayZoneText(ZONE_UPPER, "", PA_LEFT, SCROLL_SPEED, 0, PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);

    digitalWrite(LED_PIN, LOW );
  }

}


