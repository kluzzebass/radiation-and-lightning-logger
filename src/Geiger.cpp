
#include <Geiger.h>

void Geiger::isr() {
  count++;
}

void Geiger::loop() {
  int c = count;

  if (c) {
    log.println("tick!");
    count -= c;
  }
}
