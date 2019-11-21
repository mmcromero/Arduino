// --- WIFI ---
#include <ESP8266WiFi.h>
const char* ssid = "aqui-em-cima";          // troque pelo nome da sua rede
const char* password = "$3m$3nh4";     // troque pela senha da sua rede
WiFiClient nodemcuClient;

// --- MQTT ---
#include <PubSubClient.h>
//const char* mqtt_Broker = "romeropi.duckdns.org";
const char* mqtt_Broker = "192.168.0.6";
const int mqtt_port = 1883;
const char* mqtt_ClientID = "termometro-mmcromero01";
const char* mqtt_user = "mmcromero";
const char* mqtt_pass = "HmpPb2013";
const char* topicoTemperatura = "mmcromero/temperatura";
const char* topicoUmidade = "mmcromero/umidade";
PubSubClient client(nodemcuClient);

// --- DHT ---
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN D3 //D3
#define DHTTYPE DHT22
DHT_Unified dht(DHTPIN, DHTTYPE);
float temperatura;
float umidade;
uint32_t delayMS;



void setup() {
Serial.begin(115200);
// Initialize device.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

  Serial.print("Delay do sensor DHT22: ");
  Serial.println(delayMS);
  
  conectarWifi();
  client.setServer(mqtt_Broker, 1883);
}

void loop() {
  if (!client.connected()) {
    reconectarMQTT();
  }
  medirTemperaturaUmidade();
  

  
  
  
  publicarTemperaturaUmidadeNoTopico();
  delay(delayMS);
}

void conectarWifi() {
  delay(10);

  Serial.print("Conectando");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    
    Serial.print(".");
  }
  Serial.println("Conectado");
}

void reconectarMQTT() {
  while (!client.connected()) {
    client.connect(mqtt_ClientID, mqtt_user, mqtt_pass);
  }
}

void publicarTemperaturaUmidadeNoTopico() {
  client.publish(topicoTemperatura, String(temperatura).c_str(), true);
  client.publish(topicoUmidade, String(umidade).c_str(), true);
}

void medirTemperaturaUmidade() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  temperatura = event.temperature;
  dht.humidity().getEvent(&event);
  umidade = event.relative_humidity;  
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" - Humidade: ");
  Serial.println(umidade);
}

