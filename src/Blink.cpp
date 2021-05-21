
#include <Blink.h>

void Blink::blink(unsigned int duration) {
  onUntil = millis() + duration;
  digitalWrite(pin, !activeLow);
  on = true;
  log.print("B: Pin ");
  log.print(pin);
  log.print(" is on for ");
  log.print(duration);
  log.println("ms.");
}

void Blink::setup() {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, activeLow);
  log.print("B: Blink on pin ");
  log.print(pin);
  log.println(" initialized.");
}

void Blink::loop() {
  if (on && millis() >= onUntil) {
    digitalWrite(pin, activeLow);
    on = false;
    log.print("B: Pin ");
    log.print(pin);
    log.println(" is off.");
  }
}
