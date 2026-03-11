#pragma once

#include "sensor_manager.h"
#include "esp_err.h"
#include "sensor_manager.h"
#include "port_monitor.h"

#define MAX_HEALTH_PARAMS 10

#define MAX_PORTS 5

typedef struct {
    sensor_kv_t sensor_kv[MAX_SENSOR_KV];
    int sensor_count;
    sensor_kv_t health_kv[MAX_HEALTH_PARAMS];
    int health_count;
    port_stat_t ports[MAX_PORTS];
    int port_count;
} dynamic_data_t;


/* Collect all runtime telemetry */
esp_err_t dynamic_data_collect(dynamic_data_t *out);