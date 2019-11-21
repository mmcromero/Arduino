const char jarbasVersion = " Jarbas INREBRA - limpo";

//Variavel de ambiente determina se estamos em teste ou produção
// TESTE = 0
// PRODUÇÃO = 1

int ambiente = 0;


/* ------------------------------------------------------------------ ETHERNET ------------------------------------------------------------------ */
//ethernet
#include <Ethernet.h>
EthernetServer server(8080); // Cria o servidor na porta 8080
//String string1 ="";

// TESTE
  //byte myMac[]={0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x02 };  //Mac TESTE
  //byte myIp[]  = { 192, 168, 0, 66 };   //define IP da central automação 2
//PRODUÇÃO
  byte myMac[]={0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};  //Mac PRODUÇÃO
  byte myIp[]  = { 192, 168, 0, 46 };   //define IP da central automação 1

byte gateway[] = { 192,168,0, 1 }; // ip do roteador
byte subnet[] = { 255, 255, 255, 0 };
/* ------------------------------------------------------------------ ETHERNET ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ IR ------------------------------------------------------------------ */
/// IR
#include <IRremote.h>  
IRsend irsend; // IR SALA

/////// receptor IR sala
int RECV_PIN = 45; 
IRrecv irrecv(RECV_PIN);

decode_results results; 
float armazenavalor; 
//float armazenavalorMemoria; 
/* ------------------------------------------------------------------ IR ------------------------------------------------------------------ */

String cmd = "";
String comando = "";
String strTipo = "";
String strRepeticao = "";
String cmdEthernet = "";
String cmdWire = "";
String strLocal = "N"; 

String strTamanho = ""; 
int intTamanho; 

//----------------------------

int NEGATIVOIRSALA = 42;
int NEGATIVOIRESCADA = 2 ;
int NEGATIVOIRCRIANCAS = 43;
int NEGATIVOIRMEIO = 44;



//generico
void sendIr(String strRepeticao, String strLocal, String strValue)
{   
  int intRepeticao = strRepeticao.toInt();
  
  if(strLocal != "S"){digitalWrite(NEGATIVOIRSALA, HIGH); }
  if(strLocal != "E"){digitalWrite(NEGATIVOIRESCADA, HIGH); }
  if(strLocal != "C"){digitalWrite(NEGATIVOIRCRIANCAS, HIGH); }
  if(strLocal != "M"){digitalWrite(NEGATIVOIRMEIO, HIGH); }
    
    for (int i = 0; i < intRepeticao; i++) {
      
      //LED repetição 5
      
      if(strValue == "ledDesliga"){ irsend.sendNEC(0xF740BF, 32); }//ledDesliga
      if(strValue == "ledLiga"){ irsend.sendNEC(0xF7C03F, 32); }//ledLiga
      if(strValue == "ledSobe"){ irsend.sendNEC(0xF700FF, 32); }//ledSobe
      if(strValue == "ledDesce"){ irsend.sendNEC(0xF7807F, 32); }//ledDesce
      if(strValue == "ledFlash"){ irsend.sendNEC(0xF7D02F, 32); }//ledFlash
      if(strValue == "ledStrobe"){ irsend.sendNEC(0xF7F00F, 32); }//ledStrobe
      if(strValue == "ledFade"){ irsend.sendNEC(0xF7C837, 32); }//ledFade
      if(strValue == "ledSmooth"){ irsend.sendNEC(0xF7E817, 32); }//ledSmooth
      if(strValue == "ledWhite"){ irsend.sendNEC(0xF7E01F, 32); }//ledWhite
      if(strValue == "ledRed"){ irsend.sendNEC(0xF720DF, 32); }//ledRed
      if(strValue == "ledRed1"){ irsend.sendNEC(0xF710EF, 32); }//ledRed1
      if(strValue == "ledRed2"){ irsend.sendNEC(0xF730CF, 32); }//ledRed2
      if(strValue == "ledRed3"){ irsend.sendNEC(0xF708F7, 32); }//ledRed3
      if(strValue == "ledRed4"){ irsend.sendNEC(0xF728D7, 32); }//ledRed4
      if(strValue == "ledGreen"){ irsend.sendNEC(0xF7A05F, 32); }//ledGreen
      if(strValue == "ledGreen1"){ irsend.sendNEC(0xF7906F, 32); }//ledGreen1
      if(strValue == "ledGreen2"){ irsend.sendNEC(0xF7B04F, 32); }//ledGreen2
      if(strValue == "ledGreen3"){ irsend.sendNEC(0xF78877, 32); }//ledGreen3
      if(strValue == "ledGreen4"){ irsend.sendNEC(0xF7A857, 32); }//ledGreen4
      if(strValue == "ledBlue"){ irsend.sendNEC(0xF7609F, 32); }//ledBlue
      if(strValue == "ledBlue1"){ irsend.sendNEC(0xF750AF, 32); }//ledBlue1
      if(strValue == "ledBlue2"){ irsend.sendNEC(0xF7708F, 32); }//ledBlue2
      if(strValue == "ledBlue3"){ irsend.sendNEC(0xF748B7, 32); }//ledBlue3
      if(strValue == "ledBlue4"){ irsend.sendNEC(0xF76897, 32); }//ledBlue4
      
      
      //TV repetição 1
      if(strValue == "ligaTv"){ irsend.sendNEC(0x20DF10EF, 32); } //ligaTv
      if(strValue == "inputTv"){ irsend.sendNEC(0x20DFD02F, 32); } //inputTv
      if(strValue == "cimaTv"){ irsend.sendNEC(0x20DF02FD, 32); } //cimaTv
      if(strValue == "baixoTv"){ irsend.sendNEC(0x20DF827D, 32); } //baixoTv
      if(strValue == "esquerdaTv"){ irsend.sendNEC(0x20DFE01F, 32); } //esquerdaTv
      if(strValue == "direitaTv"){ irsend.sendNEC(0x20DF609F, 32); } //direitaTv
      if(strValue == "okTv"){ irsend.sendNEC(0x20DF22DD, 32); } //okTv
      if(strValue == "homeTv"){ irsend.sendNEC(0x20DF3EC1, 32); } //homeTv
      if(strValue == "exitTv"){ irsend.sendNEC(0x20DFDA25, 32); } //exitTv
      if(strValue == "qmenuTvOld"){ irsend.sendNEC(0x20DFA25D, 32); } //qmenuTv
      if(strValue == "qmenuTv"){ irsend.sendNEC(0x20DFC23D, 32); } //qmenuTv
      if(strValue == "backTv"){ irsend.sendNEC(0x20DF14EB, 32); } //backTv
      //if(strValue == "tv3d"){ irsend.sendNEC(0x20DFD02F, 32); } //tv3d
      if(strValue == "volumeTvMenos"){ irsend.sendNEC(0x20DFC03F, 32); } //volumeTvMenos
      if(strValue == "volumeTvMais"){ irsend.sendNEC(0x20DF40BF, 32); } //volumeTvMais
      if(strValue == "canalTvMenos"){ irsend.sendNEC(0x20DF807F, 32); } //canalTvMenos
      if(strValue == "canalTvMais"){ irsend.sendNEC(0x20DF00FF, 32); } //canalTvMais
      //if(strValue == "playTv"){ irsend.sendNEC(0x20DF0DF2, 32); } //playTv
      //if(strValue == "pauseTv"){ irsend.sendNEC(0x20DFD02F, 32); } //pauseTv
      //if(strValue == "stopTv"){ irsend.sendNEC(0x20DFD02F, 32); } //stopTv
      //if(strValue == "rrTv"){ irsend.sendNEC(0x20DFD02F, 32); } //rrTv
      //if(strValue == "ffTv"){ irsend.sendNEC(0x20DFD02F, 32); } //ffTv
      //if(strValue == "ratioTv"){ irsend.sendNEC(0x20DFD02F, 32); } //ratioTv
      //if(strValue == "sleepTv"){ irsend.sendNEC(0x20DFD02F, 32); } //sleepTv
      //if(strValue == "pictureTv"){ irsend.sendNEC(0x20DFD02F, 32); } //pictureTv
      //if(strValue == "somTv"){ irsend.sendNEC(0x20DFD02F, 32); } //somTv

      // tv lg sala
      //q menu 20DFC23D

      // input 20DFD02F
      
      
      //CONTROLE NET repetição 1
      if(strValue == "ligaNet"){ irsend.sendNEC(0xE17A48B7, 32);} //ligaNet
      if(strValue == "infoNet"){ irsend.sendNEC(0xE17AC837, 32);} //infoNet
      if(strValue == "portalNet"){ irsend.sendNEC(0xE17A24DB, 32);} //portalNet
      if(strValue == "mozaicoNet"){ irsend.sendNEC(0xE17A847B, 32);} //mozaicoNet
      if(strValue == "volMaisNet"){ irsend.sendNEC(0xE17AB04F, 32);} //volMaisNet
      if(strValue == "volMenosNet"){ irsend.sendNEC(0xE17A708F, 32);} //volMenosNet
      if(strValue == "canalMaisNet"){ irsend.sendNEC(0xE17A08F7, 32);} //canalMaisNet
      if(strValue == "canalMenosNet"){ irsend.sendNEC(0xE17A58A7, 32);} //canalMenosNet
      if(strValue == "num1Net"){ irsend.sendNEC(0xE17A807F, 32);} //num1Net
      if(strValue == "num2Net"){ irsend.sendNEC(0xE17A40BF, 32);} //num2Net
      if(strValue == "num3Net"){ irsend.sendNEC(0xE17AC03F, 32);} //num3Net
      if(strValue == "num4Net"){ irsend.sendNEC(0xE17A20DF, 32);} //num4Net
      if(strValue == "num5Net"){ irsend.sendNEC(0xE17AA05F, 32);} //num5Net
      if(strValue == "num6Net"){ irsend.sendNEC(0xE17A609F, 32);} //num6Net
      if(strValue == "num7Net"){ irsend.sendNEC(0xE17AE01F, 32);} //num7Net
      if(strValue == "num8Net"){ irsend.sendNEC(0xE17A10EF, 32);} //num8Net
      if(strValue == "num9Net"){ irsend.sendNEC(0xE17A906F, 32);} //num9Net
      if(strValue == "num0Net"){ irsend.sendNEC(0xE17A00FF, 32);} //num0Net
      if(strValue == "cimaNet"){ irsend.sendNEC(0xE17AD02F, 32);} //cimaNet
      if(strValue == "baixoNet"){ irsend.sendNEC(0xE17A30CF, 32);} //baixoNet
      if(strValue == "esquerdaNet"){ irsend.sendNEC(0xE17AD827, 32);} //esquerdaNet
      if(strValue == "direitaNet"){ irsend.sendNEC(0xE17A38C7, 32);} //direitaNet
      if(strValue == "okNet"){ irsend.sendNEC(0xE17AA857, 32);} //okNet
      if(strValue == "muteNet"){ irsend.sendNEC(0xE17AF807, 32);} //muteNet
      if(strValue == "voltarNet"){ irsend.sendNEC(0xE17A8877, 32);} //voltarNet
      if(strValue == "favNet"){ irsend.sendNEC(0xE17AB847, 32);} //favNet
      if(strValue == "menuNet"){ irsend.sendNEC(0xE17AC43B, 32);} //menuNet
      if(strValue == "sairNet"){ irsend.sendNEC(0xE17A50AF, 32);} //sairNet
      if(strValue == "nowNet"){ irsend.sendNEC(0xE17A9C63, 32);} //nowNet
      if(strValue == "audioNet"){ irsend.sendNEC(0xE17AE817, 32);} //audioNet
      if(strValue == "agoraNet"){ irsend.sendNEC(0xE17A7887, 32);} //agoraNet
      if(strValue == "legendaNet"){ irsend.sendNEC(0xE17A18E7, 32);} //legendaNet
      if(strValue == "musicaNet"){ irsend.sendNEC(0xE17A04FB, 32);} //musicaNet
      if(strValue == "rrNet"){ irsend.sendNEC(0xE17A2CD3, 32);} //rrNet
      if(strValue == "playPauseNet"){ irsend.sendNEC(0xE17A6C93, 32);} //playPauseNet
      if(strValue == "stopNet"){ irsend.sendNEC(0xE17A4CB3, 32);} //stopNet
      if(strValue == "ffNet"){ irsend.sendNEC(0xE17AAC53, 32);} //ffNet
      if(strValue == "recNet"){ irsend.sendNEC(0xE17ACC33, 32);} //recNet
      if(strValue == "netTvNet"){ irsend.sendNEC(0xE17A28D7, 32);} //netTvNet
      
      //CONTROLE RECIVER repetição 1


      if(strValue == "onReciver"){ irsend.sendNEC(0x5EA1B847, 32); } //liga
      if(strValue == "offReciver"){ irsend.sendNEC(0x5EA17887, 32); } //desliga

      if(strValue == "dvdReciver"){ irsend.sendNEC(0x5EA1837C, 32); }  //input dvd (tv)
      if(strValue == "cdReciver"){ irsend.sendNEC(0x5EA1A857, 32); } //input cd (aux)
      if(strValue == "radioReciver"){ irsend.sendNEC(0x5EA16897, 32); } //input radio
      //if(strValue == "Reciver"){ irsend.sendNEC(0x, 32); } // stereo
      //if(strValue == "Reciver"){ irsend.sendNEC(0x, 32); } //
      //if(strValue == "Reciver"){ irsend.sendNEC(0x, 32); } //
      
      if(strValue == "volMenosReciver"){ irsend.sendNEC(0x5EA1D827, 32); } //volMenosSub
      if(strValue == "volMaisReciver"){ irsend.sendNEC(0x5EA158A7, 32); } //volMaisSub
      
      delay(40);
    }
    
  if(strLocal != "S"){digitalWrite(NEGATIVOIRSALA, LOW); }
  if(strLocal != "E"){digitalWrite(NEGATIVOIRESCADA, LOW); }
  if(strLocal != "C"){digitalWrite(NEGATIVOIRCRIANCAS, LOW); }
  if(strLocal != "M"){digitalWrite(NEGATIVOIRMEIO, LOW); } 
  

  Serial.print("ir generico: ");
  Serial.print(strValue);
  Serial.print("Local: ");
  Serial.print(strLocal);
  Serial.print("Repeticao: ");
  Serial.println(intRepeticao);
  
  irrecv.enableIRIn(); // Re-enable receiver
}


//----------------------------



void setup(){ 
  
  Serial.begin(9600);


  pinMode(NEGATIVOIRSALA, OUTPUT);
  pinMode(NEGATIVOIRESCADA, OUTPUT);
  pinMode(NEGATIVOIRCRIANCAS, OUTPUT);
  pinMode(NEGATIVOIRMEIO, OUTPUT);
  
  
  digitalWrite(NEGATIVOIRSALA, LOW);
  digitalWrite(NEGATIVOIRESCADA, LOW);
  digitalWrite(NEGATIVOIRCRIANCAS, LOW);
  digitalWrite(NEGATIVOIRMEIO, LOW);

  Ethernet.begin(myMac, myIp, gateway, subnet);

  if(ambiente == 0){ // TESTE
    //apresentaLuzes();
  }else if(ambiente == 1){ //PRODUÇÃO
    
  }


  //Rec IR
  irrecv.enableIRIn(); // Inicializa o receptor IR 


}


void loop(){


  
  /////// RECEBE IR //-----------------------------------------------------------------------------------------------------------
  if (irrecv.decode(&results))  
  {  
    Serial.print("Valor lido 1 : ");  
    Serial.println(results.value, HEX);  
    armazenavalor = (results.value);   
    
    if(results.value !=0xffffffff){
      armazenavalor = (results.value);
    }
    


    
    //volume

    if (armazenavalor == 0x20DF40BF) {   
      if(results.value !=0xffffffff){
        sendIr("1","S", "volMaisReciver");// #1volMaisReciverS
      }else{
        sendIr("10","S", "volMaisReciver");// #1volMenosReciverS
      }
    }
      
    if (armazenavalor == 0x20DFC03F) { 
      if(results.value !=0xffffffff){
        sendIr("1","S", "volMenosReciver");// #1volMenosReciverS
      }else{
        sendIr("10","S", "volMenosReciver");// #1volMenosReciverS
      }
    }

    
    irrecv.resume(); //Le o próximo valor  
  } 



  //-----------------------------------------------------------------------------------------------------------------------------



  /////// RECEBE ETHERNET //-----------------------------------------------------------------------------------------------------------
  EthernetClient client1 = server.available();
  if (client1) {
    boolean currentLineIsBlank = true;
    while (client1.connected()) {
      if (client1.available()) {
        char c = client1.read();
        if (c != ' ') {
          cmdEthernet += c;
          Serial.println(c);
        }else{
          Serial.println("espaço em branco");
        }
        

        if (c == '\n' && currentLineIsBlank) {
          
          client1.println("HTTP/1.1 200 OK");
          // IMPORTANTE, ISSO FAZ O ARDUINO RECEBER REQUISIÇÃO AJAX DE OUTRO SERVIDOR E NÃO APENAS LOCAL.
          client1.println("Content-Type: text/javascript");
          client1.println("Access-Control-Allow-Origin: *");
          client1.println();          
          //montaString();
          //client1.print(montaString());
          break;
          
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }

    if (cmdEthernet.indexOf("/IR") != -1)  {
          strTipo = cmdEthernet.substring(8,6);
          strRepeticao = cmdEthernet.substring(8,9);
          intTamanho = cmdEthernet.length();
          strLocal = cmdEthernet.substring(intTamanho -9, intTamanho -10);
          comando = cmdEthernet.substring(9,intTamanho -10);
      
          Serial.print("strTipo: ");
          Serial.println(strTipo);
          Serial.print("strRepeticao: ");
          Serial.println(strRepeticao);
          Serial.print("strLocal: ");
          Serial.println(strLocal);
          Serial.print(" ");
          Serial.print(" request:");
          Serial.println(cmdEthernet);
      
          //sendIr(strRepeticao,strLocal, comando);
          
    }
    
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client1.stop();
  }
  //-----------------------------------------------------------------------------------------------------------------------------
  
  /////// RECEBE SERIAL //-----------------------------------------------------------------------------------------------------------
  if(Serial.available() > 0) // serial pc
  {
    
    while(Serial.available() > 0)
    {
      cmd += char(Serial.read());
      delay(10);
    }
    Serial.println(cmd); 
    //refHora = millis();
  }
  
  if (cmd.length() >0 || cmdEthernet.length() >0) { 
    cmd += cmdEthernet;
    
    // lampadas
    // informa tempera
    //if(cmd == "t1"){dht11Externo();}
    //if(cmd == "t2"){dht11Sala();}
  
    //if(cmd == "relogio"){  mostraHora();}
    

    strTipo = cmd.substring(0,1);
    strRepeticao = cmd.substring(1,2);
    strLocal = cmd.substring(cmd.length() -1);
    comando = cmd.substring(2,cmd.length() -1);
    
    //comandos IR
    
    if(strTipo == "#"){
      sendIr(strRepeticao,strLocal, comando);
    }

    
    cmd = "";
    comando = "";
    cmdEthernet = "";
  }
  //-----------------------------------------------------------------------------------------------------------------------------


  // Executa 1 vez a cada 15 segundos
  //if(millis() - ref >= 15000){
    //dht11Externo();
    //dht11Sala();
    //ref = millis(); // Atualiza a referência
  //}

  
  //if(millis() - refHora >= 1000){
  //  verificaRtc();
  //  refHora = millis(); // Atualiza a referência
  //}
  
}// FIM DO LOOP  

///////////////////////////////// funções



//-----------------------------------------------------------------------------------------------------------------------------



