
//TEST CODE 

#include <SoftwareSerial.h>
#define trigPin 13
#define echoPin 15

long duration, cm, inches;

void setup() {

  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  delay(5000);
  Serial.println("Modo patrulha iniciado!");
}

void loop()
{

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
  Serial.println("Intruso Detectado!");
  Serial.println("Enviando notificação por mensagem...");
  delay(5000);
  Serial.println("Modo patrulha iniciado...");
  }
  //end if statement

}//ends loop
