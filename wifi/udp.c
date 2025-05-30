//UDP Client
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>



// === Servo ===
Servo myservo;
const int servoPin = 11;
float t = 0;
const float speedFactor = 0.05;

// === WS2812B ===
#define LED_PIN 6
#define NUM_LEDS 33
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// === Stato ===
unsigned long lastColorChange = 0;
const unsigned long colorInterval = 300; // 300ms = bum bum bum
int currentColorIndex = 0;

// Colori LED da ciclare
const uint32_t colors[] = {
  // Verde
  strip.Color(0, 128, 0),     // Verde scuro
  strip.Color(0, 200, 50),    // Verde medio
  strip.Color(0, 255, 100),   // Verde chiaro

  // Arancione
  strip.Color(200, 80, 0),    // Arancione scuro
  strip.Color(255, 120, 0),   // Arancione medio
  strip.Color(255, 180, 60),  // Arancione chiaro

  // Azzurro
  strip.Color(0, 120, 200),   // Azzurro scuro
  strip.Color(0, 180, 255),   // Azzurro medio
  strip.Color(100, 220, 255)  // Azzurro chiaro
};

const int numColors = sizeof(colors) / sizeof(colors[0]);


// AP e CONNESSIONE WIFI
char ssid[] = "R4_AP";         // Nome della rete AP creata dal R4
char pass[] = "12345678";      // Password
int status = WL_IDLE_STATUS;

WiFiUDP udp;
const char* host = "192.168.4.1"; // IP dell'UNO R4 in modalità AP
unsigned int port = 4210;



void setColor(uint32_t color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}


void setup() {
    Serial.begin(9600);
    while (!Serial);

    myservo.attach(servoPin);
    myservo.write(30);

    // Connessione al Wi-Fi
    Serial.print("Connessione a: ");
    Serial.println(ssid);

  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    delay(1000);
  }
    Serial.println("Connesso!");
    Serial.print("IP assegnato: ");
    Serial.println(WiFi.localIP());
    udp.begin(port);  // CORRETTO


    strip.begin();
    strip.show();  // Spegne tutti i LED all'inizio
    Serial.println("Test luminosità LED WS2812B");

}

// === Stato ricezione ===
bool triggerAttivo = false;  // Una volta true, non si torna più indietro

void loop() {
    if (!triggerAttivo) {
        // Invia messaggio
        const char* msg = "Ciao da MKR1000";
        udp.beginPacket(host, port);
        udp.write(msg);
        udp.endPacket();

        Serial.println("Messaggio inviato");

        // Ricezione risposta
        delay(500);
        int packetSize = udp.parsePacket();
        if (packetSize) {
            char reply[255];
            int len = udp.read(reply, 255);
            if (len > 0) reply[len] = '\0';
            Serial.print("Risposta: ");
            Serial.println(reply);
            delay(200);

            if (reply[0] == 'L') {
                triggerAttivo = true; // Attiva modalità permanente
            }
        }
    }

    if (triggerAttivo) {
        // 🤖 Movimento servo (ritmo scimmia invariato)
        for (int i = 0; i < 10; i++) {
          float sinValue = (sin(t) + 1) / 2;
          int angle = 0 + sinValue * 60;
          myservo.write(angle);
          t += speedFactor;
          delay(20);

        // 💡 Cambio colore LED ogni 300ms (indipendente)
        if (millis() - lastColorChange >= colorInterval) {
            lastColorChange = millis();
            setColor(colors[currentColorIndex]);
            currentColorIndex = (currentColorIndex + 1) % numColors;
        }
        }
      
    }
}

// Funzione per riempire tutta la striscia con un colore
void fillStrip(uint32_t color) {
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, color);
    }
    strip.show();
}