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
  Serial.begin(9600);
  pinMode(bt_input, INPUT_PULLUP); //internal pullup resistor is used to simplify the circuit
  pinMode(bt_esq, INPUT_PULLUP);
  pinMode(bt_dir, INPUT_PULLUP);
  //pinMode(bluePin, OUTPUT);
  ///pinMode(greenPin, OUTPUT);
  pinMode(led_indicativo, OUTPUT);
  Timer1.initialize(5000000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( piscaLed ); // attach the service routine here
  //pinMode(ledPin,OUTPUT);
  
}
 
void loop() {
   
  






 
  if ( digitalRead(bt_esq) == LOW) {
      digitalWrite(led_esq, HIGH );
      Timer1.setPeriod(400000);
      retorno = 1;
      //Serial.println("bt-esq");
  }else if (digitalRead(bt_dir) == LOW ) {
      digitalWrite(led_dir, HIGH );
      Timer1.setPeriod(400000);
      retorno = 1;
      //Serial.println("bt-dir");
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
      }
      
      //if (last_command == 100){
          //digitalWrite(led_indicativo, !digitalRead(led_indicativo));
          //Serial.println("Comando - 1");
          //Timer1.setPeriod(800000);
      //}

        if (command == 1){
          //digitalWrite(greenPin, !digitalRead(greenPin));
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

void piscaLed()
{
    // Toggle LED
    digitalWrite( led_indicativo, digitalRead( led_indicativo ) ^ 1 );
}
