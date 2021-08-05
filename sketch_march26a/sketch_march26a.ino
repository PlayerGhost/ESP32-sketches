#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "ESP";
const char* password =  "ghostesp";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttUser = "";
const char* mqttPassword = "";

WiFiClient espClient;
PubSubClient client(espClient);

int contador = 1;
char mensagem[30];
int ledPin = 2;

void setup()
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Iniciando conexao com a rede WiFi..");
  }

  Serial.println("Conectado na rede WiFi!");
  client.setCallback(callback);
}

void loop()
{
  //Conecta com o broker, caso não esteja conectado.
  if(!client.connected()){
  reconectabroker();
  }
  sprintf(mensagem, "MQTT ESP32 - Mensagem no. %d", contador);
  Serial.print("Mensagem enviada: ");
  Serial.println(mensagem);

  //Envia a mensagem ao broker
  client.publish("TesteJoel", mensagem);
  Serial.println("Mensagem enviada com sucesso...");
  
  //Incrementa o contador
  contador++;
  
  //Aguarda 5 segundos
  delay(5000);

  //Para checar a rotina de callback
  client.loop();
}

void reconectabroker()
{
  //Conexao ao broker MQTT
  client.setServer(mqttServer, mqttPort);
  while (!client.connected())
  {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("ESPdoJoel")) //Se tivesse um usuario e senha no broker aqui teria que ser client.connect("ESPdoJoel",mqttUser,mqttPassword)
    {
      Serial.println("Conectado ao broker!");
      client.subscribe("ExpJoel/Led1");
    }
    else
    {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Chegou mensagem no topico: ");
  Serial.print(topic);
  Serial.print(". Mensagem: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "ExpJoel/Led1") {
    Serial.print("Trocando o estado do LED para: ");
    if(messageTemp == "on"){
      Serial.println("Ligado");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("Desligado");
      digitalWrite(ledPin, LOW);
    }
  }
}
