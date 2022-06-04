// ===================
// Incluir bibliotecas
// ===================
#include <ESP8266WiFi.h> //inclui suporte ao NodeMCU
#include <PubSubClient.h> //inclui suporte ao MQTT no HiveMQ Cloud
#include <SoftwareSerial.h>

// =======================
// Definicoes e constantes
// =======================

int ledPin = 12;
int trigPin = 13;
int echoPin = 15;

// Configuracoes da rede WiFi
const char* ssid = "Clash"; //SSID da rede WiFi
const char* password = "32047568#"; //senha da rede WiFi

const char* mqtt_server = "broker.mqtt-dashboard.com"; //URL do broker MQTT
const int mqtt_server_port = 1883; //porta do broker MQTT

// Variaveis globais e objetos
#define MSG_BUFFER_SIZE (500) //define MSG_BUFFER_SIZE como 500
WiFiClient client; //cria o objeto client
PubSubClient mqtt_client(client); //cria o objeto mqtt_client
long lastMsg = 12;
long duration, cm, inches;

String clientID = "ESP8266Client-"; //identificacao do cliente

String topicoPrefixo = "MACK32047568"; //para o prefixo do topico, utilizar MACK seguido do TIA
String topicoTodos = topicoPrefixo + "/#"; //para retornar todos os topicos
String topico_0 = topicoPrefixo + "/hello"; //topico para teste
String mensagem_0 = "NodeMCU Connected"; //mensagem para o topico 0
String topico_1 = topicoPrefixo + "/sensor1"; //topico para o sensor 1
String mensagem_1 = "Intruso detectadooooo"; //mensagem para o topico 1
String topico_2 = topicoPrefixo + "/atuador1"; //topico para o atuador 1
String mensagem_2 = ""; //mensagem para o topico 2
String mensagemTemp = ""; //armazena temporariamente as mensagens recebidas via MQTT

int estadoBotao = 12; //variavel que le o estado atual do botao conectado em D0
int ultimoEstadoBotao = 12; //variavel que guarda o ultimo estado do botao conectado em D0
int controlaEstadoBotao = 12; //variavel que controla o estado do botao conectado em D0

// ==============================
// Funcoes definidas pelo usuario
// ==============================

// Funcao para configurar a conexao WiFi
void setup_wifi() {
  //WiFi.mode(WIFI_STA); //ativa o modo de estacao
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}


// Funcao para conectar no broker MQTT
void reconnect() {
  // Loop until we’re reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection…");

	// Create a random client ID
    randomSeed(micros()); //inicializa a semente do gerador de numeros aleatorios
    clientID += String(random(0xffff), HEX);

	// Attempt to connect
    if (mqtt_client.connect(clientID.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqtt_client.publish(topico_0.c_str(), mensagem_0.c_str());
      // ... and resubscribe
      mqtt_client.subscribe(topicoTodos.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}	

// =======
// setup()
// =======

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  delay(5000);
  pinMode(ledPin,OUTPUT);
  Serial.println("Modo patrulha iniciado!");
  Serial.begin(9600); //inicializa a saida serial
  setup_wifi();

  mqtt_client.setServer(mqtt_server, mqtt_server_port); //configura a conexao com o broker MQTT
  mqtt_client.setCallback(callback); //configura o recebimento de mensagens
}

// ======
// loop()
// ======

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  inches = (duration / 2) / 74;
  Serial.println(inches);
  if (inches < 20 || inches > 500) {
  digitalWrite(ledPin, HIGH);
  Serial.println("Intruso Detectado!");
  Serial.println("Enviando notificação por mensagem...");
  
  if (!mqtt_client.connected()) {
    reconnect();
  }
  mqtt_client.loop(); //processa as mensagens e mantem a conexao com o broker MQTT

  delay(50);

  delay(5000);
  digitalWrite(ledPin, LOW);
  Serial.println("Modo patrulha iniciado...");
}
}
