// --- WIFI ---
#include <ESP8266WiFi.h>
const char* ssid = "aqui-em-cima";          // troque pelo nome da sua rede
const char* password = "$3m$3nh4";     // troque pela senha da sua rede
WiFiClient nodemcuClient;

// --- MQTT ---
#include <PubSubClient.h>
//const char* mqtt_Broker = "mqtt.eclipse.org";
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
#define DHTPIN 2 //D3
#define DHTTYPE DHT22
DHT_Unified dht(DHTPIN, DHTTYPE);
float temperatura;
float umidade;
uint32_t delayMS;

// --- DISPLAY ---
#include <Adafruit_SSD1306.h>
#define OLED_RESET LED_BUILTIN
Adafruit_SSD1306 display(OLED_RESET);

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
  
  configurarDisplay();
  conectarWifi();
  client.setServer(mqtt_Broker, 1883);
}

void loop() {
  if (!client.connected()) {
    reconectarMQTT();
  }
  medirTemperaturaUmidade();
  mostrarTemperaturaUmidade();
  publicarTemperaturaUmidadeNoTopico();
}

void conectarWifi() {
  delay(10);

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Conectando ");
  Serial.print("Conectando");
  display.display();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.print(".");
    Serial.print(".");
    display.display();
  }
}

void reconectarMQTT() {
  while (!client.connected()) {
    client.connect(mqtt_ClientID, mqtt_user, mqtt_pass);
  }
}

void configurarDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.clearDisplay();
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
  delay(delayMS);
}

void mostrarTemperaturaUmidade() {
  mostrarMensagemNoDisplay("Temperatura", (temperatura), " C");
  mostrarMensagemNoDisplay("Umidade", (umidade), " %");
}

void mostrarMensagemNoDisplay(const char* texto1, int medicao, const char* texto2) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(texto1);
  display.setTextSize(5);
  display.setCursor(20, 20);
  display.print(medicao);
  display.setTextSize(2);
  display.print(texto2);
  display.display();
  delay(2000);
}
