#include <Servo.h>
#include <SoftwareSerial.h>

Servo myservo;

static int trigger = 4;
static int echo = 3;

long durata;
long distanza; 

bool programmaAvviato = false; 



// Definizione della comunicazione seriale con Pololu
SoftwareSerial maestroSerial(19, 18);


// Pin motori per avanti e indietro
const int ruotaSX_indietro = 12; // Ruota sinistra indietro
const int ruotaSX_avanti = 13;   // Ruota sinistra avanti
const int pinPotenzaSX = 11;
const int ruotaDX_indietro = 8;  // Ruota destra indietro
const int ruotaDX_avanti = 9;    // Ruota destra avanti
const int pinPotenzaDX = 7;


// Costanti di configurazione
const int potenza = 200; // Potenza PWM per i motori
const int tempo_per_90_gradi = 550; // Tempo stimato per girare di 90°








// Funzione per avviare lo script su Pololu
void startScript() {
    if(programmaAvviato) {
      maestroSerial.write(0xA7); // Comando "Restart Script"
      Serial.println("Script avviato su Pololu");
    }
}


// Funzione per fermare i motori
void fermaMotori() {
    Serial.println("Motori fermi.");
    digitalWrite(ruotaSX_indietro, LOW);
    digitalWrite(ruotaSX_avanti, LOW);
    analogWrite(pinPotenzaSX, 0);
    digitalWrite(ruotaDX_indietro, LOW);
    digitalWrite(ruotaDX_avanti, LOW);
    analogWrite(pinPotenzaDX, 0);
}


// Funzione per muoversi dritto
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
    digitalWrite(ruotaDX_avanti, LOW); startScript();
    analogWrite(pinPotenzaDX, potenza);

    delay(secondi * 1000);
    fermaMotori();
}


// Funzione per ruotare
void ruota(int gradi, bool destra) {
    fermaMotori();
    delay(100);

    int tempo_rotazione = (tempo_per_90_gradi * gradi) / 90;

    Serial.print("Ruoto di ");
    Serial.print(gradi);
    Serial.print(" gradi a ");
    Serial.println(destra ? "destra" : "sinistra");

    if (destra) {
        digitalWrite(ruotaSX_indietro, LOW);
        digitalWrite(ruotaSX_avanti, HIGH);
        analogWrite(pinPotenzaSX, potenza);
        digitalWrite(ruotaDX_indietro, LOW);
        digitalWrite(ruotaDX_avanti, LOW);
        analogWrite(pinPotenzaDX, 0);
    } else {
        digitalWrite(ruotaSX_indietro, LOW);
        digitalWrite(ruotaSX_avanti, LOW);
        analogWrite(pinPotenzaSX, 0);
        digitalWrite(ruotaDX_indietro, LOW);
        digitalWrite(ruotaDX_avanti, HIGH);
        analogWrite(pinPotenzaDX, potenza);
    }

    delay(tempo_rotazione);
    fermaMotori();
    delay(100);
}


// Funzione per ruotare su se stesso
void ruota_se_stesso(int gradi, bool destra) {
    fermaMotori();
    delay(100);

    int tempo_rotazione = (tempo_per_90_gradi * gradi) / 90;

    Serial.print("Ruoto su me stesso di ");
    Serial.print(gradi);
    Serial.print(" gradi a ");
    Serial.println(destra ? "destra" : "sinistra");

    if (destra) {
        digitalWrite(ruotaSX_indietro, LOW);
        digitalWrite(ruotaSX_avanti, HIGH);
        analogWrite(pinPotenzaSX, potenza);
        digitalWrite(ruotaDX_indietro, HIGH);
        digitalWrite(ruotaDX_avanti, LOW);
        analogWrite(pinPotenzaDX, potenza);
    } else {
        digitalWrite(ruotaSX_indietro, HIGH);
        digitalWrite(ruotaSX_avanti, LOW);
        analogWrite(pinPotenzaSX, potenza);
        digitalWrite(ruotaDX_indietro, LOW);
        digitalWrite(ruotaDX_avanti, HIGH);
        analogWrite(pinPotenzaDX, potenza);
    }

    delay(tempo_rotazione);
    fermaMotori();
    delay(100);
}








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

  myservo.write(30);



  maestroSerial.begin(9600);

  pinMode(ruotaSX_indietro, OUTPUT);
  pinMode(ruotaSX_avanti, OUTPUT);
  pinMode(pinPotenzaSX, OUTPUT);
  pinMode(ruotaDX_indietro, OUTPUT);
  pinMode(ruotaDX_avanti, OUTPUT);
  pinMode(pinPotenzaDX, OUTPUT);

  fermaMotori();
}

void loop() {
  if (!programmaAvviato) {
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

        if (distanza <= 50 && distanza > 0) {
            Serial.println("Ostacolo rilevato! Avvio il movimento del servo.");
            programmaAvviato = true;
        }
  }

  if (programmaAvviato) {
    
    startScript();

    
    dritto(2.5);
    delay(2000);

    
    // ruota a sinistra di 45 ;
    ruota_se_stesso(45, false);

    // ruota(45, false);
    // ruota(45, false);
    delay(2000);


    dritto(1);
    delay(2000);


    // ruota a destra di 45
    ruota_se_stesso(45, true);
    //ruota(28, true);
    //ruota(28, true);
    delay(2000);


    dritto(2);
    delay(2000);
    

    // ruota su se stesso???
    ruota_se_stesso(45, true);
    delay(500);
    ruota_se_stesso(45, false);
    delay(500);
    ruota_se_stesso(45, false);
    delay(500);
    ruota_se_stesso(45, true);
    delay(500);



    indietro(2);
    delay(2000);


    ruota_se_stesso(45, true);
    delay(500);



    dritto(1);
    delay(2000);



    ruota_se_stesso(45, false);
    delay(500);



    dritto(2);
    delay(2000);


     ruota_se_stesso(45, true);
    delay(500);
    ruota_se_stesso(45, false);
    delay(500);
    ruota_se_stesso(45, false);
    delay(500);
    ruota_se_stesso(45, true);
    delay(500);

    indietro(2);
    delay(2000);


    ruota_se_stesso(45, true);
    delay(500);


    dritto(1);
    delay(2000);


    ruota_se_stesso(45, false);
    delay(500);



    dritto(2);
    delay(2000);


     ruota_se_stesso(45, true);
    delay(500);
    ruota_se_stesso(45, false);
    delay(500);
    ruota_se_stesso(45, false);
    delay(500);
    ruota_se_stesso(45, true);
    delay(500);

    indietro(2);
    delay(2000);

    // ruota a sinistra di 45 ;
    ruota_se_stesso(45, false);
    delay(2000);

    dritto(1);
    delay(2000);

    ruota_se_stesso(45, true);
    delay(2000);

    dritto(2);
    delay(2000);

    for(int i = 0; i < 1000; i++) {
      ruota_se_stesso(45, true);
      delay(500);
      ruota_se_stesso(45, false);
      delay(500);
      ruota_se_stesso(45, false);
      delay(500);
      ruota_se_stesso(45, true);
      delay(500);
    }

  }

  delay(100);
}