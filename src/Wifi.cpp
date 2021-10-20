
#include <Wifi.h>

bool shouldSaveConfig = false;

void saveConfigCallback () {
  shouldSaveConfig = true;
}

void Wifi::setup(bool reset) {
  logger.println(F("W: Initializing Wifi."));

  if (reset) {
    logger.println(F("W: Clearing credentials."));
    WiFi.disconnect();
    delay(3000);
    ESP.restart();
    delay(5000);
  }

  WiFiManager wm;
  wm.setSaveConfigCallback(saveConfigCallback);

  char mqttHost[MAX_HOST_LEN];
  char mqttPort[MAX_PORT_LEN];
  char mqttPrefix[MAX_PREFIX_LEN];

  strncpy(mqttHost, cfg.mqttHost, MAX_HOST_LEN);
  snprintf(mqttPort, MAX_PORT_LEN, "%d", cfg.mqttPort);
  strncpy(mqttPrefix, cfg.mqttPrefix, MAX_PREFIX_LEN);

  WiFiManagerParameter customMqttHost("host", "mqtt host", mqttHost, MAX_HOST_LEN - 1);
  WiFiManagerParameter customMqttPort("port", "mqtt port", mqttPort, MAX_PORT_LEN - 1);
  WiFiManagerParameter customMqttPrefix("prefix", "mqtt prefix", mqttPrefix, MAX_PREFIX_LEN - 1);

  wm.addParameter(&customMqttHost);
  wm.addParameter(&customMqttPort);
  wm.addParameter(&customMqttPrefix);

  if (!wm.autoConnect(AP_NAME)) {
    logger.println("W: Failed to connect and hit timeout.");
    delay(3000);
    // if we still have not connected restart and try all over again
    ESP.restart();
    delay(5000);
  }

  logger.println("W: Connected.");

  strcpy(mqttHost, customMqttHost.getValue());
  strcpy(mqttPort, customMqttPort.getValue());
  strcpy(mqttPrefix, customMqttPrefix.getValue());

  if (shouldSaveConfig) {
    logger.println("W: Config has changed");
    strncpy(cfg.mqttHost, mqttHost, MAX_HOST_LEN);
    if (!(cfg.mqttPort = atoi(mqttPort))) cfg.mqttPort = DEFAULT_MQTT_PORT;
    strncpy(cfg.mqttPrefix, mqttPrefix, MAX_PREFIX_LEN);
    cfg.write();
  }

  logger.println("W: Configuration:");

  logger.print("W:   Local IP: ");
  logger.println(WiFi.localIP());
  logger.print("W:   Gateway IP: ");
  logger.println(WiFi.gatewayIP());
  logger.print("W:   Subnet Mask: ");
  logger.println(WiFi.subnetMask());

  logger.print("W:   MQTT Host: ");
  logger.println(cfg.mqttHost);
  logger.print("W:   MQTT Port: ");
  logger.println(cfg.mqttPort);
  logger.print("W:   MQTT Prefix: ");
  logger.println(cfg.mqttPrefix);

  WiFi.printDiag(logger);

  logger.println("W: Wifi initialized.");
}

