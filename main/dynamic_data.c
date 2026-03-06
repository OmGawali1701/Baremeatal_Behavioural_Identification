#include "dynamic_data.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_timer.h"
#include "esp_heap_caps.h"

#define TAG "DYNAMIC_DATA"

/* Collect runtime telemetry */
esp_err_t dynamic_data_collect(dynamic_data_t *out)
{
    if (!out)
        return ESP_ERR_INVALID_ARG;

    /* Clear counts */
    out->sensor_count = 0;
    out->health_count = 0;

    /* --------------------------
       1 Collect sensor data
       -------------------------- */
    if (sensor_manager_collect(out->sensor_kv,
                               &out->sensor_count) != ESP_OK) {
        ESP_LOGW(TAG, "Sensor collection failed");
    }

    /* --------------------------
       2️ Collect health data
       -------------------------- */

    int64_t uptime_us = esp_timer_get_time();
    uint32_t uptime_sec = uptime_us / 1000000;

    uint32_t free_heap = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);

    wifi_ap_record_t ap_info;
    int8_t rssi = 0;

    if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
        rssi = ap_info.rssi;
    }

    /* Fill health kv */
    int idx = 0;

	if (idx < MAX_HEALTH_PARAMS) {
    out->health_kv[idx++] = (sensor_kv_t){
        .key = "uptime_sec",
        .value = (float)uptime_sec,
        .unit = "sec"
    };

    out->health_kv[idx++] = (sensor_kv_t){
        .key = "heap_free_bytes",
        .value = (float)free_heap,
        .unit = "bytes"
    };

    out->health_kv[idx++] = (sensor_kv_t){
        .key = "wifi_rssi",
        .value = (float)rssi,
        .unit = "dBm"
    };
    }

    out->health_count = idx;

    return ESP_OK;
}