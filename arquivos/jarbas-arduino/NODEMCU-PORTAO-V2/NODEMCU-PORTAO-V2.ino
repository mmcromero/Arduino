
//-----------------------------------FINGERPRINT - INICIO --------------------------
#include <Adafruit_Fingerprint.h>

SoftwareSerial mySerial(4, 5); // D2 - amarelo D1 - verde

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
//-----------------------------------FINGERPRINT - FIM -----------------------------


//-----------------------------------BOUNCE2 - INICIO --------------------------
#include <Bounce2.h>

#define BUTTON_PIN_FINGERPRINT 0 //D3
#define BUTTON_PIN_PORTAO 2 //D4 


#define RELE_PIN_FINGERPRINT 12 //D6
#define RELE_PIN_PORTAO 14 //D5

// Instantiate a Bounce object
Bounce debouncer1 = Bounce(); 
// Instantiate another Bounce object
Bounce debouncer2 = Bounce(); 
//-----------------------------------BOUNCE2 - FIM ----------------------------




const int pinoSensorMg = 13; //PINO DIGITAL UTILIZADO PELO SENSOR











int ledState = HIGH;
int portaoState = HIGH;

int ativaFingerPrint; // 0 desativado - 1 ativado
int porcentagemId;

int timerPortao=0;
int timerFingerprint=0;








void setup()  
{
  Serial.begin(9600);


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
  } else {
    Serial.println("Sensor não encontrado :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contem "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Aguardando por uma digital válida...");


  pinMode(BUTTON_PIN_FINGERPRINT,INPUT_PULLUP);
  debouncer1.attach(BUTTON_PIN_FINGERPRINT);
  debouncer1.interval(5); // interval in ms
  
  pinMode(BUTTON_PIN_PORTAO,INPUT_PULLUP);
  debouncer2.attach(BUTTON_PIN_PORTAO);
  debouncer2.interval(5); // interval in ms
  
  if (finger.verifyPassword()) {
     Serial.println("Configuração inicial realizada com sucesso!");
  } else {
    Serial.println("Configuração inicial realizada com ERRO! =( ");
  }

  digitalWrite(RELE_PIN_FINGERPRINT,LOW); // desativo o rele da digital para deixar o led apagado
  

}

void loop()                     // run over and over again
{
  //getFingerprintIDez();
  //delay(50);            //don't ned to run this at full speed.

    // Update the Bounce instances :
  debouncer1.update(); // fingerprint 
  debouncer2.update(); // portao

  // Get the updated value :
  int btFingerprint = debouncer1.read();
  int btPortao = debouncer2.read();
  
 
  

  if ( btFingerprint == LOW ) {
    digitalWrite(RELE_PIN_FINGERPRINT,HIGH);
    ativaFingerPrint = 1;
    Serial.println("Ativa fingerprint");
  } 
  
  if(ativaFingerPrint  == 1){
    timerFingerprint = timerFingerprint +1;
    if(timerFingerprint > 25){
      ativaFingerPrint = 0;
      timerFingerprint = 0;
    }
    //int idUser = getFingerprintIDez();
    //verificaUsuario(idUser);


getFingerprintID();
    
    delay(50);
  }else{
    digitalWrite(RELE_PIN_FINGERPRINT, LOW );
  }

  if(portaoState  == LOW || btPortao == LOW){
   //Serial.println("Abre o Portão");
    timerPortao = timerPortao +1;
    if(timerPortao > 15){
      portaoState = HIGH;
      timerPortao = 0;
      //Serial.println("Fecha o Portão");
    }
    digitalWrite(RELE_PIN_PORTAO, LOW );
    //Serial.println(timerPortao);
  }else{
    digitalWrite(RELE_PIN_PORTAO, HIGH );
  }

  if (digitalRead(pinoSensorMg) == HIGH){ //SE A LEITURA DO PINO FOR IGUAL A HIGH, FAZ
        //Serial.println("portão aberto");
  }
  else{ //SENÃO, FAZ
      //Serial.println("portão fechado");
  }
 

} // FIM DO LOOP


void verificaUsuario(int id){
  int liberado = 0;

  
  if(id >= 1 && id <= 4){
    Serial.print("Bem vindo Marco - ");
    Serial.println(porcentagemId);
    liberado = 1;
  }
  if(id >= 5 && id <= 6){
    Serial.print("Bem vindo Arthur - ");
    Serial.println(porcentagemId);
    liberado = 1;
  }

  if(id >= 7 && id <= 8){
    Serial.print("Bem vinda Gisele - ");
    Serial.println(porcentagemId);
    liberado = 1;
  }

  if(id >= 9 && id <= 10){
    Serial.print("Bem vinda Raquel - ");
    Serial.println(porcentagemId);
    liberado = 1;
  }

  if(id >= 11 && id <= 12){
    Serial.print("Bem vinda Renata - ");
    Serial.println(porcentagemId);
    liberado = 1;
  }

  if(id >= 13 && id <= 14){
    Serial.print("Bem vinda Mercia - ");
    Serial.println(porcentagemId);
    liberado = 1;
  }

  if(liberado == 1){
    portaoState = LOW;
    
  }
}



uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
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
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  return finger.fingerID;
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
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}


