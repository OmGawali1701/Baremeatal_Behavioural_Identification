#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_err.h"

#include "wifi_manager.h"
#include "mqtt_manager.h"
#include "uart_checker.h"

#include "json_builder.h"
#include "sensor_manager.h"
#include "mq_sensor.h"
#include "bme280_sensor.h"


#define TAG "APP_MAIN"
#define MQTT_TOPIC "device/BM/data"
#define PUBLISH_INTERVAL_MS 5000

static void publish_task(void *arg)
{
    while (1) {

        char *json_payload = build_device_json();
        if (json_payload) {

            /* UART ground-truth mirror */
            uart_checker_print_json(json_payload);

            /* MQTT publish */
if (mqtt_is_connected()) 
{
    if (mqtt_publish(MQTT_TOPIC, json_payload)) 
    {
        ESP_LOGI(TAG, "Behavioral JSON sent");
        free(json_payload);
        
    } else {
        ESP_LOGE(TAG, "MQTT publish failed");
    }
} else {
    ESP_LOGW(TAG, "MQTT offline, JSON not sent");
}
        }

        vTaskDelay(pdMS_TO_TICKS(PUBLISH_INTERVAL_MS));
    }
}

void app_main(void)
{
    /* ---------- NVS (MUST be first) ---------- */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    /* ---------- Debug UART ---------- */
    uart_checker_init();

    /* ---------- Networking ---------- */
    wifi_init_sta();     // async connect
    
    while (!wifi_is_connected()) 
    {
    vTaskDelay(pdMS_TO_TICKS(500));
}
    mqtt_init();         // async connect

    /* ---------- Sensors ---------- */
    
    mq_sensor_register();
    bme280_sensor_register();
    sensor_manager_init();

    /* ---------- Publish Task ---------- */
    xTaskCreate(
        publish_task,
        "publish_task",
        8192,
        NULL,
        5,
        NULL
    );

    ESP_LOGI(TAG, "Application started");
}
