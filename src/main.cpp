#include <Arduino.h>

#include <Geiger.h>
#include <Lightning.h>

// Interrupt pin for radiation sensor
#define GEIGER_IRQ_PIN D2

// Interrupt pin for lightning sensor
#define LIGHTNING_IRQ_PIN D1

// CS pin for lightning sensor
#define LIGHTNING_CS_PIN D8

bool geigerOk;
bool lightningOk;

Geiger geiger(Serial);
Lightning lightning(Serial, LIGHTNING_CS_PIN, LIGHTNING_IRQ_PIN);

ICACHE_RAM_ATTR void geiger_isr() {
  geiger.isr();
}

ICACHE_RAM_ATTR void lightning_isr() {
  lightning.isr();
}

void setup() {
  Serial.begin(115200);

  if (!(geigerOk = geiger.setup())) {
    Serial.println("G: Radiation detector initialization failed.");
  }

  if (!(lightningOk = lightning.setup())) {
    Serial.println("L: Lightning detector initialization failed.");
  }

  interrupts();

  pinMode(GEIGER_IRQ_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(GEIGER_IRQ_PIN), geiger_isr, FALLING);

  pinMode(LIGHTNING_IRQ_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(LIGHTNING_IRQ_PIN), lightning_isr, RISING);

}

void loop() {
  if (geigerOk) geiger.loop();
  if (lightningOk) lightning.loop();
}