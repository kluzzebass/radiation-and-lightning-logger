
#ifndef __Lightning_h__
#define __Lightning_h__

#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <AS3935SPI.h>

#include <Blink.h>

#define INDOOR 0x12 
#define OUTDOOR 0xE
#define LIGHTNING_INT 0x08
#define DISTURBER_INT 0x04
#define NOISE_INT 0x01

#define SENSE_INCREASE_INTERVAL 15000

class Lightning {
  public:
    Lightning(Stream &s, Blink &led, uint8_t cs, uint8_t irq)
    : log(s), led(led), as3935(cs, irq) {};

    void isr();
    bool setup();
    void loop();

  private:
    volatile bool interrupted = false;
    Stream &log;
    Blink &led;
    AS3935SPI as3935;
    uint32_t senseAdjLast = 0L;

};

#endif
