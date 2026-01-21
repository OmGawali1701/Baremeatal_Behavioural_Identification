#include "static_data.h"

#include "esp_system.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_mac.h"
#include "esp_log.h"

#define TAG "STATIC_DATA"

cJSON *get_static_data_json(void)
{
    cJSON *obj = cJSON_CreateObject();
    if (!obj) {
        ESP_LOGE(TAG, "Failed to create static JSON object");
        return NULL;
    }

    /* ---------- Chip information ---------- */
    esp_chip_info_t chip;
    esp_chip_info(&chip);

    cJSON_AddStringToObject(obj, "chip_model", "ESP32-S3");
    cJSON_AddNumberToObject(obj, "chip_revision", chip.revision);
    cJSON_AddNumberToObject(obj, "cores", chip.cores);

    /* ---------- Flash size ---------- */
    uint32_t flash_size = 0;
    if (esp_flash_get_size(NULL, &flash_size) == ESP_OK) {
        cJSON_AddNumberToObject(obj, "flash_size_kb", flash_size / 1024);
    }

    /* ---------- MAC address (identity anchor) ---------- */
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);

    char mac_str[18];
    snprintf(mac_str, sizeof(mac_str),
             "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2],
             mac[3], mac[4], mac[5]);

    cJSON_AddStringToObject(obj, "mac_address", mac_str);

    /* ---------- ESP-IDF version ---------- */
    cJSON_AddStringToObject(obj, "idf_version", esp_get_idf_version());

    return obj;
}
