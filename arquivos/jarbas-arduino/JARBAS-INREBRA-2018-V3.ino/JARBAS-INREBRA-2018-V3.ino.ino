String jarbasVersion = "Jarbas-INREBRA-2018-V3";

/* ------------------------------------------------------------------ ETHERNET ------------------------------------------------------------------ */
//ethernet
#include <Ethernet.h>
EthernetServer server(8087); // Cria o servidor na porta 8080

byte myMac[]={0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};  //Mac PRODUÇÃO
byte myIp[]  = { 192, 168, 0, 7 };   //define IP da central automação 1

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
String strLocal = "N"; 

String strTamanho = ""; 
int intTamanho; 

bool debugControle = true;
//----------------------------

//generico
void sendIr(String strRepeticao, String strLocal, String strValue)
{   

  int intRepeticao = strRepeticao.toInt();
  if(intRepeticao < 11){
    for (int i = 0; i < intRepeticao; i++) {
      unsigned long code = strValue.toInt();
      irsend.sendNEC(code, 32);
      delay(40);
    }
    if(debugControle == true){
      Serial.print("codigo:");
      Serial.print(strValue);
      Serial.print(" - repeticao:");
      Serial.print(strRepeticao);
      Serial.print(" - local:");
      Serial.println(strLocal);
    }
  }else{
    Serial.println("muitas repetições melhor parar");
  }
  irrecv.enableIRIn(); // Re-enable receiver
}


//----------------------------



void setup(){ 
  Serial.begin(9600);
  Ethernet.begin(myMac, myIp, gateway, subnet);//inicializa eternet
  irrecv.enableIRIn(); // Inicializa o receptor IR 
  Serial.println(jarbasVersion);
  Serial.println("teste");
}

void loop(){
  
  /////// RECEBE SERIAL //-----------------------------------------------------------------------------------------------------------
  if(Serial.available() > 0) {
    while(Serial.available() > 0){
      cmd += char(Serial.read());
      delay(10);
    }
    Serial.println(cmd); 
  }
  if (cmd.length() >0) { 
    // switch debug
    if(cmd == "debug-on"){debugControle = true;}
    if(cmd == "debug-off"){debugControle = false;}
    cmd = "";
  }
  //-----------------------------------------------------------------------------------------------------------------------------

  /////// RECEBE IR //-----------------------------------------------------------------------------------------------------------
  if (irrecv.decode(&results)){  
    Serial.print("Valor lido 1 : ");  
    Serial.println(results.value, HEX);  
    //armazenavalor = (results.value);   
    if(results.value !=0xffffffff){
      armazenavalor = (results.value);
    }
    
    //volume
    if (armazenavalor == 0x20DF40BF) {   
      if(results.value !=0xffffffff){
        sendIr("1","S", "1587632295");// #1volMaisReciverS
      }else{
        sendIr("10","S", "1587632295");// #1volMenosReciverS
      }
    }
      
    if (armazenavalor == 0x20DFC03F) { 
      if(results.value !=0xffffffff){
        sendIr("1","S", "1587664935");// #1volMenosReciverS
      }else{
        sendIr("10","S", "1587664935");// #1volMenosReciverS
      }
    }
    irrecv.resume(); //Le o próximo valor  
  } 



  //-----------------------------------------------------------------------------------------------------------------------------



  /////// RECEBE ETHERNET //-----------------------------------------------------------------------------------------------------------
  EthernetClient client = server.available();
  if (!client) {
    return;
  }
  // Wait until the client sends some data
  Serial.println("Novo client");
  while(!client.available()){
    delay(1);
  }
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  
  if(debugControle == true){
    Serial.print("Request: ");
    Serial.println(request);
  }
  
  if (request.indexOf("/ir?") != -1)  {
    strRepeticao = request.substring(9,10);
    intTamanho = request.length();
    strLocal = request.substring(intTamanho -9, intTamanho -10);
    comando = request.substring(10,intTamanho -10);
    sendIr(strRepeticao,strLocal, comando);
  }
    
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Access-Control-Allow-Origin: *");
  client.println(""); //  do not forget this one
  //continua....
  //client.println("arduino - conectado!"); //  do not forget this one
  //client.println(jarbasVersion); //  do not forget this one
  client.print(montaString());

  delay(1);
  client.stop();
  Serial.println("Client disconectado");
  Serial.println("");
    
    
  
  //-----------------------------------------------------------------------------------------------------------------------------
  
  
}// FIM DO LOOP  



String montaString()
{
    String string1 ="";
    string1.concat("dados({");
   //tomadaSala
    string1.concat("Status : '"); // valor / estado
    string1.concat("OnLine");
    //tomadaArthur
    string1.concat("', Versao : '"); // valor / estado
    string1.concat(jarbasVersion);
    //tomadaCasal
    //string1.concat("', IP : '"); // valor / estado
    //string1.concat(myIp[]);
    string1.concat("'})");
    return string1;

}

