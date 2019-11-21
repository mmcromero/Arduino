//------------------------------- SERVIDOR WEB - INICIO -------------------------------

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

WiFiServer server(80);
//------------------------------- SERVIDOR WEB - FIM ----------------------------------

#define RELE_PIN_1 12 //D6
#define RELE_PIN_2 14 //D5


void setup()  
{
  Serial.begin(9600);


  // setando os pinos do rele
  pinMode(RELE_PIN_1,OUTPUT);
  pinMode(RELE_PIN_2,OUTPUT);
  digitalWrite(RELE_PIN_1,HIGH); // ativo o rele da digital para configuração inicial
  digitalWrite(RELE_PIN_2,HIGH);

  //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
    
    //set custom ip for portal
    wifiManager.setAPStaticIPConfig(IPAddress(192,168,0,60), IPAddress(192,168,0,1), IPAddress(255,255,255,0));

    
    //start-block2
    IPAddress _ip = IPAddress(192, 168, 0, 60);
    IPAddress _gw = IPAddress(192, 168, 0, 1);
    IPAddress _sn = IPAddress(255, 255, 255, 0);
    //end-block2
  
    wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);


    wifiManager.setTimeout(180);
  
    if(!wifiManager.autoConnect("NodeFront")) {
      Serial.println("timeout falha ao se conectar....");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      //ESP.reset();
      delay(5000);
    } else{
      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");
    }

    
    // Start the server
    server.begin();
    Serial.println("Server started");
 
    // Print the IP address
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");

}

void loop()                     // run over and over again
{



 

  if ( btFingerprint == LOW ) {
    digitalWrite(RELE_PIN_FINGERPRINT,HIGH);
    ativaFingerPrint = 1;
    Serial.println("Ativa fingerprint");
  } 
  
  if(ativaFingerPrint  == 1){
    timerFingerprint = timerFingerprint +1;
    if(timerFingerprint > 5){
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
  }

  if(portaoState  == LOW || btPortao == LOW){
   //Serial.println("Abre o Portão");
    timerPortao = timerPortao +1;
    if(timerPortao > 5){
      portaoState = HIGH;
      timerPortao = 0;
      //Serial.println("Fecha o Portão");
    }
    digitalWrite(RELE_PIN_PORTAO, LOW );
    //Serial.println(timerPortao);
  }else{
    digitalWrite(RELE_PIN_PORTAO, HIGH );
  }






  

// Check if a client has connected
  WiFiClient client = server.available();
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
  Serial.print("Request: ");
  Serial.println(request);
  
  client.flush();

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");


  client.println("<br><br>");
  client.println("<a href=\"/RELE1=ON\"\"><button>Rele 1 on</button></a>");
  client.println("<a href=\"/RELE1=OFF\"\"><button>Rele 1 Off </button></a><br />");  
  client.println("<a href=\"/RELE2=ON\"\"><button>Rele 2 on</button></a>");
  client.println("<a href=\"/RELE2=OFF\"\"><button>Rele 2 Off </button></a><br />"); 
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disconectado");
  Serial.println("");
 

} // FIM DO LOOP









