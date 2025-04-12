#include <Servo.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

// === Bluetooth ===
SoftwareSerial BTSerial(10, 11); // RX, TX

// === Servo ===
Servo myservo;
const int servoPin = 9;
float t = 0;
const float speedFactor = 0.12;

// === WS2812B ===
#define LED_PIN 6
#define NUM_LEDS 33
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// === Stato ===
bool programmaAvviato = false;
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

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);

  myservo.attach(servoPin);
  myservo.write(30);

  strip.begin();
  strip.show(); // Spegne tutti i LED

  Serial.println("UNO pronto. In attesa del comando START via Bluetooth...");
}

void loop() {
  // 🎯 Ricezione comando via Bluetooth
  if (BTSerial.available()) {
    String comando = BTSerial.readStringUntil('\n');
    comando.trim();

    if (comando == "START") {
      Serial.println("Comando START ricevuto. Avvio programma...");
      programmaAvviato = true;
    }
  }

  if (programmaAvviato) {
    // 🤖 Movimento servo (ritmo scimmia invariato)
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

  delay(20); // solo per il movimento fluido del servo
}

void setColor(uint32_t color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
