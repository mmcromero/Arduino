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

//generico

//segundo = 0;
//minuto = 46;
//hora = 23;
//diaDaSemana = 4; // 1-segunda 2-terça 3-quarta 4-quinta 5-sexta 6-sabado
//diaDoMes = 16;
//mes = 8;
//ano = 18;

/* Função que realiza uma conversão de Decimal para Binário.
   Utilizada na gravação de dados no módulo.
*/
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}


void configuraModulo(String dados)
{
  /* Inicializando as variáveis abaixo com os dados de hora e data desejados.
     Lembrando que quando o dado for menor que 10, ou seja, tiver apenas um dígito
     não deve-se digitar o zero à esquerda. Exemplo: se deseja armazenar na variável hora
     o valor de 9horas, digite apenas o dígito 9, nunca 09.
  */
  // rtc-2214001171218
  // 12345678901234567
  String strHora = dados.substring(4,6);
  hora = strHora.toInt();

  String strMinuto = dados.substring(6,8);
  minuto = strMinuto.toInt();

  String strSegundo = dados.substring(8,10);
  segundo = strSegundo.toInt();

  String strDiaDaSemana = dados.substring(10,11);
  diaDaSemana = strDiaDaSemana.toInt();

  String strDiaDoMes = dados.substring(11,13);
  diaDoMes = strDiaDoMes.toInt();

  String strMes = dados.substring(13,15);
  mes = strMes.toInt();

  String strAno = dados.substring(15,17);
  ano = strAno.toInt();


  Serial.println(hora);
  Serial.println(minuto);
  Serial.println(segundo);
  Serial.println(diaDaSemana); // 1-segunda 2-terça 3-quarta 4-quinta 5-sexta 6-sabado
  Serial.println(diaDoMes);
  Serial.println(mes);
  Serial.println(ano);

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






void controleBrilho(String tipo){
  Serial.print("brilho atual: ");
  Serial.println(tipo);
  if(tipo == "+"){
      if(brilhoLed < 15){
        brilhoLed = brilhoLed + 1;  
      }  
      Serial.println(brilhoLed);
   }else if(tipo == "-"){
      if(brilhoLed >= 1){
        brilhoLed = brilhoLed - 1;
      }
      Serial.println(brilhoLed);
   }else{
     Serial.print("Caiu no else: "); 
     Serial.println(tipo);
   }

}
