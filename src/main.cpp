
// Use these to enable/disable the radiation and/or lightning detection code
#define ENABLE_RADIATION
// #define ENABLE_LIGHTNING

#include <Arduino.h>

#include <Config.h>
#include <Wifi.h>
#include <MQTT.h>
#include <Uptime.h>
#include <Blink.h>
#include <Radiation.h>
#include <Lightning.h>

// Pull this low to reset the clear the wifi settings and format the file system.
#define RESET_PIN D0

// Interval (ms) between each uptime update
#define UPTIME_INTERVAL 10000


#ifdef ENABLE_RADIATION
// Interrupt pin for radiation sensor
#define RADIATION_IRQ_PIN D2
#endif // ENABLE_RADIATION

#ifdef ENABLE_LIGHTNING
// Interrupt pin for lightning sensor
#define LIGHTNING_IRQ_PIN D1

// CS pin for lightning sensor
#define LIGHTNING_CS_PIN D8
#endif // ENABLE_LIGHTNING

Stream &logger = Serial;

Config cfg(logger, "/config.json");
Wifi wifi(logger, cfg);
MQTT mqtt(logger, cfg);
Uptime uptime(logger, mqtt, UPTIME_INTERVAL);

bool resetEverything = false;
#ifdef ENABLE_RADIATION
bool radiationOk;
#endif // ENABLE_RADIATION
#ifdef ENABLE_LIGHTNING
bool lightningOk;
#endif // ENABLE_LIGHTNING

// Indicator LED pin
Blink led(logger, LED_BUILTIN, true);

#ifdef ENABLE_RADIATION
// Radiation detector
Radiation radiation(logger, led, mqtt);
#endif // ENABLE_RADIATION

#ifdef ENABLE_LIGHTNING
// Lightning detector
Lightning lightning(logger, led, mqtt, LIGHTNING_CS_PIN, LIGHTNING_IRQ_PIN, false);
#endif // ENABLE_LIGHTNING

#ifdef ENABLE_RADIATION
ICACHE_RAM_ATTR void radiation_isr() {
  radiation.isr();
}
#endif // ENABLE_RADIATION

#ifdef ENABLE_LIGHTNING
ICACHE_RAM_ATTR void lightning_isr() {
  lightning.isr();
}
#endif // ENABLE_LIGHTNING

#ifdef ENABLE_RADIATION
void setupRadiation() {
  if (!(radiationOk = radiation.setup())) {
    logger.println(F("R: Radiation detector initialization failed."));
    return;
  }
  pinMode(RADIATION_IRQ_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(RADIATION_IRQ_PIN), radiation_isr, FALLING);
}
#endif // ENABLE_RADIATION

#ifdef ENABLE_LIGHTNING
void setupLightning() {
  if (!(lightningOk = lightning.setup())) {
    logger.println(F("L: Lightning detector initialization failed."));
    return;
  }
  pinMode(LIGHTNING_IRQ_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(LIGHTNING_IRQ_PIN), lightning_isr, RISING);
}
#endif // ENABLE_LIGHTNING

void checkReset() {
  pinMode(RESET_PIN, INPUT_PULLUP);
  delay(200);

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
  uptime.setup();

  // Lastly, set up the detectors
  interrupts();
#ifdef ENABLE_RADIATION
  setupRadiation();
#endif // ENABLE_RADIATION
#ifdef ENABLE_LIGHTNING  
  setupLightning();
#endif // ENABLE_LIGHTNING

}

void loop() {
  uptime.loop();
#ifdef ENABLE_RADIATION
  if (radiationOk) radiation.loop();
#endif // ENABLE_RADIATION
#ifdef ENABLE_LIGHTNING
  if (lightningOk) lightning.loop();
#endif // ENABLE_LIGHTNING
  led.loop();
  mqtt.loop();

}


