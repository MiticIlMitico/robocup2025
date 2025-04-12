#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX, TX

Servo myservo;

static const int trigger = 3;
static const int echo = 4;
const int servoPin = 9;

long durata;
long distanza; 

float t = 0;
const float speedFactor = 0.35;

bool programmaAvviato = false;

void misurareDistanza() {
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  durata = pulseIn(echo, HIGH, 300000);

  if (durata > 0) {
    distanza = durata / 29 / 2;
    Serial.print("Distanza: ");
    Serial.print(distanza);
    Serial.println(" cm");
  } else {
    distanza = -1;
    Serial.println("Nessun segnale rilevato!");
  }
}

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600); // Deve combaciare col Mega

  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(trigger, LOW);
  delay(50);

  myservo.attach(servoPin);
  myservo.write(30);

  Serial.println("UNO pronto. In attesa del comando START via Bluetooth...");
}

void loop() {
  // 🎯 Ricezione comando via Bluetooth
  if (BTSerial.available()) {
    String comando = BTSerial.readStringUntil('\n');
    comando.trim();

    if (comando == "START") {
      Serial.println("Comando START ricevuto. Avvio programma...");
      programmaAvviato = true;
    }
  }

  // 👁️‍🗨️ Avvio programma solo dopo START
  if (programmaAvviato) {

      Serial.println("Ostacolo rilevato! Servo attivo.");
      
      float sinValue = (sin(t) + 1) / 2;
      int angle = 0 + sinValue * 60;
      myservo.write(angle);
      t += speedFactor;
      delay(20);

  }

  delay(100);
}
