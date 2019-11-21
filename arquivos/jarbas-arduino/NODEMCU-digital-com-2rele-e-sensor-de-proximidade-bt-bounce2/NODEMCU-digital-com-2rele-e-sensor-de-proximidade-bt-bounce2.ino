#include <Bounce2.h>
#define BUTTON_PORTAO_PIN 0
#define BUTTON_FINGERPRINT_PIN 2

#define LED_PIN 12
#define PORTAO_PIN 14
int ledState = HIGH;
int portaoState = HIGH;



int leitura; //Armazena o valor lido pelo sensor  



int porcentagemId;

int timerPortao=0;
int timerFingerprint=0;
// Instantiate a Bounce object :
Bounce debouncer = Bounce(); 
Bounce debouncer2 = Bounce(); 


#include <Adafruit_Fingerprint.h>

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial
#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 5);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");

  // Setup the button with an internal pull-up :
  pinMode(BUTTON_PORTAO_PIN,INPUT_PULLUP);
  pinMode(BUTTON_FINGERPRINT_PIN,INPUT_PULLUP);
  
  // After setting up the button, setup the Bounce instance :
  debouncer.attach(BUTTON_PORTAO_PIN);
  debouncer.interval(500);

  debouncer2.attach(BUTTON_FINGERPRINT_PIN);
  debouncer2.interval(100);
  
  // Setup the LED :
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,ledState);
  pinMode(PORTAO_PIN,OUTPUT);
  digitalWrite(PORTAO_PIN,portaoState);
 Serial.println("aqui");
 
}

void loop()                     // run over and over again
{
  
 //Serial.println(digitalRead(BUTTON_PORTAO_PIN));


  // Update the Bounce instance :
  debouncer.update();
  // Get the updated value :
  int value = debouncer.read();
  // Turn on or off the LED as determined by the state :
  if ( value == LOW ) {
    digitalWrite(PORTAO_PIN, LOW );
    Serial.println("bt 1");
  } 
  else {
    digitalWrite(PORTAO_PIN, HIGH );
    //Serial.println("bt 1 high");
  }


  
  

  

  // Update the Bounce instance :
  debouncer2.update();
  // Get the updated value :
  int value2 = debouncer2.read();
  // Turn on or off the LED as determined by the state :
  if ( value2 == LOW ) {
    digitalWrite(LED_PIN, LOW );
    leitura = 1;
    Serial.println("bt 2");
  } 
  
  if(leitura  == 1){
    timerFingerprint = timerFingerprint +1;
    if(timerFingerprint > 25){
      leitura = 0;
      timerFingerprint = 0;
    }
    digitalWrite(LED_PIN,HIGH);
    //Serial.println(timerFingerprint);
    int idUser = getFingerprintIDez();
    verificaUsuario(idUser);
  }

  if(portaoState  == LOW){
   
    timerPortao = timerPortao +1;
    if(timerPortao > 15){
      portaoState = HIGH;
      timerPortao = 0;
    }
    digitalWrite(PORTAO_PIN,portaoState);
    Serial.println(timerPortao);
  }
//Serial.println(ledState);



  
   
} //fim do loop


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
