#include <Servo.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Servo myservo;

static int trigger = 4;
static int echo = 3;

long durata;
long distanza; 

bool programmaAvviato = false; 

SoftwareSerial maestroSerial(19, 18); // Pololu
Adafruit_BNO055 bno = Adafruit_BNO055(55); // Giroscopio

const int ruotaSX_indietro = 12;
const int ruotaSX_avanti = 13;
const int pinPotenzaSX = 11;
const int ruotaDX_indietro = 8;
const int ruotaDX_avanti = 9;
const int pinPotenzaDX = 7;

const int potenza = 200;
const int tempo_per_90_gradi = 550;

// Parametri PID
float Kp = 2.5;
float Ki = 0.0;
float Kd = 0.5;
float integral = 0;
float previous_error = 0;


// === FUNZIONI ===

void startScript() {
  if(programmaAvviato) {
    maestroSerial.write(0xA7);
    Serial.println("Script avviato su Pololu");
  }
}

void fermaMotori() {
  Serial.println("Motori fermi.");
  digitalWrite(ruotaSX_indietro, LOW);
  digitalWrite(ruotaSX_avanti, LOW);
  analogWrite(pinPotenzaSX, 0);
  digitalWrite(ruotaDX_indietro, LOW);
  digitalWrite(ruotaDX_avanti, LOW);
  analogWrite(pinPotenzaDX, 0);
}

void dritto(int secondi) {
  Serial.print("Muovo avanti per ");
  Serial.print(secondi);
  Serial.println(" secondi");

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
  Serial.print("Muovo indietro per ");
  Serial.print(secondi);
  Serial.println(" secondi");

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

  float angoloDesiderato;
  if (destra) {
    angoloDesiderato = fmod(angoloIniziale + angoloTarget, 360);
  } else {
    angoloDesiderato = fmod(angoloIniziale - angoloTarget + 360, 360);
  }

  float errore = 0;
  unsigned long tempoPrecedente = millis();

  while (true) {
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    float angoloAttuale = orientationData.orientation.x;

    errore = angoloDesiderato - angoloAttuale;

    if (errore > 180) errore -= 360;
    if (errore < -180) errore += 360;

    if (abs(errore) < 1.0) break;

    unsigned long tempoAttuale = millis();
    float deltaTempo = (tempoAttuale - tempoPrecedente) / 1000.0;

    integral += errore * deltaTempo;
    float derivative = (errore - previous_error) / deltaTempo;
    float output = Kp * errore + Ki * integral + Kd * derivative;

    previous_error = errore;
    tempoPrecedente = tempoAttuale;

    output = constrain(output, -255, 255);
    int velocita = abs((int)output);
    velocita = constrain(velocita, 70, 255); // soglia minima

    if (output > 0) {
      // ruota a destra
      digitalWrite(ruotaSX_indietro, LOW);
      digitalWrite(ruotaSX_avanti, HIGH);
      analogWrite(pinPotenzaSX, velocita);
      digitalWrite(ruotaDX_indietro, HIGH);
      digitalWrite(ruotaDX_avanti, LOW);
      analogWrite(pinPotenzaDX, velocita);
    } else {
      // ruota a sinistra
      digitalWrite(ruotaSX_indietro, HIGH);
      digitalWrite(ruotaSX_avanti, LOW);
      analogWrite(pinPotenzaSX, velocita);
      digitalWrite(ruotaDX_indietro, LOW);
      digitalWrite(ruotaDX_avanti, HIGH);
      analogWrite(pinPotenzaDX, velocita);
    }


    //debug
    Serial.print("Angolo attuale: ");
    Serial.print(angoloAttuale);
    Serial.print(" | Errore: ");
    Serial.println(errore);


    delay(20);
  }

  fermaMotori();
  integral = 0;
  previous_error = 0;
}

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

// === SETUP ===

void setup() {
  Serial.begin(9600);

  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(trigger, LOW);
  delay(50);

  maestroSerial.begin(9600);

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

// === LOOP ===

void loop() {
  
  /*dritto(2);
  delay(1000);

  ruota_se_stesso_preciso(45, false);
  delay(1000);
*/
  dritto(1);
  delay(1000);

  ruota_se_stesso_preciso(45, true);
  delay(1000);
  ruota_se_stesso_preciso(45, true);
  delay(1000);
  ruota_se_stesso_preciso(45, false);
  delay(1000);
  ruota_se_stesso_preciso(45, false);
  delay(1000);
  ruota_se_stesso_preciso(45, true);
  delay(1000);
  ruota_se_stesso_preciso(45, true);
  delay(1000);

  dritto(2);
  delay(1000);

  ruota_se_stesso_preciso(45, false);
  delay(1000);
  ruota_se_stesso_preciso(45, false);
  delay(1000);
  ruota_se_stesso_preciso(45, true);
  delay(1000);
  ruota_se_stesso_preciso(45, true);
  delay(1000);
  ruota_se_stesso_preciso(45, false);
  delay(1000);
  ruota_se_stesso_preciso(45, false);
  delay(1000);

  dritto(1);
  delay(1000);
}
