#pragma once

#include "esp_err.h"
#include "sensor_manager.h"

/* AQI-style classification */
typedef enum {
    GAS_LEVEL_CLEAN = 0,
    GAS_LEVEL_LOW,
    GAS_LEVEL_MODERATE,
    GAS_LEVEL_HIGH,
    GAS_LEVEL_SEVERE
} gas_level_t;

/* Public API */
esp_err_t mq_sensor_get_aqi(uint16_t *aqi);
gas_level_t mq_sensor_get_gas_level(void);

/* Sensor registration */
void mq_sensor_register(void);
