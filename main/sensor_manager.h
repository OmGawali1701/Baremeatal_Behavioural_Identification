#pragma once

#include "esp_err.h"
#include "cJSON.h"

/* -------- Sensor Data Container -------- */
typedef struct {
    const char *key;
    float value;
    const char *unit;
} sensor_kv_t;

/* -------- Sensor Driver Interface -------- */
typedef struct {
    const char *name;   // "bme280", "mq_sensor"
    esp_err_t (*init)(void);

    /* Sensor fills an array of key-value pairs */
    esp_err_t (*read)(sensor_kv_t *out, int *count);
} sensor_driver_t;


/* -------- Sensor Manager API -------- */
void sensor_manager_init(void);
void sensor_manager_register(sensor_driver_t *driver);
void sensor_manager_collect(cJSON *dynamic_obj);
