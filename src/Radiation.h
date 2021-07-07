
#ifndef __Radiation_h__
#define __Radiation_h__

#include <Arduino.h>

#include <Blink.h>
#include <MQTT.h>
#include <ArduinoJson.h>

#define RADIATION_JSON_MSG_LEN 32
#define RADIATION_JSON_DOC_LEN 20

class Radiation {
  public:
    Radiation(Stream &s, Blink &led, MQTT &mqtt) : logger(s), led(led), mqtt(mqtt) {};

    void isr();
    bool setup();
    void loop();

  private:
    volatile int count = 0;
    Stream &logger;
    Blink &led;
    MQTT &mqtt;

};

#endif
