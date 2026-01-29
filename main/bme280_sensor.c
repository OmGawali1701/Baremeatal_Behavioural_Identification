#include "bme280_sensor.h"
#include "sensor_manager.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "driver/i2c.h"

#include "bme280.h"          // Bosch driver
#include "bme280_defs.h"

#define TAG "BME280_SENSOR"

/* -------- I2C Configuration -------- */
#define I2C_PORT        I2C_NUM_0
#define I2C_SDA_PIN     8
#define I2C_SCL_PIN     9
#define I2C_FREQ_HZ     100000

#define BME280_ADDR     BME280_I2C_ADDR_PRIM   // 0x76

/* -------- Bosch Device -------- */
static struct bme280_dev bme_dev;

/* -------- I2C Low-level helpers -------- */
static int8_t bme280_i2c_read(uint8_t reg, uint8_t *data,
                              uint32_t len, void *intf_ptr)
{
    uint8_t addr = *(uint8_t *)intf_ptr;
    return i2c_master_write_read_device(
        I2C_PORT, addr, &reg, 1, data, len, pdMS_TO_TICKS(100));
}

static int8_t bme280_i2c_write(uint8_t reg, const uint8_t *data,
                               uint32_t len, void *intf_ptr)
{
    uint8_t buf[len + 1];
    buf[0] = reg;
    memcpy(&buf[1], data, len);

    uint8_t addr = *(uint8_t *)intf_ptr;
    return i2c_master_write_to_device(
        I2C_PORT, addr, buf, len + 1, pdMS_TO_TICKS(100));
}

static void bme280_delay_us(uint32_t period, void *intf_ptr)
{
    vTaskDelay(pdMS_TO_TICKS((period + 999) / 1000));
}

/* -------- Init -------- */
static esp_err_t bme280_init_sensor(void)
{
    ESP_LOGI(TAG, "Initializing BME280 sensor");

    /* I2C init */
    i2c_config_t cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ_HZ
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &cfg));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT, cfg.mode, 0, 0, 0));

    /* Bosch driver setup */
    static uint8_t dev_addr = BME280_ADDR;
    bme_dev.intf = BME280_I2C_INTF;
    bme_dev.intf_ptr = &dev_addr;
    bme_dev.read = bme280_i2c_read;
    bme_dev.write = bme280_i2c_write;
    bme_dev.delay_us = bme280_delay_us;

    if (bme280_init(&bme_dev) != BME280_OK) {
        ESP_LOGE(TAG, "BME280 init failed");
        return ESP_FAIL;
    }

    /* Sensor configuration */
struct bme280_settings settings = {
    .osr_h = BME280_OVERSAMPLING_1X,
    .osr_p = BME280_OVERSAMPLING_4X,
    .osr_t = BME280_OVERSAMPLING_2X,
    .filter = BME280_FILTER_COEFF_4
};

uint8_t sel = BME280_CTRL_HUM_MSK |
              BME280_CTRL_PRESS_MSK |
              BME280_CTRL_TEMP_MSK |
              BME280_FILTER_MSK;

bme280_set_sensor_settings(sel, &settings, &bme_dev);
bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &bme_dev);
    ESP_LOGI(TAG, "BME280 initialized successfully");
    return ESP_OK;
}

/* -------- Read -------- */
static esp_err_t bme280_read(sensor_kv_t *out, int *count)
{
    if (!out || !count) return ESP_ERR_INVALID_ARG;

    struct bme280_data data;
    if (bme280_get_sensor_data(BME280_ALL, &data, &bme_dev) != BME280_OK) {
        return ESP_FAIL;
    }

    out[0] = (sensor_kv_t){
        .key = "temperature_c",
        .value = data.temperature,
        .unit = "C"
    };
    out[1] = (sensor_kv_t){
        .key = "humidity_pct",
        .value = data.humidity,
        .unit = "%"
    };
    out[2] = (sensor_kv_t){
        .key = "pressure_hpa",
        .value = data.pressure / 100.0f,
        .unit = "hPa"
    };

    *count = 3;
    return ESP_OK;
}

/* -------- Register -------- */
void bme280_sensor_register(void)
{
    static sensor_driver_t driver = {
        .name = "bme280",
        .init = bme280_init_sensor,
        .read = bme280_read
    };

    sensor_manager_register(&driver);
}
