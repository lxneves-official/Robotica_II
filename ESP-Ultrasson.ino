#include <ESP8266WiFi.h> 
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Definindo as portas utilizadas pelo sensor

#define trigPin D1
#define echoPin D2

// Insira sua rede e sua senha

const char* nome = "nome da rede";
const char* senha = "senha";

ESP8266WebServer server(80);

// Declaração das variaveis utilizadas no código

int wifiStatus;
String textoHTML;
long duracao;
float distancia, aux;

// Envia o texto para o server

void handleRoot() {
  textoHTML = "<meta http-equiv= refresh content= 3>";
  textoHTML += "Distancia: ";
  textoHTML += distancia;
  textoHTML += "<br> Duracao: ";
  textoHTML += duracao;
  server.send(200, "text/html", textoHTML);
}

void handleNotFound(){
  String message = "Arquivo não foi encontrado \n\n";
  message += "URI: ";
  message += server.uri();
  message += "\n Método: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\n Argumentos: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i> server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void){

  Serial.begin(115200);
  delay(200);

  Serial.print("Iniciando a conexão da rede! ");
  Serial.println(nome);
  WiFi.begin(nome, senha);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 

  
  // Pinos de comunicação que receberam os dados
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  
    wifiStatus = WiFi.status(); 

  if( wifiStatus == WL_CONNECTED){
    Serial.println("3ºSTEAM o melhor.... O nodeMCU esta conectado! ");
    Serial.print("O IP utilizado na conexão é: ");
    Serial.println(WiFi.localIP());
    Serial.println("");
    }
    else {
      Serial.println("Ahhhhhhhhhh... O nodeMCU não esta conectado!");
      }
  delay(500);

  // Enviando o texto HTML para o server

// Primeira opção
  server.on("/", handleRoot);
  

// Segunda opção
  server.on("/inline", [](){
    server.send(200, "text/plain", "2 opção de envio");
  });

// Caso ocorra um erro, (tratativas do erro)

  server.onNotFound(handleNotFound);

  //Inicia o server

  server.begin();
  Serial.println("HTTP server iniciado");
}

void loop(void){
  
  // Deixa o server em loop, assim se houver alguma mudança nas informações, ele atualiza
  
  server.handleClient();
  
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duracao = pulseIn(echoPin, HIGH ,1000000);
  aux = duracao;
  distancia = (aux*340/10000)/2; // distancia em centimetros
  /*Serial.print("Distancia: ");
  Serial.println(distancia);

  Serial.print("Duracao: "); 
  Serial.println(duracao);*/

  delay(500);
}
