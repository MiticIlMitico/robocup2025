#include <Servo.h>

Servo myservo;  

static int trigger = 3;
static int echo = 4;

long durata;
long distanza; 

const int servoPin = 9;

float t = 0;                                                                                                                                                                                                                  
const float speedFactor = 0.35; 

bool programmaAvviato = false; 

void misurareDistanza() {
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10); // Impulso esatto di 10μs
  digitalWrite(trigger, LOW);

  durata = pulseIn(echo, HIGH, 300000); // Timeout aumentato a 300ms

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

  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(trigger, LOW);
  delay(50); 

  myservo.attach(servoPin);
  myservo.write(30);
}

void loop() {
  if (!programmaAvviato) {
    misurareDistanza();
    Serial.print("Distanza rilevata: ");
    Serial.println(distanza); // Debug

    if (distanza <= 50 && distanza > 0) {
      Serial.println("Ostacolo rilevato! Avvio il movimento del servo.");
      programmaAvviato = true;
    }
  }

  if (programmaAvviato) {
    float sinValue = (sin(t) + 1) / 2;
    int angle = 0 + sinValue * 60;
    myservo.write(angle);
    t += speedFactor;
    delay(20);
  }

  delay(100); // Ritardo tra le letture per stabilità
}
