#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>


#define PIR1  D7
#define SMG1  D5
#define SMG2  D6

#define RELAY1 D0
#define RELAY2 D3


// Setup Wi-Fi e MQTT 

const char* ssid = "lah-de-baixo";            // SUBSTITUA PELO NOME DA SUA REDE
const char* password = "$3m$3nh4";             // SUBSTITUA PELA SENHA DA SUA REDE
const char* mqtt_server = "192.168.0.6";         // SUBSTITUA PELO NOME DO SEU SERVER
const char* mqtt_ClientID = "nodemcuQuintalFrente";
const char* topicoPir1 = "ha/quintal-da-frente/pir1";    // SUBSTITUA PELO NOME DO SEU TOPICO
const char* topicoSmg1 = "ha/quintal-da-frente/smg1";    // SUBSTITUA PELO NOME DO SEU TOPICO
const char* topicoSmg2 = "ha/quintal-da-frente/smg2";    // SUBSTITUA PELO NOME DO SEU TOPICO
const char* topicoRele1 = "ha/quintal-da-frente/rele1";    // SUBSTITUA PELO NOME DO SEU TOPICO
const char* topicoRele2 = "ha/quintal-da-frente/rele2";    // SUBSTITUA PELO NOME DO SEU TOPICO
const char* mqtt_user = "mmcromero";    // SUBSTITUA PELO NOME DO SEU USUARIO
const char* mqtt_password = "HmpPb2013";        // SUBSTITUA PELO SENHA DO SEU USUARIO
const int mqtt_port = 1883;                // SUBSTITUA PELO PORTA DO SEU TOPICO

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {

Serial.begin(9600);
  

  setup_wifi();


  

  pinMode(PIR1, INPUT_PULLUP);
  pinMode(SMG1, INPUT_PULLUP); //DEFINE O PINO COMO ENTRADA / "_PULLUP" É PARA ATIVAR O RESISTOR INTERNO
  pinMode(SMG2, INPUT_PULLUP); //DEFINE O PINO COMO ENTRADA / "_PULLUP" É PARA ATIVAR O RESISTOR INTERNO
  //DO ARDUINO PARA GARANTIR QUE NÃO EXISTA FLUTUAÇÃO ENTRE 0 (LOW) E 1 (HIGH)

  pinMode(RELAY1, OUTPUT);     
  pinMode(RELAY2, OUTPUT);
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  int trigger = digitalRead(PIR1);
  client.publish(topicoPir1, String(trigger).c_str(), true);

  int sensorMg1 = digitalRead(SMG1);
  client.publish(topicoSmg1, String(sensorMg1).c_str(), true);
  int sensorMg2 = digitalRead(SMG2);
  client.publish(topicoSmg2, String(sensorMg2).c_str(), true);

  delay(1000);
}


void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqtt_ClientID, mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(RELAY1, HIGH);   // Turn the LED on (Note that LOW is the voltage level
  }
  if ((char)payload[0] == '0') {
    digitalWrite(RELAY1, LOW);   // Turn the LED on (Note that LOW is the voltage level
  }

}
