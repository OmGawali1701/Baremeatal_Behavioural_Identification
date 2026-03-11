#pragma once

#include "esp_err.h"

#define DEVICE_ID_MAX_LEN     32
#define DEVICE_NAME_MAX_LEN   32
#define FW_VERSION_MAX_LEN    32
#define CHIP_MODEL_MAX_LEN    16

typedef struct
{
    char device_id[DEVICE_ID_MAX_LEN];
    char device_name[DEVICE_NAME_MAX_LEN];
    char firmware_version[FW_VERSION_MAX_LEN];

    char chip_model[CHIP_MODEL_MAX_LEN];

    uint32_t core_count;
    uint32_t cpu_freq_mhz;
    uint32_t flash_size_mb;

} static_data_t;

esp_err_t static_data_collect(static_data_t *out);

