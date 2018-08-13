//Programa : Controle servo motor com joystick
//Autor : FILIPEFLOP
 
#include <VarSpeedServo.h>
 
//Cria objeto para controlar o servo base
VarSpeedServo servo_base;
//Cria objeto para controlar o servo inclinacao
VarSpeedServo servo_inclinacao;
 
int pino_x = A4; //Pino ligado ao X do joystick
int pino_y = A5; //Pino ligado ao Y do joystick
int val_x;   //Armazena o valor do eixo X
int val_y;   //Armazena o valor do eixo Y

//Constants
const int buttonPin = 37;     
const int ledPin =  3;     

//Variables
int buttonState = 0;
int flag=0;
 
void setup()
{
  //Define a porta a ser ligada ao servo base
  servo_base.attach(39, 1, 180);
  //Define a porta a ser ligada ao servo inclinacao
  servo_inclinacao.attach(41, 1, 180);

  //Input or output?
  pinMode(ledPin, OUTPUT);      
  pinMode(buttonPin, INPUT_PULLUP);  
}
 
void loop()
{

  //Read button state (pressed or not pressed?)
  buttonState = digitalRead(buttonPin);

  //If button pressed...
  if (buttonState == LOW) { 
    //...ones, turn led on!
    if ( flag == 0){
      digitalWrite(ledPin, HIGH);
      flag=1; //change flag variable
    }
    //...twice, turn led off!
    else if ( flag == 1){
      digitalWrite(ledPin, LOW);
      flag=0; //change flag variable again 
    }    
  } 

    
  //Recebe o valor do joystick, eixo X
  val_x = analogRead(pino_x);
  //Converte o valor lido para um valor entre 1 e 180 graus
  val_x = map(val_x, 0, 1023, 1, 180);
  //Move o servo base para a posicao definida pelo joystick
  servo_base.slowmove(val_x, 60);
  //Recebe o valor do joystick, eixo Y
  val_y = analogRead(pino_y);
  //Converte o valor lido para um valor entre 1 e 180 graus
  val_y = map(val_y, 0, 1023, 1, 180);
  //Move o servo inclinacao para a posicao definida pelo joystick
  servo_inclinacao.slowmove(val_y, 60);
  //Aguarda a movimentacao do servo e reinicia a leitura
  delay(30);


 



  
}
