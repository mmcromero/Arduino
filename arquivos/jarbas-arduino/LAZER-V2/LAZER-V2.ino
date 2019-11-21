
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
#define BUTTON_PIN_1 7
Bounce debouncer1 = Bounce(); // Instantiate a Bounce object
    
const int lazerPin =  3;
int flag=0;

int PIN_RELE = 6;
 
void setup()
{
  Serial.begin(9600);
  servo_base.attach(servo1, 1, 180);
  servo_inclinacao.attach(servo2, 1, 180);
  pinMode(BUTTON_PIN_1,INPUT_PULLUP); // Setup the first button with an internal pull-up :
  debouncer1.attach(BUTTON_PIN_1);// After setting up the button, setup the Bounce instance :
  debouncer1.interval(50); // interval in ms
  pinMode(lazerPin, OUTPUT);

  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, HIGH);
}
 
void loop()
{

   debouncer1.update();
  int btLazer = debouncer1.read();
  if ( btLazer == LOW) {
    Serial.println("bt comando LOW");
    if ( flag == 0){
      digitalWrite(PIN_RELE, LOW);
      digitalWrite(lazerPin, HIGH);
      flag=1;
      delay(5);
    }else if ( flag == 1){
      digitalWrite(PIN_RELE, HIGH);
      digitalWrite(lazerPin, LOW);
      flag=0; 
      delay(5); 
    }   
  }

  if(flag == 1){
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

  


 



  
}
