
#ifndef __Radiation_h__
#define __Radiation_h__

#include <Arduino.h>

class Radiation {
  public:
    Radiation(Stream &s) : log(s) {};

    void isr();
    bool setup();
    void loop();

  private:
    volatile int count = 0;
    Stream &log;

};

#endif
