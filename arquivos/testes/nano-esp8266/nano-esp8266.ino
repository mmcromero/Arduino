// Programa : Teste modulo wireless ESP8266
// Autor : Arduino e Cia

// Carrega as bibliotecas ESP8266 e SoftwareSerial
#include "ESP8266.h"
#include "SoftwareSerial.h"
    
// Cria uma serial nas portas 2 (RX) e 3 (TX)
SoftwareSerial minhaSerial(2 , 3); 

// Define que o modulo ira utilizar a serial minhaSerial
ESP8266 wifi(minhaSerial);

// Configuracao ID e senha da rede Wireless
#define SSID        "aqui-em-cima"
#define PASSWORD    "$3m$3nh4"


void setup()
{
    Serial.begin(9600);
    
    Serial.print("Inicializando modulo\r\n");
    Serial.print("Versao do firmware: ");
    Serial.println(wifi.getVersion().c_str());
    // Define modo de operacao como STA (station)
    if (wifi.setOprToStation()) {
        Serial.print("Modo STA ok\r\n");
    } else {
        Serial.print("Erro ao definir modo STA !r\n");
    }
    
    // Conexao a rede especificada em SSID
    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Conectado com sucesso a rede wireless\r\n");
        Serial.print("IP: ");       
        Serial.println(wifi.getLocalIP().c_str());
    } else {
        Serial.print("Erro ao conectar rede wireless !!!\r\n");
    }
    
    Serial.print("*** Fim ***\r\n");
}

void loop(void)
{

      Serial.print("*** oi ***\r\n");
      


    
}

void httppost () {

  wifi.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.

  if( wifi.find("OK")) {

  Serial.println("TCP connection ready");

  } delay(1000);

  String postRequest =

  "POST " + uri + " HTTP/1.0\r\n" +

  "Host: " + server + "\r\n" +

  "Accept: *" + "/" + "*\r\n" +

  "Content-Length: " + data.length() + "\r\n" +

  "Content-Type: application/x-www-form-urlencoded\r\n" +

  "\r\n" + data;

  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.

  wifi.print(sendCmd);

  wifi.println(postRequest.length() );

  delay(500);

  if(wifi.find(">")) { 
    Serial.println("Sending.."); wifi.print(postRequest);

    if( wifi.find("SEND OK")) { Serial.println("Packet sent");
    
      while (wifi.available()) {
    
        String tmpResp = wifi.readString();
    
        Serial.println(tmpResp);
    
      }
    
      // close the connection
    
      wifi.println("AT+CIPCLOSE");
    
    }
  
  }
  }
