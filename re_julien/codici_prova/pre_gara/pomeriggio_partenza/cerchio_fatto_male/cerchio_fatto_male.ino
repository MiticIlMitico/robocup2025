#include <Servo.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Servo myservo;
SoftwareSerial maestroSerial(19, 18); // Bluetooth HC-05 collegato su pin 19 (RX) e 18 (TX)
Adafruit_BNO055 bno = Adafruit_BNO055(55);

const int trigger = 4;
const int echo = 3;

const int ruotaSX_indietro = 12;
const int ruotaSX_avanti = 13;
const int pinPotenzaSX = 11;
const int ruotaDX_indietro = 8;
const int ruotaDX_avanti = 9;
const int pinPotenzaDX = 7;

// Qui controlli la velocità globale
const int potenza = 145; // Più basso = più lento

float Kp = 2.5;
float Ki = 0.0;
float Kd = 0.5;
float integral = 0;
float previous_error = 0;

bool programmaAvviato = false;
bool bluetoothInviato = false;
unsigned long startTime = 0;

long durata;
long distanza;

// === SETUP ===

void setup() {
  Serial.begin(9600);
  maestroSerial.begin(9600); // Bluetooth

  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(trigger, LOW);
  delay(50);

  pinMode(ruotaSX_indietro, OUTPUT);
  pinMode(ruotaSX_avanti, OUTPUT);
  pinMode(pinPotenzaSX, OUTPUT);
  pinMode(ruotaDX_indietro, OUTPUT);
  pinMode(ruotaDX_avanti, OUTPUT);
  pinMode(pinPotenzaDX, OUTPUT);

  fermaMotori();

  if (!bno.begin()) {
    Serial.println("Errore nel rilevare il BNO055");
    while (1);
  }

  bno.setExtCrystalUse(true);
  Serial.println("BNO055 pronto.");
  myservo.write(30);
}

// === FUNZIONI DI MOVIMENTO ===

void fermaMotori() {
  digitalWrite(ruotaSX_indietro, LOW);
  digitalWrite(ruotaSX_avanti, LOW);
  analogWrite(pinPotenzaSX, 0);
  digitalWrite(ruotaDX_indietro, LOW);
  digitalWrite(ruotaDX_avanti, LOW);
  analogWrite(pinPotenzaDX, 0);
}

void dritto(int secondi) {
  digitalWrite(ruotaSX_indietro, LOW);
  digitalWrite(ruotaSX_avanti, HIGH);
  analogWrite(pinPotenzaSX, potenza);
  digitalWrite(ruotaDX_indietro, LOW);
  digitalWrite(ruotaDX_avanti, HIGH);
  analogWrite(pinPotenzaDX, potenza);
  delay(secondi * 1000);
  fermaMotori();
}

void indietro(int secondi) {
  digitalWrite(ruotaSX_indietro, HIGH);
  digitalWrite(ruotaSX_avanti, LOW);
  analogWrite(pinPotenzaSX, potenza);
  digitalWrite(ruotaDX_indietro, HIGH);
  digitalWrite(ruotaDX_avanti, LOW);
  analogWrite(pinPotenzaDX, potenza);
  delay(secondi * 1000);
  fermaMotori();
}

// Cerchio con PWM proporzionale a potenza
void cerchio_costante_live(int scale_sx, int scale_dx) {
  int pwm_sx = map(scale_sx, 0, 255, 0, potenza);
  int pwm_dx = map(scale_dx, 0, 255, 0, potenza);

  digitalWrite(ruotaSX_indietro, LOW);
  digitalWrite(ruotaSX_avanti, HIGH);
  analogWrite(pinPotenzaSX, pwm_sx);

  digitalWrite(ruotaDX_indietro, LOW);
  digitalWrite(ruotaDX_avanti, HIGH);
  analogWrite(pinPotenzaDX, pwm_dx);
}



void rettangoloArrotondato() {
  // Durata dei segmenti dritti (in secondi)
  int tempo_dritto = 2;

  // Durata curva morbida (più lunga = curva più dolce)
  int tempo_curva = 1000; // in millisecondi

  // Valori per la curva dolce
  int pwm_sx = 70;
  int pwm_dx = 145;

  for (int i = 0; i < 4; i++) {
    // Vai dritto
    digitalWrite(ruotaSX_indietro, LOW);
    digitalWrite(ruotaSX_avanti, HIGH);
    analogWrite(pinPotenzaSX, potenza);
    digitalWrite(ruotaDX_indietro, LOW);
    digitalWrite(ruotaDX_avanti, HIGH);
    analogWrite(pinPotenzaDX, potenza);
    delay(tempo_dritto * 1000);

    // Curva morbida a sinistra (bordo arrotondato)
    digitalWrite(ruotaSX_indietro, LOW);
    digitalWrite(ruotaSX_avanti, HIGH);
    analogWrite(pinPotenzaSX, pwm_sx); // più lenta
    digitalWrite(ruotaDX_indietro, LOW);
    digitalWrite(ruotaDX_avanti, HIGH);
    analogWrite(pinPotenzaDX, pwm_dx); // più veloce
    delay(tempo_curva); // quanto durare la curva

    // Ferma per stabilità
    fermaMotori();
    delay(200);
  }
}

// === LOOP PRINCIPALE ===

void loop() {
  if (!programmaAvviato) {
    // Misura distanza
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

    if (distanza > 0 && distanza <= 50) {
      Serial.println("Ostacolo rilevato! Avvio il movimento.");
      programmaAvviato = true;
      startTime = millis();

      // Esegui il rettangolo con bordi arrotondati
      rettangoloArrotondato();
    }
  } else {
    unsigned long elapsed = millis() - startTime;

    if (!bluetoothInviato && elapsed >= 10000) {
      maestroSerial.println("START");
      Serial.println("Comando START inviato via Bluetooth.");
      bluetoothInviato = true;
    }

    if (elapsed >= 60000) {
      fermaMotori();
      Serial.println("Fine movimento.");
      programmaAvviato = false;
      bluetoothInviato = false;
    }
  }

  delay(100);
}