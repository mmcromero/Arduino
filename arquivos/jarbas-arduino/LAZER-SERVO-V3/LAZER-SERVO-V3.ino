

#include <VarSpeedServo.h>
 
VarSpeedServo servo_base;
VarSpeedServo servo_inclinacao;
int servo1 = 4;
int servo2 = 5;
 
int pino_x = A4; //Pino ligado ao X do joystick
int pino_y = A5; //Pino ligado ao Y do joystick
int val_x;   //Armazena o valor do eixo X
int val_y;   //Armazena o valor do eixo Y

#include <Bounce2.h>


#define BUTTON_PIN 7
#define LED_PIN 3
int PIN_RELE = 6;

int ledState = LOW;
int pinState = LOW;

// Instantiate a Bounce object :
Bounce debouncer = Bounce(); 

void setup() {
  Serial.begin(9600);
  servo_base.attach(servo1, 1, 180);
  servo_inclinacao.attach(servo2, 1, 180);
  // Setup the button with an internal pull-up :
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  
  // After setting up the button, setup the Bounce instance :
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(500);
  
  // Setup the LED :
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,ledState);

  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, pinState);
 
  
}

void loop() {

  // Update the Bounce instance :
   debouncer.update();
   
   // Call code if Bounce fell (transition from HIGH to LOW) :
   if ( debouncer.fell() ) {
      pinState = !pinState;
      digitalWrite(PIN_RELE, pinState);
     // Toggle LED state :
     ledState = !ledState;
     digitalWrite(LED_PIN,ledState);
     
   }

  //Recebe o valor do joystick, eixo X
      val_x = analogRead(pino_x);//Converte o valor lido para um valor entre 1 e 180 graus
      val_x = map(val_x, 0, 1023, 1, 180); //Move o servo base para a posicao definida pelo joystick
      servo_base.slowmove(val_x, 60);//Recebe o valor do joystick, eixo Y
      val_y = analogRead(pino_y);//Converte o valor lido para um valor entre 1 e 180 graus
      val_y = map(val_y, 0, 1023, 1, 180);//Move o servo inclinacao para a posicao definida pelo joystick
      servo_inclinacao.slowmove(val_y, 60);//Aguarda a movimentacao do servo e reinicia a leitura
      //Aguarda a movimentacao do servo e reinicia a leitura
      //delay(30);


}

