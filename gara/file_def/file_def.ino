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

const int potenza = 100;

// === PID ===

float Kp = 2.5;
float Ki = 0.0;
float Kd = 0.5;
float integral = 0;
float previous_error = 0;

bool programmaAvviato = false;
long durata;
long distanza;

// === Setup ===

void setup() {
  Serial.begin(9600);
  maestroSerial.begin(9600);

  Serial3.begin(9600);  // HC-05 MASTER
  Serial3.println("START");

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
  int potenzaDritto = 70; 

  digitalWrite(ruotaSX_indietro, LOW);
  digitalWrite(ruotaSX_avanti, HIGH);
  analogWrite(pinPotenzaSX, potenzaDritto);

  digitalWrite(ruotaDX_indietro, LOW);
  digitalWrite(ruotaDX_avanti, HIGH);
  analogWrite(pinPotenzaDX, potenzaDritto);

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

// === Movimento a cerchio ===

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

// === LOOP ===

void loop() {
  if (!programmaAvviato) {
    // === Lettura distanza ===
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

    // === Se ostacolo vicino, avvia movimento ===
    if (distanza > 0 && distanza <= 50) {
      Serial.println("Ostacolo rilevato! Avvio cerchio.");
      programmaAvviato = true;

      dritto(5);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000); 
      dritto(7);
      delay(1000);


      ruota_se_stesso_preciso(45, false);
      delay(1000);


      indietro(2.5);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);

      dritto(4.5);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);
      
      indietro(2.5);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);

      dritto(4.5);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);

      indietro(2.5);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);

      dritto(4.5);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);

      indietro(2.5);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);

      dritto(4.5);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);

      indietro(2.5);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);

      dritto(4.5);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);

      indietro(2);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);

      dritto(2);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);

      indietro(2);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);

      dritto(2);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);

      indietro(2);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);

      dritto(2);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, true);
      delay(1000);
      ruota_se_stesso_preciso(45, false);
      delay(1000);



      fermaMotori();

    }
  }

  delay(100);
}
