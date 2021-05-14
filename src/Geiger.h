
#ifndef __Geiger_h__
#define __Geiger_h__

#include <Arduino.h>

class Geiger {
  public:
    Geiger(Stream &s) : log(s) {
      count = 0;
    };

    void isr();
    void loop();

  private:
    int count;
    Stream &log;

};

#endif
