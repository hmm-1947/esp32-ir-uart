#include <Arduino.h>
#include <HardwareSerial.h>

#define IR_LED_PIN 17
#define IR_RX_PIN 16

HardwareSerial IRSerial(2); 

// Exact timing for 300 baud (3.333 ms per bit)
void sendBit(bool b) {
  digitalWrite(IR_LED_PIN, b ? HIGH : LOW);
  delayMicroseconds(3333);
}

void sendByte(uint8_t b) {
  // Start bit
  sendBit(0);

  // Data bits (LSB first)
  for (int i = 0; i < 8; i++) {
    sendBit(b & 1);
    b >>= 1;
  }

  // Stop bit
  sendBit(1);
}

void sendString(const char* s) {
  while (*s) {
    sendByte(*s);
    delay(500);  // spacing to avoid overlap
    s++;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(IR_LED_PIN, OUTPUT);
  digitalWrite(IR_LED_PIN, HIGH); // idle = HIGH

  // BC547 usually produces inverted logic — if wrong, change to false
  IRSerial.begin(300, SERIAL_8N1, IR_RX_PIN, -1, true);

  Serial.println("IR Link Ready...");
}

void loop() {
  // SEND ANY MESSAGE HERE:
  sendString("HELLO ESP32 OVER IR ");

  // RECEIVE
  while (IRSerial.available()) {
    char c = IRSerial.read();
    Serial.print("Received: ");
    Serial.print((int)c);
    Serial.print("  Char: ");
    Serial.println(c);
  }

  delay(1000);
}