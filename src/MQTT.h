
#ifndef __MQTT_h__
#define __MQTT_h__

#include <Arduino.h>
#include <Config.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define MQTT_RECONNECT_DELAY 5000
#define MAX_TOPIC_LENGTH 32

class MQTT {
  public:
    MQTT(Stream &s, Config &c) : logger(s), cfg(c), client(wifiClient) {};

    void setup();
    void loop();
    void reconnect();
    void publish(const char *topic, const char *msg);

  private:
    Stream &logger;
    Config &cfg;

    WiFiClient wifiClient;
    PubSubClient client;
    String clientId;
    unsigned long lastConnectionAttempt = 0;

    const char *translateState(int state);
};

#endif
