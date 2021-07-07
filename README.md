# radiation-and-lightning-logger
ESP8266 based radiation and lightning logger

## Building the project

The project was developed using Visual Studio Code and PlatformIO,
but can probably be built using PlatformIO alone. Have a look in the
`platformio.ini` file to make sure everything looks ok. Pay special
attention to `upload_port` and `monitor_port` as these will change
between operating systems.

In the `main.cpp` file, the following definitions are found:

```
#define ENABLE_RADIATION
#define ENABLE_LIGHTNING
```

Remove or comment out either of these to disable the respective
functionality. The usefulness of removing both is questionable.

## Hookup guide for Wemos D1 Mini

### RadiationD-v1.1 Radiation Detector

*D1 Mini -> RadiationD*

      5V -> 5V
     GND -> GND
      D2 -> VIN (Yeah, that label makes no sense)

### Lightning Sensor AS3935

*D1 Mini -> AS3935*

     3V3 -> Vdd
     GND -> GND
     GND -> SI
      D8 -> CS
      D7 -> MOSI/SDA
      D6 -> MISO
      D5 -> SCK/SCL
      D1 -> IRQ

## Starting up

When the Wemos D1 Mini is booted, the WiFiManager will go into
configuration mode, setting up an AP with a landing page for
configuring the WiFi network and MQTT server. Once completed,
it will try to connect to the configured WiFi network whenever
it's beeing booted. If reconfiguration is needed, connect pin `D0`
to `GND` and then hit the reset button on the Wemos D1 Mini. This
will force it the WiFiManager back into configuration mode. Remove
the GND short after reconfiguring, otherwise it'll just boot back
into configuration mode again.