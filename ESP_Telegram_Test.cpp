#include "CTBot.h"
#include <ESP8266WiFi.h>
CTBot myBot;

//Definicoes da rede wifi e conexao
String ssid  = "nome da rede"; //Nome da sua rede wifi
String pass  = "senha da rede"; //Senha da sua rede wifi
String token = "72585497**************************7H4"; //Coloque seu token aqui

//Pinos dos leds
uint8_t led1 = D4;
uint8_t led2 = D3;

void setup()
{
  Serial.begin(115200);
  delay (1000);
  Serial.println("Inicializando bot Telegram...");

  Serial.print("Conectando-se à rede: ");
  Serial.println(ssid);
  WiFi.begin(ssid,pass);

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tentativas++;
    if (tentativas > 40) { // timeout de 20 segundos
      Serial.println("\nFalha ao conectar-se ao Wi-Fi");
      return;
    }
  }
  Serial.println("\nWi-Fi conectado com sucesso!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());  

  //Define o token
  myBot.setTelegramToken(token);

  //Verifica a conexao
  if (myBot.testConnection())
    Serial.println("\nConexao Ok!");
  else
    Serial.println("\nFalha na conexao!");

  //Define os pinos dos leds como saida e apaga os leds
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
}

void loop()
{
  //Variavel que armazena a mensagem recebida
  TBMessage msg;

  //Verifica se chegou alguma mensagem
  if (myBot.getNewMessage(msg))
  {
    //Verifica se foi recebia a mensagem "Liga1"
    if (msg.text.equalsIgnoreCase("LIGA1"))
    {
      //Liga o led 1
      digitalWrite(led1, LOW);
      //Retorna uma pensagem informando que o led foi aceso
      myBot.sendMessage(msg.sender.id, "LED 1 Aceso!");
    }
    else if (msg.text.equalsIgnoreCase("DESLIGA1"))
    {
      digitalWrite(led1, HIGH);
      myBot.sendMessage(msg.sender.id, "LED 1 Apagado!");
    }
    else if (msg.text.equalsIgnoreCase("LIGA2"))
    {
      digitalWrite(led2, LOW);
      myBot.sendMessage(msg.sender.id, "LED 2 Aceso!");
    }
    else if (msg.text.equalsIgnoreCase("DESLIGA2"))
    {
      digitalWrite(led2, HIGH);
      myBot.sendMessage(msg.sender.id, "LED 2 Apagado!");
    }
    else
    {
      //Caso receba qualquer outro comando, envia uma mensagem generica/informativa
      String reply;
      reply = (String)"Olá " + msg.sender.username + (String)"! Estou a disposição!";
      myBot.sendMessage(msg.sender.id, reply);
    }
  }
  delay(500);
}
