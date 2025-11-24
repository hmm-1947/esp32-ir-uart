#include <Arduino.h>
#include <HardwareSerial.h>

#define IR_TX_PIN 17
#define IR_RX_PIN 16

HardwareSerial IRSerial(2);

// ----- SEND ONE BIT -----
void sendBit(bool bit) {
  digitalWrite(IR_TX_PIN, bit ? HIGH : LOW);
  delayMicroseconds(416);   // 2400 baud
}

// ----- SEND ONE BYTE -----
void sendByte(uint8_t b) {
  sendBit(0);          // start bit
  for (int i = 0; i < 8; i++) {
    sendBit(b & 1);
    b >>= 1;
  }
  sendBit(1);          // stop bit
}

// ----- SEND A STRING -----
void sendString(const char* s) {
  while (*s) {
    sendByte(*s);
    delay(5);          // small spacing
    s++;
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(IR_TX_PIN, OUTPUT);
  digitalWrite(IR_TX_PIN, HIGH);  // idle state

  // LM393 output is inverted → enable invert mode
  IRSerial.begin(2400, SERIAL_8N1, IR_RX_PIN, -1, true);

  Serial.println("IR Test Ready...");
}

void loop() {

  sendString("HELLO ESP32 OVER IR\n");

  Serial.println("[TX] Sent HELLO...");

  // Receive characters if any
  while (IRSerial.available()) {
    char c = IRSerial.read();
    Serial.print("[RX] ASCII: ");
    Serial.print((int)c);
    Serial.print("  Char: ");
    Serial.println(c);
  }

  delay(500);
}
