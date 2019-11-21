
#include <Bounce2.h>

#define BUTTON_PIN_FINGERPRINT D1 //D4
#define BUTTON_PIN_ABRE_PORTAO 17 //D5


#define LED_PIN D0

// Instantiate a Bounce object
Bounce debouncer1 = Bounce(); 

// Instantiate another Bounce object
Bounce debouncer2 = Bounce(); 

void setup() {

  // Setup the first button with an internal pull-up :
  pinMode(BUTTON_PIN_FINGERPRINT,INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer1.attach(BUTTON_PIN_FINGERPRINT);
  debouncer1.interval(5); // interval in ms
  
   // Setup the second button with an internal pull-up :
  pinMode(BUTTON_PIN_ABRE_PORTAO,INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer2.attach(BUTTON_PIN_ABRE_PORTAO);
  debouncer2.interval(5); // interval in ms


  //Setup the LED :
  pinMode(LED_PIN,OUTPUT);

}

void loop() {
  // Update the Bounce instances :
  debouncer1.update();
  debouncer2.update();

  // Get the updated value :
  int value1 = debouncer1.read();
  int value2 = debouncer2.read();

  // Turn on the LED if either button is pressed :
  if ( value1 == LOW || value2 == LOW ) {
    digitalWrite(LED_PIN, HIGH );
  } 
  else {
    digitalWrite(LED_PIN, LOW );
  }

}


