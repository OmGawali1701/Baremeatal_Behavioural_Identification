#include "static_data.h"
#include "esp_system.h"
#include "esp_private/esp_clk.h"
#include "esp_chip_info.h"
#include "esp_mac.h"
#include "esp_flash.h"
#include "esp_log.h"
#include <stdio.h>
#include <string.h>

#define TAG "STATIC_DATA"

/* You can move this to Kconfig later */
#define FIRMWARE_VERSION "1.0.0"

esp_err_t static_data_collect(static_data_t *out)
{
    if (!out)
        return ESP_ERR_INVALID_ARG;

    memset(out, 0, sizeof(static_data_t));

    /* -------------------------
       Device ID (MAC-based)
    -------------------------- */
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);

    snprintf(out->device_id,
             DEVICE_ID_MAX_LEN,
             "%02X%02X%02X%02X%02X%02X",
             mac[0], mac[1], mac[2],
             mac[3], mac[4], mac[5]);

    /* -------------------------
       Firmware version
    -------------------------- */
    strncpy(out->firmware_version,
            FIRMWARE_VERSION,
            FW_VERSION_MAX_LEN - 1);

    /* -------------------------
       Chip information
    -------------------------- */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    switch (chip_info.model) {
        case CHIP_ESP32:
            strncpy(out->chip_model, "ESP32", CHIP_MODEL_MAX_LEN - 1);
            break;
        case CHIP_ESP32S2:
            strncpy(out->chip_model, "ESP32-S2", CHIP_MODEL_MAX_LEN - 1);
            break;
        case CHIP_ESP32S3:
            strncpy(out->chip_model, "ESP32-S3", CHIP_MODEL_MAX_LEN - 1);
            break;
        case CHIP_ESP32C3:
            strncpy(out->chip_model, "ESP32-C3", CHIP_MODEL_MAX_LEN - 1);
            break;
        default:
            strncpy(out->chip_model, "UNKNOWN", CHIP_MODEL_MAX_LEN - 1);
            break;
    }

    out->core_count = chip_info.cores;

    /* -------------------------
       CPU frequency
    -------------------------- */
    out->cpu_freq_mhz = esp_clk_cpu_freq() / 1000000;

    /* -------------------------
       Flash size
    -------------------------- */
    uint32_t flash_size = 0;
    if (esp_flash_get_size(NULL, &flash_size) == ESP_OK) {
        out->flash_size_mb = flash_size / (1024 * 1024);
    }

    ESP_LOGI(TAG, "Static data collected");

    return ESP_OK;
}