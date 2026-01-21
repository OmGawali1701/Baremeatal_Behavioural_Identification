#include "mq_sensor.h"
#include "sensor_manager.h"

#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"

#define TAG "MQ_SENSOR"

/* -------- ADC Configuration -------- */
#define MQ_ADC_CHANNEL ADC_CHANNEL_3
#define MQ_ADC_UNIT    ADC_UNIT_1
#define ADC_MAX        4095.0f

#define AQI_MAX        500

static adc_oneshot_unit_handle_t adc_handle;

/* -------- Init -------- */
static esp_err_t mq_init(void)
{
    ESP_LOGI(TAG, "Initializing MQ Gas Sensor (ADC One-Shot)");

    adc_oneshot_unit_init_cfg_t unit_cfg = {
        .unit_id = MQ_ADC_UNIT,
    };

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_cfg, &adc_handle));

    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };

    ESP_ERROR_CHECK(adc_oneshot_config_channel(
        adc_handle,
        MQ_ADC_CHANNEL,
        &chan_cfg
    ));

    return ESP_OK;
}

/* -------- ADC → AQI Mapping -------- */
static uint16_t mq_adc_to_aqi(int raw)
{
    float ratio = raw / ADC_MAX;
    uint16_t aqi = (uint16_t)(ratio * AQI_MAX);

    if (aqi > AQI_MAX) aqi = AQI_MAX;
    return aqi;
}

/* -------- Read -------- */
static esp_err_t mq_read(sensor_data_t *out)
{
    if (!out) return ESP_ERR_INVALID_ARG;

    int raw = 0;
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, MQ_ADC_CHANNEL, &raw));

    uint16_t aqi = mq_adc_to_aqi(raw);

    out->sensor_name = "MQ_GAS";
    out->value = (float)aqi;
    out->unit = "AQI";

    ESP_LOGD(TAG, "MQ raw=%d AQI=%u", raw, aqi);

    return ESP_OK;
}

/* -------- Register -------- */
void mq_sensor_register(void)
{
    static sensor_driver_t mq_driver = {
        .name = "MQ_SENSOR",
        .init = mq_init,
        .read = mq_read
    };

    sensor_manager_register(&mq_driver);
}
