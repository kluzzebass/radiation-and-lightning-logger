
#include <Radiation.h>

void Radiation::isr() {
  count++;
}

bool Radiation::setup() {
  log.println("R: Radiation detector initialized.");
  return true;
}

void Radiation::loop() {
  int c = count;

  if (c) {
    count -= c;
    log.println("R: Click!");
    led.blink(50);
  }
}
