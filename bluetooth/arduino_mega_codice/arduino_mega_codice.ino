void setup() {
  Serial.begin(9600);        // USB monitor
  Serial3.begin(9600);
  //Serial1.begin(9600);       // HC-05 Master
  Serial.println("Avvio comunicazione Bluetooth (MASTER)...");
}

void loop() {
  Serial3.println("Ciao da Arduino Mega!");
  Serial.println("Messaggio inviato.");
  delay(2000);
}
