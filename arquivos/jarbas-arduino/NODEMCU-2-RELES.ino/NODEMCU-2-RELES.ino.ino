#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

WiFiServer server(80);
/**************************************************************************************
 * this example shows how to set a static IP configuration for the ESP
 * although the IP shows in the config portal, the changes will revert 
 * to the IP set in the source file.
 * if you want the ability to configure and persist the new IP configuration
 * look at the FS examples, which save the config to file
 *************************************************************************************/

#define RELE_PIN_1 12 //D6
#define RELE_PIN_2 14 //D5

//default custom static IP
char static_ip[16] = "192.168.0.95";
char static_gw[16] = "192.168.0.1";
char static_sn[16] = "255.255.255.0";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set static ip
  //block1 should be used for ESP8266 core 2.1.0 or newer, otherwise use block2

  //start-block1
  IPAddress _ip,_gw,_sn;
  _ip.fromString(static_ip);
  _gw.fromString(static_gw);
  _sn.fromString(static_sn);
  //end-block1

  //start-block2
  //IPAddress _ip = IPAddress(192, 168, 0, 48);
  //IPAddress _gw = IPAddress(192, 168, 0, 1);
  //IPAddress _sn = IPAddress(255, 255, 255, 0);
  //end-block2
  
  wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);


  //tries to connect to last known settings
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP" with password "password"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");


  Serial.println("local ip");
  Serial.println(WiFi.localIP());

  // Start the server
    server.begin();
    Serial.println("Server started");
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
  client.println("<a href=\"/RELE1=ON\"\"><button>Rele 1 on</button></a>");
  client.println("<a href=\"/RELE1=OFF\"\"><button>Rele 1 Off </button></a><br />");  
  client.println("<a href=\"/RELE2=ON\"\"><button>Rele 2 on</button></a>");
  client.println("<a href=\"/RELE2=OFF\"\"><button>Rele 2 Off </button></a><br />"); 
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disconectado");
  Serial.println("");


}
