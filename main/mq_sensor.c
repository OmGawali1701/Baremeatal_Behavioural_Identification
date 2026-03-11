#include "mq_sensor.h"
#include "sensor_manager.h"

#include "driver/adc.h"
#include "esp_log.h"
#include "sdkconfig.h"

#define TAG "MQ_SENSOR"

static bool initialized = false;


/* ---------- Sensor Init ---------- */
static esp_err_t mq_init(void)
{
    if (initialized)
        return ESP_OK;

    adc1_config_width(ADC_WIDTH_BIT_12);

    adc1_config_channel_atten(
        CONFIG_MQ135_ADC_CHANNEL,
        ADC_ATTEN_DB_11
    );

    ESP_LOGI(TAG, "MQ135 initialized (ADC channel %d)",
             CONFIG_MQ135_ADC_CHANNEL);

    initialized = true;

    return ESP_OK;
}


/* ---------- Sensor Read ---------- */
static esp_err_t mq_read(sensor_kv_t *out, int *count)
{
    if (!out || !count)
        return ESP_ERR_INVALID_ARG;

    int idx = 0;

    int raw = adc1_get_raw(CONFIG_MQ135_ADC_CHANNEL);

    float aqi = (float)raw;   /* Placeholder conversion */

    out[idx++] = (sensor_kv_t){
        .key = "air_quality",
        .value = aqi,
        .unit = "raw"
    };

    *count = idx;

    return ESP_OK;
}


/* ---------- Driver Struct ---------- */

static sensor_driver_t mq_driver = {
    .name = "mq135",
    .init = mq_init,
    .read = mq_read
};


/* ---------- Register Sensor ---------- */

void mq_sensor_register(void)
{
    sensor_manager_register(&mq_driver);
}