
#include <Uptime.h>

void Uptime::setup() {
    logger.println(F("U: Uptime initialized."));
}

void Uptime::loop() {
  update();
  publish();
}

void Uptime::update() {
  uint32_t now = millis();
  uint32_t diff = now - lastMillis;
  uptime += diff;
  lastMillis = now;
}

void Uptime::publish() {
  if ((lastPublish + interval) > lastMillis) return;

  lastPublish = lastMillis;
  char msg[UPTIME_JSON_MSG_LEN];
  StaticJsonDocument<UPTIME_JSON_DOC_LEN> doc;


  doc["uptime"] = uptime;
  
  serializeJson(doc, msg);

  logger.println(F("U: Reporting uptime."));
  mqtt.publish("uptime", msg);
}
