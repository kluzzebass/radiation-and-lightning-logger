
#include <Config.h>
#include <ArduinoJson.h> 

void Config::setup(bool reset) {
      if (reset) format();

      memset(mqttHost, 0, MAX_HOST_LEN);
      memset(mqttPrefix, 0, MAX_PREFIX_LEN);

      logger.println(F("C: Opening file system."));
      if (!LittleFS.begin()) {
        logger.println(F("C: Failed."));
        return;
      }
      logger.println(F("C: Success."));
}

void Config::format() {
  logger.println(F("C: Formatting file system."));
  LittleFS.format();
}

void Config::read() {
  logger.println(F("C: Opening config file for reading."));
  File f = LittleFS.open(fileName, "r");
  if (!f) {
    logger.println(F("C: Failed. Using defaults."));
    return;
  }

  StaticJsonDocument<JSON_DOC_SIZE> doc;
  DeserializationError error = deserializeJson(doc, f);

  f.close();

  if (error) {
    logger.print(F("C: deserializeJson() failed: "));
    logger.println(error.f_str());
    return;
  }

  if (doc["mqttHost"]) strncpy(mqttHost, doc["mqttHost"], MAX_HOST_LEN);
  if (doc["mqttPort"]) mqttPort = doc["mqttPort"];
  if (doc["mqttPrefix"]) strncpy(mqttPrefix, doc["mqttPrefix"], MAX_PREFIX_LEN);

  logger.println(F("C: Done."));
}

void Config::write() {
  logger.println(F("C: Opening config file for writing."));
  File f = LittleFS.open(fileName, "w");
  if (!f) {
    logger.println(F("C: Failed."));
    return;
  }

  StaticJsonDocument<JSON_DOC_SIZE> doc;

  doc["mqttHost"] = mqttHost;
  doc["mqttPort"] = mqttPort;
  doc["mqttPrefix"] = mqttPrefix;

  if (serializeJson(doc, f) == 0) {
    Serial.println(F("C: Failed to write to file"));
  }

  f.close();
  logger.println(F("C: Done."));
}


