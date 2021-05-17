
#ifndef __Geiger_h__
#define __Geiger_h__

#include <Arduino.h>

class Geiger {
  public:
    Geiger(Stream &s) : log(s) {};

    void isr();
    bool setup();
    void loop();

  private:
    volatile int count = 0;
    Stream &log;

};

#endif
