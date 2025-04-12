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

// === Movimento rettangolare ===

void eseguiRettangolo() {
  dritto(2);                         // 1 metro avanti
  ruota_se_stesso_preciso(90, true); // gira destra
  dritto(1);                         // 50 cm avanti
  ruota_se_stesso_preciso(90, false); // gira sinistra
  indietro(2);                      // 1 metro indietro
  ruota_se_stesso_preciso(90, false); // gira sinistra
  dritto(1);                         // 50 cm avanti
  ruota_se_stesso_preciso(90, true);  // gira destra
}

// === Loop principale ===

void loop() {
  if (!programmaAvviato) {
    delay(1000); // tempo per prepararsi
    programmaAvviato = true;
    eseguiRettangolo();
    while (true); // ferma tutto dopo il giro
  }
}
