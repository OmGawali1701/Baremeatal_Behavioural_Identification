#pragma once

#include <stdbool.h>

#define MQTT_URI "mqtt://broker.emqx.io:1883"

#define MQTT_TOPIC "device/BM/data"

/* Initialize MQTT client */
void mqtt_init(void);

/* Publish data safely (only if connected) */
bool mqtt_publish(const char *topic, const char *payload);

/* Query connection status */
bool mqtt_is_connected(void);
