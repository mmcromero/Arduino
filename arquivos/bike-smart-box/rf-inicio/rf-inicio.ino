
#include <SimpleTimer.h> //INCLUSÃO DE BIBLIOTECA

SimpleTimer timer; //OBJETO DO TIPO SimpleTimer

const int pinoD0 = 2; //PINO DIGITAL UTILIZADO PARA CONECTAR A SAÍDA DO RECEPTOR RF

const int sensorTensao = A2; //PINO ANALÓGICO UTILIZADO PELO SENSOR DE TENSÃO AC
const int pinoRele = 3; //PINO DIGITAL UTILIZADO PELO MÓDULO RELÉ

String statusLamp = "DESLIGADA"; //VARIÁVEL PARA CONTROLE DE STATUS MOSTRADO NO MONITOR SERIAL

int estadoRele = LOW; //ESTADO INICIAL DO RELÉ (DESLIGADO)
int estadoBotao = LOW; //ESTADO INICIAL DO BOTÃO D0 (DESLIGADO)

void setup(){
  Serial.begin(9600); //INICIALIZA A SERIAL
  pinMode(pinoD0, INPUT); //DEFINE A PORTA COMO ENTRADA
  pinMode(sensorTensao, INPUT); //DEFINE A PORTA COMO ENTRADA
  pinMode(pinoRele, OUTPUT); //DEFINE A PORTA COMO SAÍDA  
  digitalWrite(pinoRele, estadoRele); //MÓDULO RELÉ INICIA DESLIGADO (estadoRele = LOW)
  timer.setInterval(100L, verificaBotaoControle); //DEFINE O INTERVALO DE 100 MILISSEGUNDOS(100L = 100 MILISSEGUNDOS)
  //PARA EXECUÇÃO DA FUNÇÃO verificaBotaoControle()
}

//FUNÇÃO QUE VERIFICA O PRESSIONAMENTO DO BOTÃO 'A' DO TRANSMISSOR RF (CONTROLE REMOTO)
void verificaBotaoControle(){
  if (digitalRead(pinoD0) == HIGH) { //SE LEITURA DO BOTÃO FOR IGUAL A HIGH, FAZ
    
    if (estadoBotao != HIGH) { //SE O ESTADO DO BOTÃO FOR DIFERENTE DE HIGH, OU SEJA, SE FOR IGUAL A LOW, FAZ
      estadoRele = !estadoRele; //VARIÁVEL RECEBE O ESTADO ATUAL INVERTIDO (ESTADO ATUAL = LOW, LOGO, RECEBE HIGH)
      digitalWrite(pinoRele, estadoRele); //MUDA O ESTADO DA PORTA EM QUE O RELÉ ESTÁ CONECTADO
    }
    estadoBotao = HIGH; //ESTADO DO BOTÃO RECEBE HIGH
  }else{ //SENÃO, FAZ
    estadoBotao =LOW; //ESTADO DO BOTÃO RECEBE LOW
  }
}

//FUNÇÃO RESPONSÁVEL POR VERIFICAR SE A LÂMPADA ESTÁ ACESA OU APAGADA
void verificaStatusLamp(){
  for (int i = 0; i < 100; i++){ //PARA "i" IGUAL A 0, ENQUANTO "i" MENOR QUE 100, INCREMENTA "i"
    if(analogRead(sensorTensao) > 70){ //SE LEITURA FOR MAIOR QUE 70, FAZ
        statusLamp = "LIGADA"; //VARIÁVEL RECEBE O TEXTO
    }else{ //SENÃO
      statusLamp = "DESLIGADA"; //VARIÁVEL RECEBE O TEXTO
    }
  }
}

void loop(){

  verificaBotaoControle(); //CHAMA A FUNÇÃO RESPONSÁVEL POR VERIFICAR SE O BOTÃO 'A' DO CONTROLE REMOTO FOI PRESSIONADO
  verificaStatusLamp();//CHAMA A FUNÇÃO RESPONSÁVEL POR VERIFICAR SE A LÂMPADA ESTÁ ACESA OU APAGADA

  Serial.print("LAMPADA: "); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.println(statusLamp); //IMPRIME O TEXTO NO MONITOR SERIAL
}


