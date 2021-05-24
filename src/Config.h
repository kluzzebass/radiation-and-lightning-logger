
#ifndef __Config_h__
#define __Config_h__

#include <Arduino.h>
#include <LittleFS.h>

#define JSON_DOC_SIZE 128

#define MAX_HOST_LEN 64
#define MAX_PORT_LEN 6
#define MAX_PREFIX_LEN 32
#define DEFAULT_MQTT_PORT 1883


class Config {
  public:
    Config(Stream &s, String fn) : logger(s), fileName(fn) {};

    char mqttHost[MAX_HOST_LEN];
    int mqttPort = DEFAULT_MQTT_PORT;
    char mqttPrefix[MAX_PREFIX_LEN];

    void setup(bool reset);
    void read();
    void write();

  private:
    Stream &logger;
    String fileName;

    void format();
};

#endif
