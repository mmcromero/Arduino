int potenciometro = 0; // Define o pino do Arduino que o potenciômetro está conectado
int valor = 0;   //Variável que irá receber o valor do potenciômetro
int led = 11 ; // Define o pino do Arduino que o led está conectado

void setup() {
  Serial.begin(9600);

  pinMode(led, OUTPUT); // Seta a porta do led como saída
}

void loop() {
  valor = analogRead(potenciometro); //Recebe o valor do potenciômetro
  if(valor > 0){
    analogWrite(led, (valor/4)); //Acende o led com intensidade proporcional ao valor obtido
    Serial.println(valor); //Mostra no Serial Monitor o valor obtido do potenciômetro
  }
}
