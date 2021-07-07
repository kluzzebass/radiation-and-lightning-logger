
#ifndef __Uptime_h__
#define __Uptime_h__

#include <Arduino.h>
#include <MQTT.h>
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>

#define UPTIME_JSON_DOC_LEN 16
#define UPTIME_JSON_MSG_LEN 34

class Uptime {
  public:
    Uptime(Stream &s, MQTT &mqtt, uint32_t interval) : logger(s), mqtt(mqtt), interval(interval) {};

    void setup();
    void loop();

  private:
    Stream &logger;
    MQTT &mqtt;

    uint64_t uptime;
    uint32_t lastMillis;

    uint32_t interval;
    uint32_t lastPublish;

    void update();
    void publish();
};

#endif
