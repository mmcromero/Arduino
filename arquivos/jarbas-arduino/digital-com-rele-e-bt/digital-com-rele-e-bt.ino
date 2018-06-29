#include <Bounce2.h>
#define BUTTON_PIN 7
#define LED_PIN 8
#define PORTAO_PIN 9
int ledState = LOW;
int portaoState = LOW;
// Instantiate a Bounce object :
Bounce debouncer = Bounce(); 



int porcentagemId;





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
  debouncer.interval(100);
  
  // Setup the LED :
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,ledState);
  pinMode(PORTAO_PIN,OUTPUT);
  digitalWrite(PORTAO_PIN,portaoState);


}

void loop()                     // run over and over again
{
  int idUser = getFingerprintIDez();
  verificaUsuario(idUser);
  //delay(50);            //don't ned to run this at full speed.



    // Update the Bounce instance :
   debouncer.update();
   
   // Call code if Bounce fell (transition from HIGH to LOW) :
   if ( debouncer.fell() ) {
      
     // Toggle LED state :
     ledState = !ledState;
     digitalWrite(LED_PIN,ledState);
     
   }
}


void verificaUsuario(int id){
  if(id >= 1 && id <= 4){
    Serial.print("Bem vindo Marco - ");
    Serial.println(porcentagemId);
  }
  if(id >= 5 && id <= 6){
    Serial.print("Bem vindo Arthur - ");
    Serial.println(porcentagemId);
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
