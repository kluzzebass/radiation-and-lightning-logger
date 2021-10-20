
#include <MQTT.h>

void MQTT::setup() {
  client.setServer(cfg.mqttHost, cfg.mqttPort);
  clientId = String(F("RadiationAndLightning-")) + String(random(0xffff), HEX);
  logger.println(F("M: MQTT initialized."));
}

void MQTT::loop() {
  if (client.connected()) {
    client.loop();
    return;
  }

  reconnect();
}

void MQTT::reconnect() {
  unsigned long now = millis();
  if (now < lastConnectionAttempt + 5000)
    return;

  lastConnectionAttempt = now;

  logger.println(F("M: Connecting to MQTT host..."));
  if (!client.connect(clientId.c_str())) {
      logger.print("M: Failed, rc = ");
      logger.println(translateState(client.state()));
      return;
  }
  logger.println("M: Connected.");
}

void MQTT::publish(const char *topic, const char *msg) {
  char fullTopic[MAX_PREFIX_LEN + MAX_TOPIC_LENGTH + 3];
  snprintf(fullTopic, MAX_PREFIX_LEN + MAX_TOPIC_LENGTH + 2, "/%s/%s", cfg.mqttPrefix, topic);

  // logger.print("topic: ");
  // logger.println(fullTopic);

  // logger.print("  msg: ");
  // logger.println(msg);

  client.publish(fullTopic, msg);
}

const char *MQTT::translateState(int state) {
  switch (state) {
    case -4:
      return "MQTT_CONNECTION_TIMEOUT";
    case -3:
      return "MQTT_CONNECTION_LOST";
    case -2:
      return "MQTT_CONNECT_FAILED";
    case -1:
      return "MQTT_DISCONNECTED";
    case 0:
      return "MQTT_CONNECTED";
    case 1:
      return "MQTT_CONNECT_BAD_PROTOCOL";
    case 2:
      return "MQTT_CONNECT_BAD_CLIENT_ID";
    case 3:
      return "MQTT_CONNECT_UNAVAILABLE";
    case 4:
      return "MQTT_CONNECT_BAD_CREDENTIALS";
    case 5:
      return "MQTT_CONNECT_UNAUTHORIZED";
    default:
      return "Unknown client state. Time to update the code?";
  }
}