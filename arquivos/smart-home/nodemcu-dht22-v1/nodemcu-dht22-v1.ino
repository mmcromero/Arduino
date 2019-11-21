#include <ESP8266WiFi.h>
const char* ssid = "aqui-em-cima";
const char* password = "$3m$3nh4";
WiFiClient nodemcuClient;

#include <PubSubClient.h>
const char* mqtt_server = "mqtt.eclipse.org";
const char* mqtt_ClientID = "termometro-mmcromero";
PubSubClient client(nodemcuClient);
const char* topicoTemperatura = "mmcromero/temperatura";
const char* topicoUmidade = "mmcromero/umidade";

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN D3
#define DHTTYPE DHT22

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;
float temperatura;
float umidade;

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
  conectarWifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {

  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  temperatura = event.temperature;
  dht.humidity().getEvent(&event);
  umidade = event.relative_humidity;
  

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" - Humidade: ");
  Serial.println(umidade);
  delay(5000);
  
  if(!client.connected()){
    reconectarMqtt();
  }
  publicarTemperaturaUmidade();

}


// configuração e conexão wifi
void conectarWifi(){
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

// reconectar client mqtt
void reconectarMqtt() {
  // Loop until we're reconnected
  while (!client.connected()) {
    client.connect(mqtt_ClientID);
  }
}

//publica tem e umidade
void publicarTemperaturaUmidade(){
  client.publish(topicoTemperatura, String(temperatura).c_str(), true );
  client.publish(topicoUmidade, String(umidade).c_str(), true );
}


