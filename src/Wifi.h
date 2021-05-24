
#ifndef __Wifi_h__
#define __Wifi_h__

#include <Arduino.h>
#include <Config.h>
#include <WiFiManager.h>

#define AP_NAME "Very Very Frightening"


class Wifi {
  public:
    Wifi(Stream &s, Config &c) : logger(s), cfg(c) {};

    void setup(bool reset);

  private:
    Stream &logger;
    Config &cfg;

};

#endif
