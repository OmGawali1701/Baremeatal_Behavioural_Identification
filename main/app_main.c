#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "wifi_manager.h"
#include "mqtt_manager.h"

#include "json_builder.h"
#include "sensor_manager.h"

#include "sdkconfig.h"

#define TAG "APP_MAIN"

static void publish_task(void *arg)
{
    while (1)
    {
        if (!mqtt_is_connected())
        {
            ESP_LOGW(TAG, "MQTT offline");
            vTaskDelay(pdMS_TO_TICKS(CONFIG_PUBLISH_INTERVAL_MS));
            continue;
        }

        char *json_payload = build_device_json();

        if (json_payload)
        {
            if (mqtt_publish(CONFIG_MQTT_TOPIC, json_payload))
            {
                ESP_LOGI(TAG, "Telemetry sent");
            }
            else
            {
                ESP_LOGE(TAG, "MQTT publish failed");
            }

            /* Always free */
            free(json_payload);
        }

        vTaskDelay(pdMS_TO_TICKS(CONFIG_PUBLISH_INTERVAL_MS));
    }
}

void app_main(void)
{
    /* ---------- NVS ---------- */

    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    /* ---------- WiFi ---------- */

    wifi_init_sta();

    /* Wait for connection */
    while (!wifi_is_connected())
    {
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    /* ---------- MQTT ---------- */

    mqtt_init();

    /* ---------- Sensors ---------- */

    sensor_manager_register_all();
    sensor_manager_init_all();

    /* ---------- Telemetry Task ---------- */

    xTaskCreate(
        publish_task,
        "telemetry_task",
        8192,
        NULL,
        5,
        NULL
    );

    ESP_LOGI(TAG, "System initialized");
}