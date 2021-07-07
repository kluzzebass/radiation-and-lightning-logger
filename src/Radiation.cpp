
#include <Radiation.h>

void Radiation::isr() {
  count++;
}

bool Radiation::setup() {
  logger.println(F("R: Radiation detector initialized."));
  return true;
}

void Radiation::loop() {
  int c = count;

  if (c) {
    char msg[RADIATION_JSON_MSG_LEN];
    StaticJsonDocument<RADIATION_JSON_DOC_LEN> doc;

    count -= c;
    logger.println(F("R: Click!"));
    led.blink(50);

    doc["count"] = c;

    serializeJson(doc, msg);

    logger.println(F("R: Reporting radiation."));
    mqtt.publish("radiation", msg);
  }
}
