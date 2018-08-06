String jarbasVersion = "Jarbas-INREBRA-2018-V42B-RTC";





/* ------------------------------------------------------------------ ETHERNET ------------------------------------------------------------------ */
//ethernet
#include <Ethernet.h>
#include <SPI.h>
EthernetServer server(8087); // Cria o servidor na porta 8080

byte myMac[]={0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};  //Mac PRODUÇÃO
byte myIp[]  = { 192, 168, 0, 7 };   //define IP da central automação 1

byte gateway[] = { 192,168,0, 1 }; // ip do roteador
byte subnet[] = { 255, 255, 255, 0 };
/* ------------------------------------------------------------------ ETHERNET ------------------------------------------------------------------ */


#include <MD_Parola.h>
#include <MD_MAX72xx.h>
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

void getTime(char *psz, bool f = true){// Code for reading clock time
  RTC.readTime();
  sprintf(psz, "%02d%c%02d", RTC.h, (f ? ':' : ' '), RTC.m);
}

/* ------------------------------------------------------------------ IR ------------------------------------------------------------------ */
/// IR
#include <IRremote.h>  
IRsend irsend; // IR SALA
int NEGATIVOIRSALA = 22;
int NEGATIVOIRESCADA = 24;
int NEGATIVOIRCRIANCAS = 26;
int NEGATIVOIRMEIO = 28;
int NEGATIVOIRCASAL = 30;

/////// receptor IR sala
int RECV_PIN = 45; 
IRrecv irrecv(RECV_PIN);

decode_results results; 
float armazenavalor; 

/* ------------------------------------------------------------------ IR ------------------------------------------------------------------ */


/* ------------------------------------------------------------------ Servo + Joystick + LAZER------------------------------------------------------------------ */
#include <VarSpeedServo.h>
VarSpeedServo servo_base;
VarSpeedServo servo_inclinacao;
int pino_x = A4; //Pino ligado ao X do joystick
int pino_y = A5; //Pino ligado ao Y do joystick
int val_x;   //Armazena o valor do eixo X
int val_y;   //Armazena o valor do eixo Y

#include <Bounce2.h>
#define BUTTON_PIN_1 4
Bounce debouncer1 = Bounce(); // Instantiate a Bounce object
    
const int lazerPin =  3;
int flag=0;
/* ------------------------------------------------------------------ Servo + Joystick + LAZER------------------------------------------------------------------ */


int PIN_PORTAO_PEDESTRE = 31;
int PIN_TOMADA_SALA = 33;

int FLAGTomadaSala = LOW ;
int FLAGPortaoPedestre = HIGH;

int brilhoLed = 0;

String cmd = "";
String comando = "";
String strTipo = "";
String strRepeticao = "";
String strLocal = "N"; 

String strTamanho = ""; 
int intTamanho; 

bool debugControle = true;
//----------------------------



//generico

void armadilhaBaixo(){
  Serial.println("armadilha mosquito baixo");
  if (!FLAGTomadaSala) {  
    FLAGTomadaSala = HIGH;  
    digitalWrite(PIN_TOMADA_SALA, FLAGTomadaSala);  
  }else{
    FLAGTomadaSala = LOW;  
    digitalWrite(PIN_TOMADA_SALA, FLAGTomadaSala);  
  } 
}

void armadilhaCima(){
  Serial.println("armadilha mosquito cima");
}

void resetModem(){
  Serial.println("reset Modem");
}

void portaoPedestre(){
  Serial.println("Abre de sesamo");
  if (!FLAGPortaoPedestre) {  
    FLAGPortaoPedestre = HIGH;  
    digitalWrite(PIN_PORTAO_PEDESTRE, FLAGPortaoPedestre);  
  }else{
    FLAGPortaoPedestre = LOW;  
    digitalWrite(PIN_PORTAO_PEDESTRE, FLAGPortaoPedestre);  
  } 
}


void controlaRele(String strValue){ 
  if(strValue == "armadilhaBaixo"){armadilhaBaixo();}
  if(strValue == "armadilhaCima"){armadilhaCima();}
  if(strValue == "resetModem"){resetModem();}
  if(strValue == "portaoPedestre"){portaoPedestre();}
}

void sendIr(String strRepeticao, String strLocal, String strValue)
{   

/*
S - sala
E -escada
K -crianças
M -escritorio
C -casal
E -externas
*/

  if(strLocal != "S"){digitalWrite(NEGATIVOIRSALA, HIGH); }
  if(strLocal != "E"){digitalWrite(NEGATIVOIRESCADA, HIGH); }
  if(strLocal != "K"){digitalWrite(NEGATIVOIRCRIANCAS, HIGH); }
  if(strLocal != "M"){digitalWrite(NEGATIVOIRMEIO, HIGH); }
  //if(strLocal != "E"){digitalWrite(NEGATIVOIRMEIO, HIGH); }
  if(strLocal != "C"){digitalWrite(NEGATIVOIRCASAL, HIGH); }

  
  int intRepeticao = strRepeticao.toInt();
  if(intRepeticao < 11){
    for (int i = 0; i < intRepeticao; i++) {
      unsigned long code = strValue.toInt();
      irsend.sendNEC(code, 32);
      delay(40);
    }
    if(debugControle == true){
      Serial.print("codigo:");
      Serial.print(strValue);
      Serial.print(" - repeticao:");
      Serial.print(strRepeticao);
      Serial.print(" - local:");
      Serial.println(strLocal);
    }
  }else{
    Serial.println("muitas repetições melhor parar");
  }

  if(strLocal != "S"){digitalWrite(NEGATIVOIRSALA, LOW); }
  if(strLocal != "E"){digitalWrite(NEGATIVOIRESCADA, LOW); }
  if(strLocal != "K"){digitalWrite(NEGATIVOIRCRIANCAS, LOW); }
  //if(strLocal != "M"){digitalWrite(NEGATIVOIRMEIO, LOW); }
  if(strLocal != "E"){digitalWrite(NEGATIVOIRMEIO, LOW); }
  if(strLocal != "C"){digitalWrite(NEGATIVOIRCASAL, LOW); } 
  
  irrecv.enableIRIn(); // Re-enable receiver
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

//----------------------------
#include "pitches.h" //musica




void setup(){ 
  Serial.begin(9600);
  Ethernet.begin(myMac, myIp, gateway, subnet);//inicializa eternet
  
  pinMode(speakerPin, OUTPUT); // speaker

  pinMode(NEGATIVOIRSALA, OUTPUT);
  pinMode(NEGATIVOIRESCADA, OUTPUT);
  pinMode(NEGATIVOIRCRIANCAS, OUTPUT);
  pinMode(NEGATIVOIRMEIO, OUTPUT);
  pinMode(NEGATIVOIRCASAL, OUTPUT);

  digitalWrite(NEGATIVOIRSALA, LOW);
  digitalWrite(NEGATIVOIRESCADA, LOW);
  digitalWrite(NEGATIVOIRCRIANCAS, LOW);
  digitalWrite(NEGATIVOIRMEIO, LOW);
  digitalWrite(NEGATIVOIRCASAL, LOW);
 
  pinMode(PIN_PORTAO_PEDESTRE, OUTPUT);
  pinMode(PIN_TOMADA_SALA, OUTPUT);
  
  digitalWrite(PIN_PORTAO_PEDESTRE, FLAGPortaoPedestre);
  digitalWrite(PIN_TOMADA_SALA, FLAGTomadaSala);

  irrecv.enableIRIn(); // Inicializa o receptor IR 
  Serial.println(jarbasVersion);

  servo_base.attach(7, 1, 180);
  servo_inclinacao.attach(8, 1, 180);
  pinMode(BUTTON_PIN_1,INPUT_PULLUP); // Setup the first button with an internal pull-up :
  debouncer1.attach(BUTTON_PIN_1);// After setting up the button, setup the Bounce instance :
  debouncer1.interval(5); // interval in ms
  pinMode(lazerPin, OUTPUT);     

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
   
  
  musica();

}

void loop(){
  
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

    if(cmd == "salaon"){
      digitalWrite(PIN_TOMADA_SALA, HIGH);
      Serial.println("aqui");
      }
    if(cmd == "salaoff"){
      digitalWrite(PIN_TOMADA_SALA, LOW);
      Serial.println("aqui 2");
      }
    cmd = "";
  }
  //-----------------------------------------------------------------------------------------------------------------------------


  //parola + rtc -------------------------------------------
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

  //--------------------------------------------------------


  
  /////// RECEBE IR //-----------------------------------------------------------------------------------------------------------
  if (irrecv.decode(&results)){  
    Serial.print("Valor lido 1 : ");  
    Serial.println(results.value, HEX);  
    //armazenavalor = (results.value);   
    if(results.value !=0xffffffff){
      armazenavalor = (results.value);
    }
    
    //volume
    if (armazenavalor == 0x20DF40BF) {   
      if(results.value !=0xffffffff){
        sendIr("1","S", "1587632295");// #1volMaisReciverS
      }else{
        sendIr("10","S", "1587632295");// #1volMenosReciverS
      }
    }
      
    if (armazenavalor == 0x20DFC03F) { 
      if(results.value !=0xffffffff){
        sendIr("1","S", "1587664935");// #1volMenosReciverS
      }else{
        sendIr("10","S", "1587664935");// #1volMenosReciverS
      }
    }
    irrecv.resume(); //Le o próximo valor  
  } 



  //-----------------------------------------------------------------------------------------------------------------------------



  /////// RECEBE ETHERNET //-----------------------------------------------------------------------------------------------------------
  EthernetClient client = server.available();
  if (!client) {
    return;
  }
  // Wait until the client sends some data
  Serial.println("Novo client");
  while(!client.available()){
    delay(1);
  }
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  
  if(debugControle == true){
    Serial.print("Request: ");
    Serial.println(request);
  }
  
  if (request.indexOf("/ir?") != -1)  {
    strRepeticao = request.substring(9,10);
    intTamanho = request.length();
    strLocal = request.substring(intTamanho -9, intTamanho -10);
    comando = request.substring(10,intTamanho -10);
    sendIr(strRepeticao,strLocal, comando);
  }

  if (request.indexOf("/rele?") != -1)  {
    intTamanho = request.length();
    comando = request.substring(11,intTamanho -9 );
    Serial.println(comando);
    controlaRele(comando);

    
    //intTamanho = request.length();
    //strLocal = request.substring(intTamanho -9, intTamanho -10);
    //comando = request.substring(10,intTamanho -10);
    //sendIr(strRepeticao,strLocal, comando);
  }
    
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Access-Control-Allow-Origin: *");
  client.println(""); //  do not forget this one
  //continua....
  //client.println("arduino - conectado!"); //  do not forget this one
  //client.println(jarbasVersion); //  do not forget this one
  client.print(montaString());

  delay(1);
  client.stop();
  Serial.println("Client disconectado");
  Serial.println("");
    
    
  
  //-----------------------------------------------------------------------------------------------------------------------------


  ///// ----------- SERVO + JOYSTICK

  debouncer1.update();
  int btLazer = debouncer1.read();

  if ( btLazer == LOW) {
    Serial.println("bt comando");
    if ( flag == 0){
      digitalWrite(lazerPin, HIGH);
      flag=1;

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

    }else if ( flag == 1){
      digitalWrite(lazerPin, LOW);
      flag=0;  
    }   
  }

}// FIM DO LOOP  



String montaString()
{
    String string1 ="";
    string1.concat("dados({");
   //tomadaSala
    string1.concat("Status : '"); // valor / estado
    string1.concat("OnLine");
    //tomadaArthur
    string1.concat("', Versao : '"); // valor / estado
    string1.concat(jarbasVersion);
    //tomadaCasal
    //string1.concat("', IP : '"); // valor / estado
    //string1.concat(myIp[]);
    string1.concat("'})");
    return string1;

}


