#pragma once
#include <stdbool.h>


void mqtt_init(void);
bool mqtt_is_connected(void);
bool mqtt_publish(const char *topic, const char *payload);