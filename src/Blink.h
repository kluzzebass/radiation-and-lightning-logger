
#ifndef __Blink_h__
#define __Blink_h__

#include <Arduino.h>

class Blink {
  public:
    Blink(Stream &s, uint8_t p) : logger(s), pin(p), activeLow(false) {};
    Blink(Stream &s, uint8_t p, bool activeLow) : logger(s), pin(p), activeLow(activeLow) {};

    void setup();
    void loop();
    void blink(unsigned int duration);

  private:
    bool on = false;
    unsigned long onUntil = 0;
    Stream &logger;
    uint8_t pin;
    bool activeLow;

};

#endif
