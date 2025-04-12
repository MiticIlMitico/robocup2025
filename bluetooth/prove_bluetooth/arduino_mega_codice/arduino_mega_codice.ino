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

const int potenza = 200;

// === PID ===

float Kp = 2.5;
float Ki = 0.0;
float Kd = 0.5;
float integral = 0;
float previous_error = 0;

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

// === Funzioni di movimento ===

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

void ruota_se_stesso_preciso(float angoloTarget, bool destra) {
  sensors_event_t orientationData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  float angoloIniziale = orientationData.orientation.x;

  float angoloDesiderato = destra
    ? fmod(angoloIniziale + angoloTarget, 360)
    : fmod(angoloIniziale - angoloTarget + 360, 360);

  float errore = 0;
  unsigned long tempoPrecedente = millis();

  while (true) {
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    float angoloAttuale = orientationData.orientation.x;
    errore = angoloDesiderato - angoloAttuale;

    if (errore > 180) errore -= 360;
    if (errore < -180) errore += 360;
    if (abs(errore) < 0.5) break;

    unsigned long tempoAttuale = millis();
    float deltaTempo = (tempoAttuale - tempoPrecedente) / 1000.0;

    integral += errore * deltaTempo;
    float derivative = (errore - previous_error) / deltaTempo;
    float output = Kp * errore + Ki * integral + Kd * derivative;

    previous_error = errore;
    tempoPrecedente = tempoAttuale;

    output = constrain(output, -255, 255);
    int velocita = abs((int)output);
    velocita = constrain(velocita, 70, 255);

    if (output > 0) {
      digitalWrite(ruotaSX_indietro, LOW);
      digitalWrite(ruotaSX_avanti, HIGH);
      analogWrite(pinPotenzaSX, velocita);
      digitalWrite(ruotaDX_indietro, HIGH);
      digitalWrite(ruotaDX_avanti, LOW);
      analogWrite(pinPotenzaDX, velocita);
    } else {
      digitalWrite(ruotaSX_indietro, HIGH);
      digitalWrite(ruotaSX_avanti, LOW);
      analogWrite(pinPotenzaSX, velocita);
      digitalWrite(ruotaDX_indietro, LOW);
      digitalWrite(ruotaDX_avanti, HIGH);
      analogWrite(pinPotenzaDX, velocita);
    }

    delay(20);
  }

  fermaMotori();
  integral = 0;
  previous_error = 0;
}

// === Nuova funzione: movimento a cerchio ===

void cerchio(float durata_secondi) {
  int potenzaInterna = 180;  // ruota sinistra (interna al cerchio)
  int potenzaEsterna = 220;  // ruota destra (esterna al cerchio)

  digitalWrite(ruotaSX_indietro, LOW);
  digitalWrite(ruotaSX_avanti, HIGH);
  analogWrite(pinPotenzaSX, potenzaInterna);

  digitalWrite(ruotaDX_indietro, LOW);
  digitalWrite(ruotaDX_avanti, HIGH);
  analogWrite(pinPotenzaDX, potenzaEsterna);

  delay(durata_secondi * 1000);
  fermaMotori();
}

void cerchio_avanzato(float durata_totale_sec, int steps, float intensitaCurva) {
  float t_step = durata_totale_sec / steps;

  for (int i = 0; i < steps; i++) {
    float fase = (PI / 2) * i / steps; // da 0 a PI/2 (una sola direzione)
    float pwm_offset = sin(fase) * intensitaCurva;

    int pwm_sx = constrain(potenza - pwm_offset, 100, 255);
    int pwm_dx = constrain(potenza + pwm_offset, 100, 255);

    digitalWrite(ruotaSX_indietro, LOW);
    digitalWrite(ruotaSX_avanti, HIGH);
    analogWrite(pinPotenzaSX, pwm_sx);

    digitalWrite(ruotaDX_indietro, LOW);
    digitalWrite(ruotaDX_avanti, HIGH);
    analogWrite(pinPotenzaDX, pwm_dx);

    delay(t_step * 1000);
  }

  fermaMotori();
}


void cerchio_costante(float durata_secondi, int pwm_sx, int pwm_dx) {
  digitalWrite(ruotaSX_indietro, LOW);
  digitalWrite(ruotaSX_avanti, HIGH);
  analogWrite(pinPotenzaSX, pwm_sx);

  digitalWrite(ruotaDX_indietro, LOW);
  digitalWrite(ruotaDX_avanti, HIGH);
  analogWrite(pinPotenzaDX, pwm_dx);

  delay(durata_secondi * 1000);

  fermaMotori();
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

