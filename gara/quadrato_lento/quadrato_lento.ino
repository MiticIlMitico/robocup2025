#include <Servo.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// === Oggetti e pin ===

Servo myservo;
SoftwareSerial maestroSerial(19, 18); // Pololu
Adafruit_BNO055 bno = Adafruit_BNO055(55); // Giroscopio

const int trigger = 4;
const int echo = 3;

const int ruotaSX_indietro = 12;
const int ruotaSX_avanti = 13;
const int pinPotenzaSX = 11;
const int ruotaDX_indietro = 8;
const int ruotaDX_avanti = 9;
const int pinPotenzaDX = 7;

const int potenza_lenta = 120; // 🔹 Potenza ridotta per movimento lento
const int soglia_distanza_cm = 30;

bool programmaAvviato = false;

// === Setup ===

void setup() {
  Serial.begin(9600);
  maestroSerial.begin(9600);

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

// === Misurazione distanza ===

long misuraDistanza() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  long durata = pulseIn(echo, HIGH, 30000); // massimo 30 ms
  long distanza_cm = durata * 0.034 / 2;

  return distanza_cm;
}

// === Movimento ===

void fermaMotori() {
  digitalWrite(ruotaSX_indietro, LOW);
  digitalWrite(ruotaSX_avanti, LOW);
  analogWrite(pinPotenzaSX, 0);
  digitalWrite(ruotaDX_indietro, LOW);
  digitalWrite(ruotaDX_avanti, LOW);
  analogWrite(pinPotenzaDX, 0);
}

void dritto(float secondi) {
  digitalWrite(ruotaSX_indietro, LOW);
  digitalWrite(ruotaSX_avanti, HIGH);
  analogWrite(pinPotenzaSX, potenza_lenta);
  digitalWrite(ruotaDX_indietro, LOW);
  digitalWrite(ruotaDX_avanti, HIGH);
  analogWrite(pinPotenzaDX, potenza_lenta);
  delay(secondi * 1000);
  fermaMotori();
}

void indietro(float secondi) {
  digitalWrite(ruotaSX_indietro, HIGH);
  digitalWrite(ruotaSX_avanti, LOW);
  analogWrite(pinPotenzaSX, potenza_lenta);
  digitalWrite(ruotaDX_indietro, HIGH);
  digitalWrite(ruotaDX_avanti, LOW);
  analogWrite(pinPotenzaDX, potenza_lenta);
  delay(secondi * 1000);
  fermaMotori();
}

// === Movimento curvo ===

void curva(bool destra, float durata_secondi, int intensita) {
  // 🔹 intensità minore = curva più morbida
  int pwm_sx, pwm_dx;
  if (destra) {
    pwm_sx = potenza_lenta - intensita;
    pwm_dx = potenza_lenta + intensita;
  } else {
    pwm_sx = potenza_lenta + intensita;
    pwm_dx = potenza_lenta - intensita;
  }

  pwm_sx = constrain(pwm_sx, 70, 255);
  pwm_dx = constrain(pwm_dx, 70, 255);

  digitalWrite(ruotaSX_indietro, LOW);
  digitalWrite(ruotaSX_avanti, HIGH);
  analogWrite(pinPotenzaSX, pwm_sx);

  digitalWrite(ruotaDX_indietro, LOW);
  digitalWrite(ruotaDX_avanti, HIGH);
  analogWrite(pinPotenzaDX, pwm_dx);

  delay(durata_secondi * 1000);
  fermaMotori();
}

// === Percorso quadrato lento ===

void eseguiQuadrato() {
  dritto(1.0);               // Avanza 1 metro lentamente
  curva(true, 2.0, 30);      // Curva destra lenta
  dritto(1.0);               // Avanza 50 cm lentamente
  curva(true, 2.0, 30);      // Curva destra lenta
  dritto(1.0);               // Avanza 50 cm lentamente
  curva(true, 2.0, 30);      // Curva destra lenta
  dritto(1.0);               // Avanza 50 cm lentamente
  curva(true, 2.0, 30);      // Curva destra lenta
}

// === Loop principale ===

void loop() {
  if (!programmaAvviato) {

    Serial.println("Oggetto rilevato. Avvio movimento lento.");
    programmaAvviato = true;
    delay(500);
    eseguiQuadrato();

    delay(1000); // Controlla ogni secondo
  }
}
