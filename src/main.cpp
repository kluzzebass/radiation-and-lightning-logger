#include <Arduino.h>

#include <Config.h>
#include <Wifi.h>
#include <MQTT.h>
#include <Blink.h>
#include <Radiation.h>
#include <Lightning.h>

// Pull this low to reset the clear the wifi settings and format the file system.
#define RESET_PIN D0

// Interrupt pin for radiation sensor
#define RADIATION_IRQ_PIN D2

// Interrupt pin for lightning sensor
#define LIGHTNING_IRQ_PIN D1

// CS pin for lightning sensor
#define LIGHTNING_CS_PIN D8

Stream &logger = Serial;

Config cfg(logger, "/config.json");
Wifi wifi(logger, cfg);
MQTT mqtt(logger, cfg);

bool resetEverything = false;
bool radiationOk;
bool lightningOk;

// Indicator LED pin
Blink led(logger, LED_BUILTIN, true);

// Radiation detector
Radiation radiation(logger, led, mqtt);

// Lightning detector
Lightning lightning(logger, led, mqtt, LIGHTNING_CS_PIN, LIGHTNING_IRQ_PIN, false);

ICACHE_RAM_ATTR void radiation_isr() {
  radiation.isr();
}

ICACHE_RAM_ATTR void lightning_isr() {
  lightning.isr();
}

void setupRadiation() {
  if (!(radiationOk = radiation.setup())) {
    logger.println(F("R: Radiation detector initialization failed."));
    return;
  }
  pinMode(RADIATION_IRQ_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(RADIATION_IRQ_PIN), radiation_isr, FALLING);
}

void setupLightning() {
  if (!(lightningOk = lightning.setup())) {
    logger.println(F("L: Lightning detector initialization failed."));
    return;
  }
  pinMode(LIGHTNING_IRQ_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(LIGHTNING_IRQ_PIN), lightning_isr, RISING);
}

void checkReset() {
  pinMode(RESET_PIN, INPUT_PULLUP);
  delay(2);

  if (digitalRead(RESET_PIN) == 0) {
    logger.println("### RESETTING EVERYTHING ###");
    resetEverything = true;
  }
}


void setup() {

  Serial.begin(115200);

  checkReset();

  led.setup();
  cfg.setup(resetEverything);
  cfg.read();
  wifi.setup(resetEverything);
  mqtt.setup();

  // Lastly, set up the detectors
  interrupts();
  setupRadiation();
  setupLightning();

}

void loop() {
  if (radiationOk) radiation.loop();
  if (lightningOk) lightning.loop();
  led.loop();
  mqtt.loop();
}