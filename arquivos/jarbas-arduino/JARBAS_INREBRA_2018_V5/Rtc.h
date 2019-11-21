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


String ultimoSegundo;
String ultimoMinuto;

void verificaRtc()
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

  ajustaHora += ajustaZero(hora);
  ajustaMinuto += ajustaZero(minuto);
  ajustaSegundo += ajustaZero(segundo);
  ajustaDiaDoMes += ajustaZero(diaDoMes);
  ajustaMes += ajustaZero(mes);
  
  /*
  if(ultimoMinuto != ajustaMinuto ){

    Serial.print("Agora sao: "); 
    Serial.print(ajustaHora);
    Serial.print(":");
    Serial.print(ajustaMinuto);
    Serial.print(":");
    Serial.println(ajustaSegundo);
    Serial.print("Dia da semana: ");
    switch(diaDaSemana){
      case 0:Serial.println("Domingo"); break; //se a variável diaDaSemana for zero imprima na tela "Domingo"    
      case 1:Serial.println("Segunda-feira"); break; //idem ao anterior
      case 2:Serial.println("Terca-feira"); break;   //idem
      case 3:Serial.println("Quarta-feira"); break;  //idem
      case 4:Serial.println("Quinta-feira"); break;  //idem
      case 5:Serial.println("Sexta-feira"); break;   //idem
      case 6:Serial.println("Sabado"); break;        //idem
    }

    //Imprimindo os dois dígitos das datas separadas por uma barra.
    Serial.print("Data: ");
    Serial.print(ajustaDiaDoMes);
    Serial.print("/");
    Serial.print(ajustaMes);
    Serial.print("/");
    Serial.println(ano);
    Serial.println(); //salta uma linha.

  }
  */




  if(ajustaHora == "06" && ajustaMinuto == "00" &&  ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("6:00");
    sendIr("5","E", "16203967");
    FLAGTomadaSala = LOW;
    setPin(PIN_TOMADA_SALA, FLAGTomadaSala);
  } // desliga escada
  if(ajustaHora == "06" && ajustaMinuto == "15" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som2(); Serial.println("6:15");}
  if(ajustaHora == "06" && ajustaMinuto == "30" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som3(); Serial.println("6:30");}
  if(ajustaHora == "06" && ajustaMinuto == "45" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som4(); Serial.println("6:45");}
  
  if(ajustaHora == "07" && ajustaMinuto == "00" &&  ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("7:00");}
  if(ajustaHora == "07" && ajustaMinuto == "15" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som2(); Serial.println("7:15");}
  if(ajustaHora == "07" && ajustaMinuto == "30" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som3(); Serial.println("7:30");}
  if(ajustaHora == "07" && ajustaMinuto == "45" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som4(); Serial.println("7:45");}
  
  if(ajustaHora == "08" && ajustaMinuto == "00" &&  ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("8:00");}
  if(ajustaHora == "08" && ajustaMinuto == "15" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som2(); Serial.println("8:15");}
  if(ajustaHora == "08" && ajustaMinuto == "30" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som3(); Serial.println("8:30");}
  if(ajustaHora == "08" && ajustaMinuto == "45" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som4(); Serial.println("8:45");}

  if(ajustaHora == "09" && ajustaMinuto == "00" &&  ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("9:00");}
  if(ajustaHora == "09" && ajustaMinuto == "15" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som2(); Serial.println("9:15");}
  if(ajustaHora == "09" && ajustaMinuto == "30" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som3(); Serial.println("9:30");}
  if(ajustaHora == "09" && ajustaMinuto == "45" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som4(); Serial.println("9:45");}

  if(ajustaHora == "10" && ajustaMinuto == "00" &&  ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("10:00");}
  if(ajustaHora == "10" && ajustaMinuto == "15" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som2(); Serial.println("10:15");}
  if(ajustaHora == "10" && ajustaMinuto == "30" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som3(); Serial.println("10:30");}
  if(ajustaHora == "10" && ajustaMinuto == "45" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som4(); Serial.println("10:45");}

  if(ajustaHora == "11" && ajustaMinuto == "00" &&  ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("11:00");}
  if(ajustaHora == "11" && ajustaMinuto == "15" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som2(); Serial.println("11:15");}
  if(ajustaHora == "11" && ajustaMinuto == "30" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som3(); Serial.println("11:30");}
  if(ajustaHora == "11" && ajustaMinuto == "45" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som4(); Serial.println("11:45");}

  if(ajustaHora == "12" && ajustaMinuto == "00" &&  ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("12:00");}
  if(ajustaHora == "12" && ajustaMinuto == "15" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som2(); Serial.println("12:15");}
  if(ajustaHora == "12" && ajustaMinuto == "30" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som3(); Serial.println("12:30");}
  if(ajustaHora == "12" && ajustaMinuto == "45" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som4(); Serial.println("12:45");}

  if(ajustaHora == "13" && ajustaMinuto == "00" &&  ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("13:00");}
  if(ajustaHora == "13" && ajustaMinuto == "15" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som2(); Serial.println("13:15");}
  if(ajustaHora == "13" && ajustaMinuto == "30" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som3(); Serial.println("13:30");}
  if(ajustaHora == "13" && ajustaMinuto == "45" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som4(); Serial.println("13:45");}

  if(ajustaHora == "14" && ajustaMinuto == "00" &&  ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("14:00");}
  if(ajustaHora == "14" && ajustaMinuto == "15" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som2(); Serial.println("14:15");}
  if(ajustaHora == "14" && ajustaMinuto == "30" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som3(); Serial.println("14:30");}
  if(ajustaHora == "14" && ajustaMinuto == "45" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som4(); Serial.println("14:45");}

  if(ajustaHora == "15" && ajustaMinuto == "00" &&  ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("15:00");}
  if(ajustaHora == "15" && ajustaMinuto == "15" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som2(); Serial.println("15:15");}
  if(ajustaHora == "15" && ajustaMinuto == "30" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som3(); Serial.println("15:30");}
  if(ajustaHora == "15" && ajustaMinuto == "45" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som4(); Serial.println("15:45");}  

  if(ajustaHora == "16" && ajustaMinuto == "00" &&  ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("16:00");}
  if(ajustaHora == "16" && ajustaMinuto == "15" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som2(); Serial.println("16:15");}
  if(ajustaHora == "16" && ajustaMinuto == "30" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som3(); Serial.println("16:30");}
  if(ajustaHora == "16" && ajustaMinuto == "45" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som4(); Serial.println("16:45");}
  
  if(ajustaHora == "17" && ajustaMinuto == "00" &&  ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("17:00");}
  if(ajustaHora == "17" && ajustaMinuto == "15" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som2(); Serial.println("17:15");}
  if(ajustaHora == "17" && ajustaMinuto == "30" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som3(); Serial.println("17:30");}
  if(ajustaHora == "17" && ajustaMinuto == "45" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som4(); Serial.println("17:45");}
  
  if(ajustaHora == "18" && ajustaMinuto == "00" &&  ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("18:00");}
  if(ajustaHora == "18" && ajustaMinuto == "15" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som2(); Serial.println("18:15");}
  if(ajustaHora == "18" && ajustaMinuto == "30" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som3(); Serial.println("18:30");}
  if(ajustaHora == "18" && ajustaMinuto == "45" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som4(); Serial.println("18:45");}
  
  if(ajustaHora == "19" && ajustaMinuto == "00" &&  ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("19:00");}
  if(ajustaHora == "19" && ajustaMinuto == "15" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som2(); Serial.println("19:15");}
  if(ajustaHora == "19" && ajustaMinuto == "30" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som3(); Serial.println("19:30");}
  if(ajustaHora == "19" && ajustaMinuto == "45" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som4(); Serial.println("19:45");}
  
  if(ajustaHora == "20" && ajustaMinuto == "00" &&  ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("20:00");}
  if(ajustaHora == "20" && ajustaMinuto == "15" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som2(); Serial.println("20:15");}
  
  if(ajustaHora == "20" && ajustaMinuto == "30" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som3(); Serial.println("20:30");
    sendIr("5","E", "16236607");
    FLAGTomadaSala = HIGH;
    setPin(PIN_TOMADA_SALA, FLAGTomadaSala);
  } // liga escada
  if(ajustaHora == "20" && ajustaMinuto == "45" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som4(); Serial.println("20:45");}
  
  if(ajustaHora == "21" && ajustaMinuto == "00" &&  ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("21:00");}
  if(ajustaHora == "21" && ajustaMinuto == "15" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som2(); Serial.println("21:15");}
  if(ajustaHora == "21" && ajustaMinuto == "30" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som3(); Serial.println("21:30");}
  if(ajustaHora == "21" && ajustaMinuto == "45" && ajustaSegundo== "0" && ultimoSegundo != ajustaSegundo ){som4(); Serial.println("21:45");}
  
  if(ajustaHora == "22" && ajustaMinuto == "00" &&  ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("22:00");}
  if(ajustaHora == "22" && ajustaMinuto == "15" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som2(); Serial.println("22:15");}
  if(ajustaHora == "22" && ajustaMinuto == "30" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som3(); Serial.println("22:30");}
  if(ajustaHora == "22" && ajustaMinuto == "45" && ajustaSegundo== "00" && ultimoSegundo != ajustaSegundo ){som4(); Serial.println("22:45");}
 
  if(ajustaHora == "23" && ajustaMinuto == "0" &&  ajustaSegundo== "0" && ultimoSegundo != ajustaSegundo ){som1(); Serial.println("23:00");}
  
  ultimoSegundo = ajustaSegundo;
  ultimoMinuto = ajustaMinuto;



}
