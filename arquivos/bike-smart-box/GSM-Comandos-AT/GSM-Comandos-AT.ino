//Programa: Comandos AT com o SIM800L
//Autor: Arduino e Cia

#include <SoftwareSerial.h>

//TX do SIM800L conectado ao pino 10
#define SIM800_TX_PIN 11

//RX do SIM800L conectado ao pino 11
#define SIM800_RX_PIN 10

SoftwareSerial serialSIM800(SIM800_TX_PIN, SIM800_RX_PIN);

void setup()
{
  //Comunicacao serial para uso com o Serial Monitor
  Serial.begin(9600);
  while (!Serial);

  //Comunicacao com o modulo SIM800L
  serialSIM800.begin(4800);
  delay(1000);

  Serial.println("Envie o comando AT para o SIM800L...");
}

void loop()
{
  //Mostra no Serial Monitor os dados enviados pelo modulo
  if (serialSIM800.available())
  {
    Serial.write(serialSIM800.read());
  }

  //Envia os comandos AT para o modulo
  if (Serial.available())
  {
    serialSIM800.write(Serial.read());
  }
}
