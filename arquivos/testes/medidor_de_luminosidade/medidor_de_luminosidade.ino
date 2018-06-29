/*
Projeto Arduino sensor de luz com saida de leds.
Por Jota
----------------------------------------
--=<| www.ComoFazerAsCoisas.com.br |>=--
----------------------------------------
*/

int sensor = 1;      //Pino analógico em que o sensor está conectado.
int valorSensor = 0; //variável usada para ler o valor do sensor em tempo real.

const int ledVerde = 3;
const int ledAmarelo = 4;
const int ledVermelho = 5;

//Função setup, executado uma vez ao ligar o Arduino.
void setup(){
  //Ativando o serial monitor que exibirá os valores lidos no sensor.
  Serial.begin(9600);
  
  //Definindo pinos digitais dos leds como de saída.
  pinMode(ledVerde,OUTPUT);
  pinMode(ledAmarelo,OUTPUT);
  pinMode(ledVermelho,OUTPUT);
  
  digitalWrite(ledVerde,HIGH);
  delay(1000);
  digitalWrite(ledVerde,LOW);
  delay(1000);
  digitalWrite(ledAmarelo,HIGH);
  delay(1000);
  digitalWrite(ledAmarelo,LOW);
  delay(1000);
  digitalWrite(ledVermelho,HIGH);
  delay(1000);
  digitalWrite(ledVermelho,LOW);
}

//Função loop, executado enquanto o Arduino estiver ligado.
void loop(){
  
  //Lendo o valor do sensor.
  int valorSensor = analogRead(sensor);
 
  //Os valores da luminosidade podem ser alterados conforme a necessidade.
  //Luminosidade baixa.
  if (valorSensor < 750) {
    apagaLeds();
    digitalWrite(ledVermelho,HIGH);
  }
  
  //Luminosidade média.
  if (valorSensor >= 750 && valorSensor <= 800) {
    apagaLeds();
    digitalWrite(ledAmarelo,HIGH);
  }
  
  //Luminosidade alta.
  if (valorSensor > 800) {
    apagaLeds();
    digitalWrite(ledVerde,HIGH);
  }
  
  //Exibindo o valor do sensor no serial monitor.
  Serial.println(valorSensor);
  
  delay(50); 
}

//Função criada para apagar todos os leds de uma vez.
void apagaLeds() {
  digitalWrite(ledVerde,LOW);
  digitalWrite(ledAmarelo,LOW);
  digitalWrite(ledVermelho,LOW);
} 
