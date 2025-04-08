#include <SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600); // Prova anche 38400 se non ricevi nulla
  //BTSerial.begin(9600); // Prova anche 38400 se non ricevi nulla
  Serial.println("In ascolto su Bluetooth...");
}

void loop() {
  if (BTSerial.available()) {
    char c = BTSerial.read();
    Serial.write(c); // mostra nel Monitor Seriale
  }
}
