
#include <Geiger.h>

void Geiger::isr() {
  count++;
}

bool Geiger::setup() {
  log.println("G: Radiation detector initialized.");
  return true;
}

void Geiger::loop() {
  int c = count;

  if (c) {
    log.println("G: Click!");
    count -= c;
  }
}
