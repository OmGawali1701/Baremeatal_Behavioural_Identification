#pragma once

#include "esp_err.h"

#define DEVICE_ID_MAX_LEN       32
#define FW_VERSION_MAX_LEN      16
#define CHIP_MODEL_MAX_LEN      16

typedef struct {

    char device_id[DEVICE_ID_MAX_LEN];
    char firmware_version[FW_VERSION_MAX_LEN];
    char chip_model[CHIP_MODEL_MAX_LEN];

    uint32_t cpu_freq_mhz;
    uint32_t flash_size_mb;
    uint32_t core_count;

} static_data_t;


/* Collect static device information */
esp_err_t static_data_collect(static_data_t *out);