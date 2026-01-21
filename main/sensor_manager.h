#pragma once

#include "esp_err.h"
#include "cJSON.h"

/* -------- Sensor Data Container -------- */
typedef struct {
    const char *sensor_name;
    float value;
    const char *unit;
} sensor_data_t;

/* -------- Sensor Driver Interface -------- */
typedef struct {
    const char *name;
    esp_err_t (*init)(void);
    esp_err_t (*read)(sensor_data_t *out);
} sensor_driver_t;

/* -------- Sensor Manager API -------- */
void sensor_manager_init(void);
void sensor_manager_register(sensor_driver_t *driver);
void sensor_manager_collect(cJSON *dynamic_obj);
