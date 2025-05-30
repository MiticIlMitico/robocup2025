//AP R4
#include <WiFiS3.h>
#include <WiFiUdp.h>

char ssid[] = "R4_AP";       // Nome della rete
char pass[] = "12345678";    // Password (minimo 8 caratteri)

WiFiUDP Udp;
unsigned int localPort = 4210;

void setup() {
  Serial.begin(9600);

  // Avvio in modalità Access Point
  Serial.println("Configurazione Access Point...");
  WiFi.beginAP(ssid, pass);

  // Attendere l'avvio
  while (WiFi.status() != WL_AP_LISTENING) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nAccess Point attivo!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Avvio UDP
  Udp.begin(localPort);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    char packet[255];
    int len = Udp.read(packet, 255);
    if (len > 0) packet[len] = 0;
    Serial.print("Ricevuto pacchetto: ");
    Serial.println(packet);
    
    // Rispondi al mittente
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write("L");
    Udp.endPacket();
  }
}