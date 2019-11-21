
// Biblioteca do módulo I2C
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o display no endereço 0x27
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


#include <Bounce2.h>

#define BUTTON_PIN_1 14
#define BUTTON_PIN_2 15
#define BUTTON_PIN_3 16
#define BUTTON_PIN_4 17


// Instantiate a Bounce object
Bounce debouncer1 = Bounce(); 
Bounce debouncer2 = Bounce();
Bounce debouncer3 = Bounce(); 
Bounce debouncer4 = Bounce();

void setup() {

  // Inicializa o display LCD 16x2
 lcd.begin (16, 2);
 // Liga a luz de fundo do LCD
 lcd.setBacklight(HIGH);

  // Setup the first button with an internal pull-up :
  pinMode(BUTTON_PIN_1,INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer1.attach(BUTTON_PIN_1);
  debouncer1.interval(5); // interval in ms
  
   // Setup the second button with an internal pull-up :
  pinMode(BUTTON_PIN_2,INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer2.attach(BUTTON_PIN_2);
  debouncer2.interval(5); // interval in ms


  pinMode(BUTTON_PIN_3,INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer3.attach(BUTTON_PIN_3);
  debouncer3.interval(5); // interval in ms

  pinMode(BUTTON_PIN_4,INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer4.attach(BUTTON_PIN_4);
  debouncer4.interval(5); // interval in ms


  //Setup the LED :
  pinMode(LED_BUILTIN,OUTPUT);

}

void loop() {


 //lcd.clear();
 lcd.setCursor(3, 0);
 lcd.print("Aguardando");
 lcd.setCursor(4, 1);
 lcd.print("Digital");
 //delay (50);


 
  // Update the Bounce instances :
  debouncer1.update();
  debouncer2.update();
  debouncer3.update();
  debouncer4.update();
  
  // Get the updated value :
  int value1 = debouncer1.read();
  int value2 = debouncer2.read();
  int value3 = debouncer3.read();
  int value4 = debouncer4.read();

  // Turn on the LED if either button is pressed :
  if ( value1 == LOW || value2 == LOW || value3 == LOW || value4 == LOW ) {
    digitalWrite(LED_BUILTIN, HIGH );
     lcd.setBacklight(LOW);
  } 
  else {
    digitalWrite(LED_BUILTIN, LOW );
     lcd.setBacklight(HIGH);
  }

}


