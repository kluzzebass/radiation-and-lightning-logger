
#ifndef __Radiation_h__
#define __Radiation_h__

#include <Arduino.h>

#include <Blink.h>

class Radiation {
  public:
    Radiation(Stream &s, Blink &led) : log(s), led(led) {};

    void isr();
    bool setup();
    void loop();

  private:
    volatile int count = 0;
    Stream &log;
    Blink &led;

};

#endif
