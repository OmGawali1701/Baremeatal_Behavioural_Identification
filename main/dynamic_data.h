#pragma once

#include "sensor_manager.h"
#include "esp_err.h"

#define MAX_HEALTH_PARAMS 10

typedef struct {
    sensor_kv_t sensor_kv[MAX_SENSOR_KV];
    int sensor_count;
    sensor_kv_t health_kv[MAX_HEALTH_PARAMS];
    int health_count;
} dynamic_data_t;


/* Collect all runtime telemetry */
esp_err_t dynamic_data_collect(dynamic_data_t *out);