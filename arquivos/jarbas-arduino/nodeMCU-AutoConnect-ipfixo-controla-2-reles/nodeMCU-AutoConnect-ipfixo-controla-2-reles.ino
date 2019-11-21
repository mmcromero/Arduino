#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

WiFiServer server(80);

#define RELE_PIN_1 12 //D6
#define RELE_PIN_2 14 //D5


void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    //I-O
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
    wifiManager.setAPStaticIPConfig(IPAddress(192,168,0,90), IPAddress(192,168,0,1), IPAddress(255,255,255,0));

    
    //start-block2
    IPAddress _ip = IPAddress(192, 168, 0, 95);
    IPAddress _gw = IPAddress(192, 168, 0, 1);
    IPAddress _sn = IPAddress(255, 255, 255, 0);
    //end-block2
  
    wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("AutoConnectAP");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

    // Start the server
    server.begin();
    Serial.println("Server started");
 
    // Print the IP address
    Serial.print("Use this URL to connect: ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
}

void loop() {
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

  if (request.indexOf("/RELE1=ON") != -1)  {
    digitalWrite(RELE_PIN_1,HIGH);
    Serial.println("Liga Rele 1");
  }
  if (request.indexOf("/RELE1=OFF") != -1)  {
    digitalWrite(RELE_PIN_1,LOW);
    Serial.println("Desliga Rele 1");
  }
  if (request.indexOf("/RELE2=ON") != -1)  {
    digitalWrite(RELE_PIN_2,HIGH);
    Serial.println("Liga Rele 2");
  }
  if (request.indexOf("/RELE2=OFF") != -1)  {
    digitalWrite(RELE_PIN_2,LOW);
    Serial.println("Desliga Rele 2");
  }









 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");


  client.println("<br><br>");
  client.println("<a href=\"/RELE1=ON\"\"><button>Liga Rele 1 </button></a>");
  client.println("<a href=\"/RELE1=OFF\"\"><button>Desliga Rele 1</button></a><br />");
  client.println("<a href=\"/RELE2=ON\"\"><button>Liga Rele 2 </button></a>");
  client.println("<a href=\"/RELE2=OFF\"\"><button>Desliga Rele 2</button></a><br />");    
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disconectado");
  Serial.println("");

}
 
