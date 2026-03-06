#include "mq_sensor.h"
#include "sensor_manager.h"
#include "esp_log.h"

#define TAG "MQ_SENSOR"

static esp_err_t mq_init(void)
{
    ESP_LOGI(TAG, "MQ135 initialized");
    return ESP_OK;
}

static esp_err_t mq_read(sensor_kv_t *out, int *count)
{
    if (!out || !count)
        return ESP_ERR_INVALID_ARG;

    int idx = 0;

    float aqi = 82.4;

    out[idx++] = (sensor_kv_t){ "air_quality", aqi, "AQI" };

    *count = idx;

    return ESP_OK;
}

static sensor_driver_t mq_driver = {
    .name = "mq135",
    .init = mq_init,
    .read = mq_read
};

void mq_sensor_register(void)
{
    sensor_manager_register(&mq_driver);
}