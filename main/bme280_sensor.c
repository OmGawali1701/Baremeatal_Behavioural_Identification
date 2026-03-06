#include "bme280_sensor.h"
#include "sensor_manager.h"
#include "esp_log.h"

#define TAG "BME280_SENSOR"

static esp_err_t bme280_init(void)
{
    ESP_LOGI(TAG, "BME280 initialized");
    return ESP_OK;
}

static esp_err_t bme280_read(sensor_kv_t *out, int *count)
{
    if (!out || !count)
        return ESP_ERR_INVALID_ARG;

    int idx = 0;

    float temperature = 25.3;
    float humidity = 60.1;
    float pressure = 1013.2;

    out[idx++] = (sensor_kv_t){ "temperature", temperature, "C" };
    out[idx++] = (sensor_kv_t){ "humidity", humidity, "%" };
    out[idx++] = (sensor_kv_t){ "pressure", pressure, "hPa" };

    *count = idx;

    return ESP_OK;
}

static sensor_driver_t bme_driver = {
    .name = "bme280",
    .init = bme280_init,
    .read = bme280_read
};

void bme280_sensor_register(void)
{
    sensor_manager_register(&bme_driver);
}