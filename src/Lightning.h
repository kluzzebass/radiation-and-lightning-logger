
#ifndef __Lightning_h__
#define __Lightning_h__

#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <AS3935SPI.h>
#include <ArduinoJson.h>
#include <MQTT.h>
#include <Blink.h>

#define INDOOR 0x12 
#define OUTDOOR 0xE
#define LIGHTNING_INT 0x08
#define DISTURBER_INT 0x04
#define NOISE_INT 0x01

#define SENSE_INCREASE_INTERVAL 15000
#define LIGHTNING_JSON_MSG_LEN 48
#define LIGHTNING_JSON_DOC_LEN 48


class Lightning {
  public:
    Lightning(Stream &s, Blink &led, MQTT &mqtt, uint8_t cs, uint8_t irq, bool outdoors)
    : logger(s), led(led), mqtt(mqtt), as3935(cs, irq), outdoors(outdoors) {};

    void isr();
    bool setup();
    void loop();

  private:
    volatile bool interrupted = false;
    Stream &logger;
    Blink &led;
    MQTT &mqtt;
    AS3935SPI as3935;
    bool outdoors;
    uint32_t senseAdjLast = 0L;

    void transmitReading(int distance, int energy);

};

#endif
