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

#define CLK_PIN   52 // 52 13 laranja
#define DATA_PIN  51 // 51 11 verde
#define CS_PIN    53 // 53 10 amarelo

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

#define DEBUG 1



#include <TimerOne.h>

byte bt_esq = 32;
byte bt_dir = 34;
byte bt_input = 36;

byte led_indicativo = 33;
byte led_esq = 35;
byte led_dir = 37;

//int last_command;
  int retorno = 0;
byte digitalReadOnce(byte val){
  static byte lastVal = HIGH;
  static unsigned long m = 0;
  if (lastVal != val && millis() > (m+100) ) { //M+100 -->DEBOUNCING 100ms
    lastVal = val;
    m = millis();
    return lastVal;
  } 
  return HIGH;
}
 
int getCommand(){
  static unsigned long m1 = 0;  //millis no momento inicia de pressionar o botão 
  static unsigned long m2 = 0;  //millis após soltar o botão 
  static byte count = 0;
   
  byte r = digitalRead(bt_input);
   
  if (digitalReadOnce(r) == LOW){
    m1 = millis();
    count++;
  }
   
  if (r == LOW){
    m2 = millis();
  } else {
    if (! (m2>0 && m2-m1 < 1000) ){  //o botão deve ser pressionado por menos de 1 segundo, senão cancela o comando
      count = 0;
      m1 = 0;
      m2 = 0;
    }
     
    if (m2>0 && millis()-m2 > 1500){  //após a ultima vez pressionado o botao, aguarda 1,5 segundos para finalizar e retornar o comando.
      byte c = count;
      count = 0;
      m1 = 0;
      m2 = 0;
       
      return c;
    }
  }
   
  return 0;
}
 
 
void setup() {

#if DEBUG
  Serial.begin(9600);
  Serial.println("Debug BIKE-SMART-BOX v2.2");
#endif

  pinMode(bt_input, INPUT_PULLUP); //internal pullup resistor is used to simplify the circuit
  pinMode(bt_esq, INPUT_PULLUP);
  pinMode(bt_dir, INPUT_PULLUP);
  pinMode(led_indicativo, OUTPUT);
  Timer1.initialize(5000000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( piscaLed ); // attach the service routine here

  P.begin(MAX_ZONES);// initialise the LED display
  // Set up zones for 2 halves of the display
  // Each zone gets a different font, making up the top
  // and bottom half of each letter
  P.setZone(ZONE_LOWER, 0, ZONE_SIZE - 1);
  P.setFont(ZONE_LOWER, BigFontLower);

  P.setZone(ZONE_UPPER, ZONE_SIZE, MAX_DEVICES-1);
  P.setFont(ZONE_UPPER, BigFontUpper);
  P.setCharSpacing(P.getCharSpacing() * 2); // double height --> double spacing
}


 
void loop() {

  if ( digitalRead(bt_esq) == LOW) {
      digitalWrite(led_esq, HIGH );
      Timer1.setPeriod(400000);

      if(retorno == 0){
        retorno = 1;
        Serial.println("bt-esq");
        limpaMatrixLed();
      }
      
      escreveMatrixLed(msg[0],"esquerda");
      
  }else if (digitalRead(bt_dir) == LOW ) {
      digitalWrite(led_dir, HIGH );
      Timer1.setPeriod(400000);

      if(retorno == 0){
        retorno = 1;
        Serial.println("bt-dir");
        limpaMatrixLed();
      }
      
      escreveMatrixLed(msg[1],"direita");
      
  }else {
      
      
      int command = getCommand(); 
      if(command != 0){
        Serial.print("Ultimo comando: ");
        Serial.println(command);
      }

      if(retorno == 1){
        Serial.println("retorno");
        retorno = 0;
        command = 1; 
        limpaMatrixLed();
      }else{
        escreveMatrixLed(msg[6],"esquerda");
      }
      
  

        if (command == 1){
          Serial.println("1 carro a menos, respeite!");
          Timer1.stop();
          digitalWrite( led_indicativo, LOW );
          
        }
        if (command == 2){
          Serial.println("Sua buzina não me helecopiteriza... KKK");
          Serial.println("Comando - 2");
          Timer1.setPeriod(100000);
        }
         
        if (command == 3){
          Serial.println("PASSA POR CIMA =P");
          Serial.println("Comando - 3");
          Timer1.setPeriod(200000);
        }
         
        if (command == 4){
          Serial.println("*********");
          Serial.println("Comando - 4");
          Timer1.setPeriod(300000);
        }
         
        if (command == 5){
          //digitalWrite(led_indicativo, LOW);
          Serial.println("Comando - 5");
          Timer1.setPeriod(400000);
        }

        
      
        
  }
}

void limpaMatrixLed(){
  P.displayClear();
  P.displayZoneText(ZONE_LOWER, "", PA_RIGHT, SCROLL_SPEED, 0, PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
  P.displayZoneText(ZONE_UPPER, "", PA_LEFT, SCROLL_SPEED, 0, PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
}

void escreveMatrixLed(char *msg, String direcao){
  P.displayAnimate();
  if (P.getZoneStatus(ZONE_LOWER) && P.getZoneStatus(ZONE_UPPER)){
      P.setFont(ZONE_LOWER, BigFontLower);
      P.setFont(ZONE_UPPER, BigFontUpper);

      if(direcao == "esquerda"){
      P.displayZoneText(ZONE_LOWER, msg, PA_RIGHT, SCROLL_SPEED, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      P.displayZoneText(ZONE_UPPER, msg, PA_LEFT, SCROLL_SPEED, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      }else if(direcao == "direita"){
      P.displayZoneText(ZONE_LOWER, msg, PA_RIGHT, SCROLL_SPEED, 0, PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
      P.displayZoneText(ZONE_UPPER, msg, PA_LEFT, SCROLL_SPEED, 0, PA_SCROLL_RIGHT, PA_SCROLL_RIGHT);
      }
      // synchronise the start
      P.displayClear();
      P.synchZoneStart();
  }
}
void piscaLed()
{
    // Toggle LED
    digitalWrite( led_indicativo, digitalRead( led_indicativo ) ^ 1 );
}
