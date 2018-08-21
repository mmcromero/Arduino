/* MÓDULO RTC COM DS1307 e EEPROM 24C32:
   Como o módulo RTC utiliza o barramento I2C para se comunicar 
   com o Arduino então deve-se conectar o módulo ao Arduino ligando-se
   o pino SCL do módulo na porta analógica A5 do Arduino, o pino SDA
   na porta analógica A4 do Arduino, o pino positivo VCC no 5V e o 
   pino negativo no GND.
   Para o correto funcionamento deste módulo é necessário que a bateria
   de 3V esteja inserida no suporte, localizado no verso da placa.
*/

///////////////////////////////////
// INICIALIZAÇÃO DAS BIBLIOTECAS //
///////////////////////////////////     
/* A biblioteca Wire foi desenvolvida para permitir a comunicação
   serial através do I2C(Inter-Integrated Circuit), que é um barramento
   serial multi-mestre desenvolvido pela Philips utilizado para conectar
   periféricos de baixa velocidade a uma placa mãe ou sistemas embarcados.
*/

#include <Wire.h>


/////////////////////////////////
// INICIALIZAÇÃO DAS VARIÁVEIS //
/////////////////////////////////     
/* Como a comunicação será realizada através do barramento I2C a primeira
   coisa que precisa saber é o endereço, que no caso é "0x68". Logo 
   define-se uma variável para armazenar esse endereço.
*/
#define enderecoI2C 0x68
 
/* Como este módulo trabalha em grupos de 7 Bytes, então define-se as
   variáveis para esse conjunto de 7 dados, que são as horas e data.
*/
byte segundo, minuto, hora, diaDaSemana, diaDoMes, mes, ano;


///////////
// SETUP //
/////////// 
void setup() 
{
  Serial.begin(9600);       //configurando a comunicação via porta 
                            //serial à uma velocidade de 9600bps(baud).
  
  Wire.begin();             //inicializando a biblioteca.
  
  configuraModulo();         //chamando a função para configurar o módulo.
}


///////////
// LOOP  //
/////////// 
void loop() 
{
  imprimeDadosModulo();    //chamando a função para imprimir na tela os dados
                           //gravados no módulo.
  delay(2000);             //aguardando 2 segundos para próxima leitura.
}


////////////
// FUNÇÃO //
////////////
/* Essa função configura o módulo com dados de hora e data inicialmente desejados. 
   Essa função deve ser executada uma única vez na inicialização do módulo, pois 
   ao definir os dados inicias no módulo, fará como que ele comece a contar o
   tempo e nunca mais pare de contar enquanto tiver energia, seja ela fornecida
   pela fonte principal ou pela bateria auxiliar de 3V.  
   Logo para as próximas compilações deve-se colocar um comentário na chamada 
   da função configuraModulo() localizada no setup().  
  
   Para escrever os dados de hora e data no módulo deve-se realizar os seguintes passos:
        1) Abrir a comunicação I2C em modo de gravação;
        2) Redefinir o ponteiro para o primeiro registro (0x00);
        3) Escrever sete bytes de dados;
        4) Finalizar o modo de gravação.
*/
void configuraModulo()
{
  /* Inicializando as variáveis abaixo com os dados de hora e data desejados.
     Lembrando que quando o dado for menor que 10, ou seja, tiver apenas um dígito
     não deve-se digitar o zero à esquerda. Exemplo: se deseja armazenar na variável hora
     o valor de 9horas, digite apenas o dígito 9, nunca 09.
  */
  segundo = 0;
  minuto = 46;
  hora = 23;
  diaDaSemana = 4; // 1-segunda 2-terça 3-quarta 4-quinta 5-sexta 6-sabado
  diaDoMes = 16;
  mes = 8;
  ano = 18;

  Wire.beginTransmission(enderecoI2C); //Abrindo o modo I2C no modo de gravação.
  Wire.write((byte)0x00);   //Redefinindo o ponteiro para o primeiro registro (0x00).
 
  //Para escrever dados no módulo é necessário uma conversão de Decimal para Binário
  Wire.write(decToBcd(segundo));      //convertendo os segundos.
  Wire.write(decToBcd(minuto));       //convertendo os minutos.
  Wire.write(decToBcd(hora));         //convertendo as horas.
  Wire.write(decToBcd(diaDaSemana));  //dia da semana, onde o domingo começa com "0".
  Wire.write(decToBcd(diaDoMes));     //convertendo o dia do mês.
  Wire.write(decToBcd(mes));          //convertendo o mês.
  Wire.write(decToBcd(ano));          //convertendo o ano.
  Wire.endTransmission();             //finalizando o modo de gravação.
}


/* Essa função é reponsável por ler os dados de hora e data gravados no módulo e 
   imprimi-los na tela do Monitor Serial.
*/
void imprimeDadosModulo()
{
  /* Como para valores menores que 10 o dado armazenado no módulo possui apenas
     um dígito, então será criado variáveis de ajustes, permitindo que no momento
     da impressão na tela esses valores com apenas um dígito sejam mostrados com  
     um zero à esquerda. Exemplo: ao invés de 9:58:10 a impressão ficará 09:58:10  
  */
  String ajustaSegundo;
  String ajustaMinuto;
  String ajustaHora;
  String ajustaDiaDoMes;
  String ajustaMes;
  
  /* Para ler os dados de hora e data no módulo deve-se realizar os seguintes passos:
        1) Abrir a comunicação I2C em modo de gravação;
        2) Redefinir o ponteiro para o primeiro registro (0x00);
        3) Finalizar o modo de gravação;
        4) Abrir a comunicação I2C em modo de leitura;
        5) Ler os sete bytes de dados.
  */
  Wire.beginTransmission(enderecoI2C); //Abrindo o modo I2C no modo de gravação.
  Wire.write((byte)0x00); //Redefinindo o ponteiro para o primeiro registro (0x00).
  Wire.endTransmission(); //finalizando o modo de gravação.
  Wire.requestFrom(enderecoI2C, 7);    //Abrindo o modo I2C no modo de leitura.

  //Para ler e posteriormente imprimir os dados lidos do módulo é necessário uma 
  //conversão de Binário para Decimal.
  segundo = bcdToDec(Wire.read() & 0x7f); //Alguns dados precisam de máscaras antes 
                                          //da conversão porque certos bits são 
                                          //bits de controle. 
  minuto = bcdToDec(Wire.read());         //convertendo os minutos. 
  hora = bcdToDec(Wire.read() & 0x3f);    //Alguns dados precisam de máscaras antes 
                                          //da conversão porque certos bits são 
                                          //bits de controle. Essa máscara define o
                                          //relógio para trabalhar no modo de 24h.
  diaDaSemana = bcdToDec(Wire.read());    //dia da semana, onde domingo começa com "0".
  diaDoMes = bcdToDec(Wire.read());       //convertendo o dia do mês.
  mes = bcdToDec(Wire.read());            //convertendo o mês.
  ano = bcdToDec(Wire.read());            //convertendo o ano.
 
  //Imprimindo os dois dígitos das horas separados por dois pontos.
  Serial.print("Agora sao: ");
  //chmando a função ajustaZero para o caso de dados gravado no módulo com apenas um dígito.
  ajustaHora += ajustaZero(hora); 
  Serial.print(ajustaHora);
  Serial.print(":");
  //chmando a função ajustaZero para o caso de dados gravado no módulo com apenas um dígito.
  ajustaMinuto += ajustaZero(minuto);
  Serial.print(ajustaMinuto);
  Serial.print(":");
  //chmando a função ajustaZero para o caso de dados gravado no módulo com apenas um dígito.
  ajustaSegundo += ajustaZero(segundo);
  Serial.println(ajustaSegundo);
  
  Serial.print("Dia da semana: ");

  switch(diaDaSemana)
  {
    case 0:Serial.println("Domingo"); break; //se a variável diaDaSemana for zero
                                             //imprima na tela "Domingo"    
    case 1:Serial.println("Segunda-feira"); break; //idem ao anterior
    case 2:Serial.println("Terca-feira"); break;   //idem
    case 3:Serial.println("Quarta-feira"); break;  //idem
    case 4:Serial.println("Quinta-feira"); break;  //idem
    case 5:Serial.println("Sexta-feira"); break;   //idem
    case 6:Serial.println("Sabado"); break;        //idem
  }
  
  //Imprimindo os dois dígitos das datas separadas por uma barra.
  Serial.print("Data: ");
  //chmando a função ajustaZero para o caso de dados gravado no módulo com apenas um dígito.
  ajustaDiaDoMes += ajustaZero(diaDoMes);
  Serial.print(ajustaDiaDoMes);
  Serial.print("/");
  //chmando a função ajustaZero para o caso de dados gravado no módulo com apenas um dígito.
  ajustaMes += ajustaZero(mes);
  Serial.print(ajustaMes);
  Serial.print("/");
  Serial.println(ano);
 
  Serial.println(); //salta uma linha.
}


/* Função que realiza uma conversão de Decimal para Binário.
   Utilizada na gravação de dados no módulo.
*/
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}
 
/* Função que realiza uma conversão de Binário para Decimal.
   Utilizada na impressão dos dados na tela do Monitor Serial.
*/
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}


/* Essa função insere o dígito "0" à esquerda dos dados gravados no módulo com 
   apenas um dígito, já que os valores menores que 10 são armazenados no módulo
   com apenas um dígito.  
*/
String ajustaZero(byte dado)
{
  String dadoAjustado;
  if (dado < 10) 
  {
    dadoAjustado += "0";  //concatena o dígito "0" ao valor da variável.
  }
  dadoAjustado += dado;   //concatena o dígito "0" ao valor do dado.
  
  return dadoAjustado;    //retorna o valor do dado ajustado.
}
