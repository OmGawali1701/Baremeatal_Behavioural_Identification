#pragma once

#include "esp_err.h"
#include <stdbool.h>

#define MAX_SENSORS     8
#define MAX_SENSOR_KV   32

typedef struct
{
    const char *key;
    float value;
    const char *unit;

} sensor_kv_t;


typedef struct {

    const char *name;

    esp_err_t (*init)(void);
    esp_err_t (*read)(sensor_kv_t *kv, int *count);

    bool initialized;

} sensor_driver_t;


/* Core API */
esp_err_t sensor_manager_register(sensor_driver_t *driver);
void sensor_manager_register_all(void);
esp_err_t sensor_manager_init_all(void);
esp_err_t sensor_manager_collect(sensor_kv_t *out, int *total_count);