
#include <Blink.h>

void Blink::blink(unsigned int duration) {
  onUntil = millis() + duration;
  digitalWrite(pin, !activeLow);
  on = true;
  logger.print(F("B: Pin "));
  logger.print(pin);
  logger.print(F(" is on for "));
  logger.print(duration);
  logger.println(F("ms."));
}

void Blink::setup() {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, activeLow);
  logger.print(F("B: Blink on pin "));
  logger.print(pin);
  logger.println(F(" initialized."));
}

void Blink::loop() {
  if (on && millis() >= onUntil) {
    digitalWrite(pin, activeLow);
    on = false;
    logger.print(F("B: Pin "));
    logger.print(pin);
    logger.println(F(" is off."));
  }
}
