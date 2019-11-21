String jarbasVersion = "Jarbas-CIMA-V1";

/* ------------------------------------------------------------------ ETHERNET ------------------------------------------------------------------ */
//ethernet
#include <Ethernet.h>
#include <SPI.h>
EthernetServer server(8088); // Cria o servidor na porta 8080

byte myMac[]={0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};  //Mac PRODUÇÃO
byte myIp[]  = { 192, 168, 0, 8 };   //define IP da central automação 1

byte gateway[] = { 192,168,0, 1 }; // ip do roteador
byte subnet[] = { 255, 255, 255, 0 };
/* ------------------------------------------------------------------ ETHERNET ------------------------------------------------------------------ */


#include <ArduinoJson.h>




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





int Rele_Deck_Principal = 33;
int Rele_Corredor_Deck = 35;

int ESTADO_Rele_Deck_Principal = HIGH ;
int ESTADO_Rele_Corredor_Deck = HIGH ;

//sensor de tensão
float vetCorrente[300];
const int correnteReleDeckPrincipal = A15;
const int correnteReleCorredorDeck = A14;

// fim

String cmd = "";
String comando = "";
String estado = "";
String strTipo = "";
String strRepeticao = "";
String strLocal = "N"; 
String strTamanho = ""; 
int intTamanho; 

bool debugControle = false;
bool debugTensao = false;
//----------------------------











void setPin(int pin, int flag){
  digitalWrite(pin, flag);  
  if(debugControle == true){
    Serial.print("pin: ");
    Serial.print(pin);
    Serial.print(" flag: ");
    Serial.println(flag);
  }
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

  //Serial.print("PIN = ");
  //Serial.println(pin);
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


  if(pin == 69){
    int Old_State_Rele_Deck_Principal = ESTADO_Rele_Deck_Principal;
    if(potencia > 55){
      ESTADO_Rele_Deck_Principal = HIGH;
      //Serial.println("ligado pin 69");
    }else{
      ESTADO_Rele_Deck_Principal = LOW;
      //Serial.println("desligado pin 69");
    }
    if(ESTADO_Rele_Deck_Principal != Old_State_Rele_Deck_Principal){
      Serial.print("Deck Principal: ");
      Serial.println(ESTADO_Rele_Deck_Principal);
    }
  }

  if(pin == 68){
    int Old_State_Rele_Corredor_Deck = ESTADO_Rele_Corredor_Deck;
    if(potencia > 55){
      ESTADO_Rele_Corredor_Deck = HIGH;
      //Serial.println("ligado pin 68");
    }else{
      ESTADO_Rele_Corredor_Deck = LOW;
      //Serial.println("desligado pin 68");
    }

    if(ESTADO_Rele_Corredor_Deck != Old_State_Rele_Corredor_Deck){
      Serial.print("Corredor deck: ");
      Serial.println(ESTADO_Rele_Corredor_Deck);
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


void controlaLuz(int pin){
  //digitalWrite(pin, estado.toInt());
  digitalWrite(pin, !digitalRead(pin));
}
//inString.toInt()

void setup(){ 
  Serial.begin(9600);
  Ethernet.begin(myMac, myIp, gateway, subnet);//inicializa eternet

  

 
  pinMode(Rele_Deck_Principal, OUTPUT);
  pinMode(Rele_Corredor_Deck, OUTPUT);
  
  digitalWrite(Rele_Deck_Principal, HIGH);
  digitalWrite(Rele_Corredor_Deck, HIGH);

  //sensor de tensão
  pinMode(correnteReleDeckPrincipal, INPUT);
  pinMode(correnteReleCorredorDeck, INPUT);
  //fim     

  irrecv.enableIRIn(); // Inicializa o receptor IR 
  Serial.println(jarbasVersion);

}



void loop(){
  

  
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
      //FLAGPortaoPedestre = LOW;
      Serial.println("teste");
    }
    
    // switch debug
    if(cmd == "debug-on"){debugControle = true;}
    if(cmd == "debug-off"){debugControle = false;}

    if(cmd == "debug-tensao-on"){debugTensao = true;}
    if(cmd == "debug-tensao-off"){debugTensao = false;}

    if(cmd == "rele10n"){
      digitalWrite(Rele_Deck_Principal, HIGH);
      Serial.println("aqui");
      }
    if(cmd == "rele10ff"){
      digitalWrite(Rele_Deck_Principal, LOW);
      Serial.println("aqui 2");
      }

      if(cmd == "rele20n"){
      digitalWrite(Rele_Corredor_Deck, HIGH);
      Serial.println("aqui");
      }
    if(cmd == "rele20ff"){
      digitalWrite(Rele_Corredor_Deck, LOW);
      Serial.println("aqui 2");
      }

      
      
    cmd = "";
  }
  //-----------------------------------------------------------------------------------------------------------------------------

 
verificaTensao(correnteReleDeckPrincipal);
verificaTensao(correnteReleCorredorDeck);




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
    if(request.indexOf("POST /") != -1)  {
      estado = request.substring(11,12);
      comando = request.substring(12,intTamanho -9 );
      Serial.println("POST");
    }else if(request.indexOf("GET /") != -1)  {
      estado = request.substring(10,11);
      comando = request.substring(11,intTamanho -9 );
      Serial.println("GET");
    }

    
    Serial.print("Rele: ");
    Serial.println(comando);
    Serial.print("Estado: ");
    Serial.println(estado);

    if(comando == "Rele_Deck_Principal"){
      if(estado.toInt() != ESTADO_Rele_Deck_Principal){
        Serial.println("estado diferente");
        controlaLuz(Rele_Deck_Principal);
        
      }
    }
    if(comando == "Rele_Corredor_Deck"){ 
      if(estado.toInt() != ESTADO_Rele_Corredor_Deck){
        Serial.println("estado diferente");
        controlaLuz(Rele_Corredor_Deck);
        
      }
      
    
    }
    
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


  doc["Rele_Deck_Principal"] = ESTADO_Rele_Deck_Principal;
  doc["Rele_Corredor_Deck"] = ESTADO_Rele_Corredor_Deck;

  // Add an array.
  //
  //JsonArray data = doc.createNestedArray("data");
  //data.add(48.756080);
  //data.add(2.302038);

  serializeJson(doc, client);
  
  delay(1);
  client.stop();
  Serial.println("Client disconectado");
  Serial.println("");
    
   
  
  //-----------------------------------------------------------------------------------------------------------------------------


  

}// FIM DO LOOP  





