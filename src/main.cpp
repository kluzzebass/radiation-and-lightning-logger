#include <Arduino.h>

#include <Geiger.h>

#define GEIGER_PIN D1

Geiger geiger(Serial);

IRAM_ATTR void geiger_isr() {
  geiger.isr();
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n\nhello");

  interrupts();
  pinMode(GEIGER_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(GEIGER_PIN), geiger_isr, FALLING);
}

void loop() {
  geiger.loop();
}