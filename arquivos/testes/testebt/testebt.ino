int ledPin = 13; // escolha o pino para o LED
int inPin = 2;   // escolha o pino de entrada (para o interruptor)
int val = 0;     // variável para ler o estado do interruptor

void setup() {
  pinMode(ledPin, OUTPUT);  // declara-se o LED como saída
  pinMode(inPin, INPUT);    // declara-se o interruptor como entrada
}

void loop(){
  val = digitalRead(inPin);  // ler o valor de entrada
  if (val == HIGH) {         // verificar se a entrada é HIGH (interruptor livre)
    digitalWrite(ledPin, LOW);  // desligar LED
  } else {
    digitalWrite(ledPin, HIGH);  // ligar LED
  }
}
