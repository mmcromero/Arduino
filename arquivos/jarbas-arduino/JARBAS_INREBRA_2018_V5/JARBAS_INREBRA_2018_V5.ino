String jarbasVersion = "Jarbas-INREBRA-2018-V5";

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


#include <ArduinoJson.h>


/* ------------------------------------------------------------------ PAROLA  ------------------------------------------------------------------ */

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include "Font_Data.h"
#define MAX_DEVICES 4
#define CLK_PIN   52 // 52 13 laranja
#define DATA_PIN  51 // 51 11 verde
#define CS_PIN    53 // 53 10 amarelo
//--RTC
#include <MD_DS1307.h>
#include <Wire.h>
//--RTC
// Hardware SPI connection
MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);
// Arbitrary output pins
//MD_Parola P = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
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
  
/* ------------------------------------------------------------------ PAROLA  ------------------------------------------------------------------ */



/* ------------------------------------------------------------------ IR ------------------------------------------------------------------ */
/// IR
#include <IRremote.h>  
IRsend irsend; // IR SALA
int NEGATIVOIRSALA = 22;
int NEGATIVOIRESCADA = 24;
int NEGATIVOIRCRIANCAS = 26;
int NEGATIVOIRMEIO = 28;
int NEGATIVOIRCASAL = 30;
int NEGATIVOIRBRINQUEDOTECA = 32;

/////// receptor IR sala
int RECV_PIN = 45; //45
IRrecv irrecv(RECV_PIN);

decode_results results; 
float armazenavalor; 

/* ------------------------------------------------------------------ IR ------------------------------------------------------------------ */




int tempoPortaoAberto = 100;

// pinos de reles

int PIN_PORTAO_PEDESTRE = 31;
int PIN_TOMADA_SALA = 33;
int PIN_TOMADA_MODEM = 35;
int PIN_TOMADA_ESCADA = 37;

int timerPortao=0;
int brilhoLed = 0;

//flags reles
int FLAGPortaoPedestre = HIGH;
int FLAGTomadaSala = LOW ;
int FLAGTomadaModem = LOW ;
int FLAGTomadaEscada = LOW ;

int FlagTvSala = LOW;
int FlagTvBrinquedoteca = LOW;
int FlagTvMeio = LOW;


String cmd = "";
String comando = "";
String strTipo = "";
String strRepeticao = "";
String strLocal = "N"; 
String strTamanho = ""; 
int intTamanho; 

bool debugControle = false;
bool debugTensao = false;
//----------------------------



//sensor de tensão
float vetCorrente[300];
const int sensorTVsala = A0;
const int sensorTVbrinquedoteca = A1;
const int sensorTVmeio = A2;
// fim


//----------------------------
#include "pitches.h" //musica
/* ------------------------------------------------------------------ RTC ------------------------------------------------------------------ */
/// RTC
#include <Wire.h>
#include "Rtc.h" //RTC
/* ------------------------------------------------------------------ RTC ------------------------------------------------------------------ */

 




void setPin(int pin, int flag){
  digitalWrite(pin, flag);  
  if(debugControle == true){
    Serial.print("pin: ");
    Serial.print(pin);
    Serial.print(" flag: ");
    Serial.println(flag);
  }
}

void armadilhaBaixo(){
  Serial.println("armadilha mosquito baixo");
  FLAGTomadaSala = ! FLAGTomadaSala;
  setPin(PIN_TOMADA_SALA, FLAGTomadaSala);
}

void armadilhaCima(){
  Serial.println("armadilha mosquito cima");
  FLAGTomadaEscada = ! FLAGTomadaEscada;
  setPin(PIN_TOMADA_ESCADA, FLAGTomadaEscada);
}

void resetModem(){
  Serial.println("reset Modem");
}

void controlaRele(String strValue){ 
  if(strValue == "portaoPedestre"){
    FLAGPortaoPedestre = LOW;
  }
  if(strValue == "armadilhaBaixoHigh"){
    FLAGTomadaSala = HIGH;
    setPin(PIN_TOMADA_SALA, FLAGTomadaSala);
  }
  if(strValue == "armadilhaBaixoLow"){
    FLAGTomadaSala = LOW;
    setPin(PIN_TOMADA_SALA, FLAGTomadaSala);
  }
  
  if(strValue == "armadilhaBaixo"){armadilhaBaixo();}
  if(strValue == "armadilhaCima"){armadilhaCima();}
  if(strValue == "resetModem"){resetModem();}
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
  if(strLocal != "B"){digitalWrite(NEGATIVOIRBRINQUEDOTECA, HIGH); }
  

  
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
  if(strLocal != "M"){digitalWrite(NEGATIVOIRMEIO, LOW); }
  //if(strLocal != "E"){digitalWrite(NEGATIVOIRMEIO, LOW); }
  if(strLocal != "C"){digitalWrite(NEGATIVOIRCASAL, LOW); } 
  if(strLocal != "B"){digitalWrite(NEGATIVOIRBRINQUEDOTECA, LOW); }
  
  irrecv.enableIRIn(); // Re-enable receiver
}


void verificaTensao(int pin)
{
  double maior_Valor = 0;
  double valor_Corrente = 0;  
 
  float tensao = 127;
  float potencia = 0;
 
  for(int i = 0; i < 300; i++){
    vetCorrente[i] = analogRead(pin);
    //delayMicroseconds(600);
  }  
 
  for(int i = 0; i < 300; i++){
    if(maior_Valor < vetCorrente[i]){
      maior_Valor = vetCorrente[i];
    }
  }  
 
  maior_Valor = maior_Valor * 0.004882812;
  valor_Corrente = maior_Valor - 2.5;
  valor_Corrente = valor_Corrente * 1000;
  valor_Corrente = valor_Corrente / 66;         //sensibilidade : 66mV/A para ACS712 30A / 185mV/A para ACS712 5A
  valor_Corrente = valor_Corrente / 1.41421356;
  potencia = valor_Corrente * tensao;

  if(pin == 54){
    if(potencia > 55){
      FlagTvSala = HIGH;
    }else{
      FlagTvSala = LOW;
    }
  }

  if(pin == 55){
    if(potencia > 55){
      FlagTvBrinquedoteca = HIGH;
    }else{
      FlagTvBrinquedoteca = LOW;
    }
  }

  if(pin == 56){
    if(potencia > 55){
      FlagTvMeio = HIGH;
    }else{
      FlagTvMeio = LOW;
    }
  }
  
  if(debugTensao == true){
    Serial.print("Corrente = ");
    Serial.print(valor_Corrente);
    Serial.println(" A");
  
    Serial.print("Potencia = ");
    Serial.print(potencia);
    Serial.println(" W");
  }
  
  

}



void setup(){ 
  Serial.begin(9600);
  Ethernet.begin(myMac, myIp, gateway, subnet);//inicializa eternet
  //Wire.begin();             //inicializando a biblioteca.
  pinMode(speakerPin, OUTPUT); // speaker

  pinMode(NEGATIVOIRSALA, OUTPUT);
  pinMode(NEGATIVOIRESCADA, OUTPUT);
  pinMode(NEGATIVOIRCRIANCAS, OUTPUT);
  pinMode(NEGATIVOIRMEIO, OUTPUT);
  pinMode(NEGATIVOIRCASAL, OUTPUT);
  pinMode(NEGATIVOIRBRINQUEDOTECA, OUTPUT);
  

  digitalWrite(NEGATIVOIRSALA, LOW);
  digitalWrite(NEGATIVOIRESCADA, LOW);
  digitalWrite(NEGATIVOIRCRIANCAS, LOW);
  digitalWrite(NEGATIVOIRMEIO, LOW);
  digitalWrite(NEGATIVOIRCASAL, LOW);
  digitalWrite(NEGATIVOIRBRINQUEDOTECA, LOW);
 
  pinMode(PIN_PORTAO_PEDESTRE, OUTPUT);
  pinMode(PIN_TOMADA_SALA, OUTPUT);
  
  digitalWrite(PIN_PORTAO_PEDESTRE, FLAGPortaoPedestre);
  digitalWrite(PIN_TOMADA_SALA, FLAGTomadaSala);

  //sensor de tensão
  pinMode(sensorTVsala, INPUT);
  //fim     


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

  irrecv.enableIRIn(); // Inicializa o receptor IR 
  Serial.println(jarbasVersion);

}



void loop(){
  



/////// RECEBE IR //-----------------------------------------------------------------------------------------------------------
  if (irrecv.decode(&results)){  
    Serial.print("Valor lido 1 : ");  
    Serial.println(results.value, HEX);  
    armazenavalor = (results.value);   
    if(results.value !=0xffffffff){
      armazenavalor = (results.value);
    }
    
    //volume
    if (armazenavalor == 0x20DF40BF) {   
      //if(results.value !=0xffffffff){
        sendIr("5","S", "1587632295");// #1volMaisReciverS
      //}else{
        //sendIr("10","S", "1587632295");// #1volMenosReciverS
      //}
    }
      
    if (armazenavalor == 0x20DFC03F) { 
      //if(results.value !=0xffffffff){
        sendIr("5","S", "1587664935");// #1volMenosReciverS
      //}else{
        //sendIr("10","S", "1587664935");// #1volMenosReciverS
      //}
    }
    irrecv.resume(); //Le o próximo valor  
  } 



  //-----------------------------------------------------------------------------------------------------------------------------


  ///// portão pedestre
  if(FLAGPortaoPedestre  == LOW){
    //digitalWrite(PIN_PORTAO_PEDESTRE,FLAGPortaoPedestre);
    timerPortao = timerPortao +1;
    if(timerPortao > tempoPortaoAberto){
      FLAGPortaoPedestre = HIGH;
      timerPortao = 0;
    }
    digitalWrite(PIN_PORTAO_PEDESTRE,FLAGPortaoPedestre);
    Serial.print("flag: ");
    Serial.print(FLAGPortaoPedestre);
    Serial.print(" - timer: ");
    Serial.println(timerPortao);
  }else{
    digitalWrite(PIN_PORTAO_PEDESTRE,FLAGPortaoPedestre);
  }

 

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
  
  /////// RECEBE SERIAL //-----------------------------------------------------------------------------------------------------------
  if(Serial.available() > 0) {
    while(Serial.available() > 0){
      cmd += char(Serial.read());
      delay(10);
    }
    //Serial.println(cmd); 
  }
  if (cmd.length() >0) { 

    if(cmd == "portao"){
      FLAGPortaoPedestre = LOW;
      Serial.println("teste");
    }
    
    // switch debug
    if(cmd == "debug-on"){debugControle = true;}
    if(cmd == "debug-off"){debugControle = false;}

    if(cmd == "debug-tensao-on"){debugTensao = true;}
    if(cmd == "debug-tensao-off"){debugTensao = false;}

    if(cmd == "salaon"){
      digitalWrite(PIN_TOMADA_SALA, HIGH);
      Serial.println("aqui");
      }
    if(cmd == "salaoff"){
      digitalWrite(PIN_TOMADA_SALA, LOW);
      Serial.println("aqui 2");
      }

      if(cmd == "som1"){
      som1();
      }
      if(cmd == "som2"){
      som2();
      }
      if(cmd == "som3"){
      som3();
      }
      if(cmd == "som4"){
      som4();
      }
      if(cmd == "musica"){
      musica();
      }

    if(cmd == "teste"){
      Serial.println("");
     Serial.println(segundo);
     Serial.println(minuto);
     Serial.println(hora);
     Serial.println(diaDaSemana);
     Serial.println(diaDoMes);
     Serial.println(mes);
     Serial.println(ano);
     Serial.println("");
    }
    if (cmd.indexOf("rtc-") != -1)  {
          configuraModulo(cmd);
    }

    if (cmd == "b+")  {
          controleBrilho("+");
    }

    if (cmd == "b-")  {
          controleBrilho("-");
    }
      
    cmd = "";
  }
  //-----------------------------------------------------------------------------------------------------------------------------

  





verificaRtc();

verificaTensao(sensorTVsala);
//verificaTensao(sensorTVbrinquedoteca);
//verificaTensao(sensorTVmeio);



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
  }

  if (request.indexOf("/rtc-") != -1)  {
          comando = request.substring(6,23 );

          //POST /rtc-2214001171218 HTTP/1.1
          //123456789012345678901234
          configuraModulo(comando);
  }

  if (request.indexOf("/brilho") != -1)  {
          comando = request.substring(12,13);

          //POST /brilho+ HTTP/1.1
          //123456789012345678901234
          controleBrilho(comando);
  }

  
    
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Access-Control-Allow-Origin: *");
  client.println(""); //  do not forget this one
  //client.print(montaString());
  //client.print("teste");

StaticJsonDocument<200> doc;

  // StaticJsonObject allocates memory on the stack, it can be
  // replaced by DynamicJsonDocument which allocates in the heap.
  //
  // DynamicJsonDocument  doc(200);

  // Add values in the document
  //
  doc["sensor"] = "gps";
  doc["time"] = 1351824120;

  // Add an array.
  //
  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  serializeJson(doc, client);
  
  delay(1);
  client.stop();
  Serial.println("Client disconectado");
  Serial.println("");
    
   
  
  //-----------------------------------------------------------------------------------------------------------------------------


  

}// FIM DO LOOP  



String montaString()
{
  
    //String string1 ="";
    //string1.concat("dados({");

    //String teste = "Status";
    //string1.concat(" : ");
    //string1.concat("OnLine");
    
    //string1.concat("', Versao : '");
    //string1.concat(jarbasVersion);
/*
    string1.concat("', IP : '");
    string1.concat(myIp[0]);
    string1.concat(".");
    string1.concat(myIp[1]);
    string1.concat(".");
    string1.concat(myIp[2]);
    string1.concat(".");
    string1.concat(myIp[3]);

    string1.concat("', Porta : '");
    string1.concat("8087");

    string1.concat("', Hora : '");
    string1.concat(hora);
    string1.concat("', Minuto : '");
    string1.concat(minuto);
    string1.concat("', Segundo : '");
    string1.concat(segundo);
    string1.concat("', DiaS : '");
    string1.concat(diaDaSemana);
    string1.concat("', Dia : '");
    string1.concat(diaDoMes);
    string1.concat("', Mes : '");
    string1.concat(mes);
    string1.concat("', Ano : '");
    string1.concat(ano);

    string1.concat("', tvSala : '");
    string1.concat(FlagTvSala);

    string1.concat("', tvBrinquedoteca : '");
    string1.concat(FlagTvBrinquedoteca);

    string1.concat("', tvMeio : '");
    string1.concat(FlagTvMeio);
*/
    
    
 //Serial.println(segundo);
 //Serial.println(minuto);
 //Serial.println(hora);
 //Serial.println(diaDaSemana);
 //Serial.println(diaDoMes);
 //Serial.println(mes);
 //Serial.println(ano);

 
    //string1.concat("', IP : '"); // valor / estado
    //string1.concat(myIp[]);
    //string1.concat("'})");


  
    
    //return string1;

}


