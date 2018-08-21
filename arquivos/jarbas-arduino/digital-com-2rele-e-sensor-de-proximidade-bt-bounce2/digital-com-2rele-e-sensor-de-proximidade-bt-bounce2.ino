#include <Bounce2.h>
#define BUTTON_PIN 13

#define LED_PIN 8
#define PORTAO_PIN 9
int ledState = LOW;
int portaoState = HIGH;


int pinosensor = A1; 
int leitura; //Armazena o valor lido pelo sensor  



int porcentagemId;

int timerPortao=0;

// Instantiate a Bounce object :
Bounce debouncer = Bounce(); 



#include <Adafruit_Fingerprint.h>

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

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
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  
  // After setting up the button, setup the Bounce instance :
  debouncer.attach(BUTTON_PIN);
  debouncer.interval(500);
  
  // Setup the LED :
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,ledState);
  pinMode(PORTAO_PIN,OUTPUT);
  digitalWrite(PORTAO_PIN,portaoState);

  //Define o pino do sensor optico como entrada 
  pinMode(pinosensor, INPUT);  
}

void loop()                     // run over and over again
{
  
  
  leitura = digitalRead(pinosensor); 
 
  if (leitura != 1) //Verifica se o objeto foi detectado  
  {  
    digitalWrite(LED_PIN,HIGH);
    int idUser = getFingerprintIDez();
    verificaUsuario(idUser); 
  }else{
     digitalWrite(LED_PIN,LOW);
  }


  // Update the Bounce instance :
  debouncer.update();

  // Get the updated value :
  int value = debouncer.read();

  // Turn on or off the LED as determined by the state :
  if ( value == LOW ) {
    digitalWrite(PORTAO_PIN, LOW );
  } 
  else {
    digitalWrite(PORTAO_PIN, HIGH );
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

  
      
}


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
