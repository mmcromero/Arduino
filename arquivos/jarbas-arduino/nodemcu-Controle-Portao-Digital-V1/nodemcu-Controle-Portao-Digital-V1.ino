

//----------------------------------------------------------------------Bounce - Inicio
#include <Bounce2.h>

#define BUTTON_PIN_1 0



// Instantiate a Bounce object
Bounce debouncer1 = Bounce(); 


int btFingerprint = debouncer1.read();

//----------------------------------------------------------------------Bounce - Fim


//----------------------------------------------------------------------FingerPrint - Inicio
#include <Adafruit_Fingerprint.h>

#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 5);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

uint8_t id;
//----------------------------------------------------------------------FingerPrint - Fim

const int pinoSensorMg = 8; // D7 PINO DIGITAL UTILIZADO PELO SENSOR

int ledState = HIGH;
int portaoState = HIGH;

int ativaFingerPrint; // 0 desativado - 1 ativado
int porcentagemId;

int timerPortao=0;
int timerFingerprint=0;
int valTimerFingerprint = 100;
int valTimerPortao = 50;

#define RELE_PIN_FINGERPRINT 12 //D6
#define RELE_PIN_PORTAO 14 //D5

String cmd = "";
String modoOperacao = "leitura";
void setup() {

  Serial.begin(9600);
  
  
  pinMode(BUTTON_PIN_1,INPUT_PULLUP);// Setup the first button with an internal pull-up :
  debouncer1.attach(BUTTON_PIN_1);// After setting up the button, setup the Bounce instance :
  debouncer1.interval(5); // interval in ms
  

  // setando os pinos do rele
  pinMode(RELE_PIN_FINGERPRINT,OUTPUT);
  pinMode(RELE_PIN_PORTAO,OUTPUT);
  digitalWrite(RELE_PIN_FINGERPRINT,HIGH); // ativo o rele da digital para configuração inicial
  digitalWrite(RELE_PIN_PORTAO,HIGH);


  pinMode(pinoSensorMg, INPUT_PULLUP); //DEFINE O PINO COMO ENTRADA / "_PULLUP" É PARA ATIVAR O RESISTOR INTERNO //DO ARDUINO PARA GARANTIR QUE NÃO EXISTA FLUTUAÇÃO ENTRE 0 (LOW) E 1 (HIGH)

  while (!Serial); 
  delay(100);
  Serial.println("\n\nTestando o sensor de digitais");
 
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Sensor de digitais encontrado com sucesso!");
    finger.getTemplateCount();
    Serial.print("Sensor contem "); Serial.print(finger.templateCount); Serial.println(" templates");
    Serial.println("Configuração inicial realizada com sucesso!");
    Serial.println("Aguardando por uma digital válida...");
  } else {
    Serial.println("Sensor não encontrado :(");
    //while (1) { delay(1); }
    Serial.println("Configuração inicial realizada com ERRO! =( ");
  }

  digitalWrite(RELE_PIN_FINGERPRINT,LOW); // desativo o rele da digital para deixar o led apagado

}

void loop() {

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
    if(cmd == "modo-leitura"){modoOperacao = "leitura";}
    if(cmd == "modo-gravacao"){modoOperacao = "gravacao";}
    
    cmd = "";
  }
  //-----------------------------------------------------------------------------------------------------------------------------


  // Update the Bounce instances :
  debouncer1.update();

  
  // Get the updated value :
  btFingerprint = debouncer1.read();


  




  

if(modoOperacao != "gravacao"){
  modoLeitura();
}else{
  modoGravacao();
}
  



}



// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  
  porcentagemId = finger.confidence;
  return finger.fingerID; 
}

void verificaUsuario(int id){
  int liberado = 0;

  
  if(id >= 1 && id <= 10){
    Serial.print("Bem vindo Marco - ");
    Serial.print("#id - ");
    Serial.print(id);
    Serial.print(" - % ");
    Serial.println(porcentagemId);
    liberado = 1;
  }
  if(id >= 11 && id <= 20){
    Serial.print("Bem vindo Arthur - ");
    Serial.print("#id - ");
    Serial.print(id);
    Serial.print(" - % ");
    Serial.println(porcentagemId);
    liberado = 1;
  }

  if(id >= 21 && id <= 30){
    Serial.print("Bem vinda Gisele - ");
    Serial.print("#id - ");
    Serial.print(id);
    Serial.print(" - % ");
    Serial.println(porcentagemId);
    liberado = 1;
  }

  if(id >= 31 && id <= 36){
    Serial.print("Bem vindo Arlindo - ");
    Serial.print("#id - ");
    Serial.print(id);
    Serial.print(" - % ");
    Serial.println(porcentagemId);
    liberado = 1;
  }

  if(id >= 37 && id <= 42){
    Serial.print("Bem vinda Maze - ");
    Serial.print("#id - ");
    Serial.print(id);
    Serial.print(" - % ");
    Serial.println(porcentagemId);
    liberado = 1;
  }

  if(id >= 43 && id <= 48){
    Serial.print("Bem vinda Kakau - ");
    Serial.print("#id - ");
    Serial.print(id);
    Serial.print(" - % ");
    Serial.println(porcentagemId);
    liberado = 1;
  }

  if(id >= 49 && id <= 54){
    Serial.print("Bem vinda Renata - ");
    Serial.print("#id - ");
    Serial.print(id);
    Serial.print(" - % ");
    Serial.println(porcentagemId);
    liberado = 1;
  }

  if(id >= 55 && id <= 60){
    Serial.print("Bem vinda Raquel - ");
    Serial.print("#id - ");
    Serial.print(id);
    Serial.print(" - % ");
    Serial.println(porcentagemId);
    liberado = 1;
  }

    if(liberado == 1){
    portaoState = LOW;
    
  }
}




void modoLeitura(){
    if ( btFingerprint == LOW ) {
    digitalWrite(RELE_PIN_FINGERPRINT,HIGH);
    ativaFingerPrint = 1;
    Serial.println("Ativa fingerprint");
  } 
  
  if(ativaFingerPrint  == 1){
    //lcd.setBacklight(HIGH);
    timerFingerprint = timerFingerprint +1;
    if(timerFingerprint > valTimerFingerprint){
      ativaFingerPrint = 0;
      timerFingerprint = 0;
      Serial.println("Desativa fingerprint");
    }
    int idUser = getFingerprintIDez();
    verificaUsuario(idUser);


    //getFingerprintID();
    
    delay(50);
  }else{
    digitalWrite(RELE_PIN_FINGERPRINT, LOW );
    //lcd.setBacklight(LOW);
  }

  if(portaoState  == LOW){
   ativaFingerPrint = 0;
   timerFingerprint = 0;
   Serial.println("Abre o Portão");
    timerPortao = timerPortao +1;
    if(timerPortao > valTimerPortao){
      portaoState = HIGH;
      timerPortao = 0;
      //Serial.println("Fecha o Portão");
    }
    digitalWrite(RELE_PIN_PORTAO, LOW );
    //Serial.println(timerPortao);
  }else{
    digitalWrite(RELE_PIN_PORTAO, HIGH );
    
  }
}




void modoGravacao(){
  digitalWrite(RELE_PIN_FINGERPRINT,HIGH);
  Serial.println("Pronto para gravar a digital");
  Serial.println("Por favor insira o ID # (de 1 ate 127) que voce quer salvar...");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
     return;
  }
  Serial.print("Gravando ID #");
  Serial.println(id);
  
  while (!  getFingerprintEnroll() );
}



uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    
    

    if(modoOperacao != "leitura"){
      num = Serial.parseInt();
      if(num == 231){
        Serial.println("Gravação cancelada pelo usuario");
        modoOperacao = "leitura";
        num = 0;
        break;
      }
    }else{
      Serial.println("volto para leitura!");
      Serial.println("Aguardando por uma digital válida...");
      break;
    }
    
  }
  
  return num;
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Esperando um digital valida para gravar o#"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remova o dedo");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Coloque o mesmo dedo novamente");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Criando modelo para #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    // saio do modo gravação
    modoOperacao = "leitura";


    
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}



