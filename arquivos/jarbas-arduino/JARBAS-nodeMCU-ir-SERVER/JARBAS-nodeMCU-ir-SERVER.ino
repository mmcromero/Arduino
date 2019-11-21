String jarbasVersion = "Jarbas-nodeMCU-ir-SERVER";

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <IRremoteESP8266.h>

ESP8266WebServer server(8088);
IRsend irsend(4);

String cmd = "";
bool debugControle = true;


String montaString()
{
    String string1 ="";
    string1.concat("{");

    string1.concat("\"status\" : \""); // valor / estado
    string1.concat("OnLine");

    string1.concat("\", \"versao\" : \""); // valor / estado
    string1.concat(jarbasVersion);
    
    string1.concat("\"}");
    return string1;

}

void handleRoot() {
 //server.send(200, "text/html", "<html><head> <title>ESP8266 Demo</title></head><body><h1>Hello from ESP8266, you can send NEC encoded IR signals from here!</h1><p><a href=\"ir?code=551489775\">Send 551489775</a></p><p><a href=\"ir?code2=teste\">Send teste</a></p><p><a href=\"ir?code=16771222\">Send 0xFFE896</a></p></body></html>");
 //server.send(200, "text/html", "oi");
 server.send(200, "text/html", montaString());
}


int NEGATIVOIRESCADA; // = 4 ; //D2
int NEGATIVOIRCRIANCAS = 2; //D4
int NEGATIVOIRMEIO = 14; //D5
int NEGATIVOIRCASAL = 5; //D1

void handleIr(){
  for (uint8_t i=0; i<server.args(); i++){
    if(server.argName(i) == "codigo" && server.argName(i+1) == "repeticao" && server.argName(i+2) == "local") {
      unsigned long code = server.arg(i).toInt();
      int intRepeticao = server.arg(i+1).toInt();
      String strLocal = server.arg(i+2);
      
      if(strLocal != "E"){digitalWrite(NEGATIVOIRESCADA, HIGH); }
      if(strLocal != "K"){digitalWrite(NEGATIVOIRCRIANCAS, HIGH); }
      if(strLocal != "M"){digitalWrite(NEGATIVOIRMEIO, HIGH); }
      if(strLocal != "C"){digitalWrite(NEGATIVOIRCASAL, HIGH); }

      if(intRepeticao < 11){
        for (int i = 0; i < intRepeticao; i++) {
          unsigned long code = server.arg(i).toInt();
          irsend.sendNEC(code, 32);
          delay(40);
        }
        if(debugControle == true){
          Serial.print("codigo:");
          Serial.print(server.arg(i));
          Serial.print(" - repeticao:");
          Serial.print(server.arg(i+1));
          Serial.print(" - local:");
          Serial.println(server.arg(i+2));
        }
      }else{
        Serial.println("muitas repetições melhor parar");
      }
      if(strLocal != "E"){digitalWrite(NEGATIVOIRESCADA, LOW); }
      if(strLocal != "K"){digitalWrite(NEGATIVOIRCRIANCAS, LOW); }
      if(strLocal != "M"){digitalWrite(NEGATIVOIRMEIO, LOW); }
      if(strLocal != "C"){digitalWrite(NEGATIVOIRCASAL, LOW); }
      

      
    }
    if(server.argName(i) == "code"){
      unsigned long code = server.arg(i).toInt();
      irsend.sendNEC(code, 32);
      Serial.print("code: ");
      Serial.println(code);
    }
    /*
    
    if(server.argName(i) == "repeticao"){
      Serial.println("yeey :)");
      Serial.println(server.arg(i));
    }
    if(server.argName(i) == "local"){
      Serial.println("yeey 22222:)");
      Serial.println(server.arg(i));
    }
    */
  }
  handleRoot();
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}


void setup() {
    irsend.begin();
    
    // put your setup code here, to run once:
    Serial.begin(115200);

    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    wifiManager.resetSettings();
    
    //set custom ip for portal
    wifiManager.setAPStaticIPConfig(IPAddress(192,168,0,75), IPAddress(192,168,0,1), IPAddress(255,255,255,0));

    
    //start-block2
    IPAddress _ip = IPAddress(192, 168, 0, 80);
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


    server.on("/", handleRoot);
    server.on("/ir", handleIr); 
 
    server.on("/inline", [](){
      server.send(200, "text/plain", "this works as well");
    });

    server.onNotFound(handleNotFound);
  
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    // put your main code here, to run repeatedly:
    server.handleClient();


    /////// RECEBE SERIAL //-----------------------------------------------------------------------------------------------------------
    if(Serial.available() > 0) {
      while(Serial.available() > 0){
        cmd += char(Serial.read());
        delay(10);
      }
      Serial.println(cmd); 
    }
    if (cmd.length() >0) { 
    
      // switch debug
      if(cmd == "debug-on"){debugControle = true;}
      if(cmd == "debug-off"){debugControle = false;}
      cmd = "";
    }
    //-----------------------------------------------------------------------------------------------------------------------------

}
