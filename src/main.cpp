#include <Arduino.h>

#include <Blink.h>
#include <Radiation.h>
#include <Lightning.h>

// Interrupt pin for radiation sensor
#define RADIATION_IRQ_PIN D2

// Interrupt pin for lightning sensor
#define LIGHTNING_IRQ_PIN D1

// CS pin for lightning sensor
#define LIGHTNING_CS_PIN D8

bool radiationOk;
bool lightningOk;

// Indicator LED pin
Blink led(Serial, LED_BUILTIN, true);

// Radiation detector
Radiation radiation(Serial, led);

// Lightning detector
Lightning lightning(Serial, led, LIGHTNING_CS_PIN, LIGHTNING_IRQ_PIN, false);

ICACHE_RAM_ATTR void radiation_isr() {
  radiation.isr();
}

ICACHE_RAM_ATTR void lightning_isr() {
  lightning.isr();
}

void setup() {

  Serial.begin(115200);

  led.setup();

  if (!(radiationOk = radiation.setup())) {
    Serial.println("R: Radiation detector initialization failed.");
  }

  if (!(lightningOk = lightning.setup())) {
    Serial.println("L: Lightning detector initialization failed.");
  }

  interrupts();

  pinMode(RADIATION_IRQ_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(RADIATION_IRQ_PIN), radiation_isr, FALLING);

  pinMode(LIGHTNING_IRQ_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(LIGHTNING_IRQ_PIN), lightning_isr, RISING);

}

void loop() {
  if (radiationOk) radiation.loop();
  if (lightningOk) lightning.loop();
  led.loop();
}