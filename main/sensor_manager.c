#include "sensor_manager.h"
#include "esp_log.h"

#define TAG "SENSOR_MANAGER"

#include "bme280_sensor.h"
#include "mq_sensor.h"

static sensor_driver_t *registered_sensors[MAX_SENSORS];
static int sensor_count = 0;



/* Register sensor */
esp_err_t sensor_manager_register(sensor_driver_t *driver)
{
    if (!driver || sensor_count >= MAX_SENSORS)
        return ESP_FAIL;

    registered_sensors[sensor_count++] = driver;
    driver->initialized = false;

    ESP_LOGI(TAG, "Registered sensor: %s", driver->name);
    return ESP_OK;
}


/* Available sensors*/

void sensor_manager_register_all(void)
{
    bme280_sensor_register();
    mq_sensor_register();
}

/* Initialize all sensors */
esp_err_t sensor_manager_init_all(void)
{
    for (int i = 0; i < sensor_count; i++) {

        if (registered_sensors[i]->init) {

            if (registered_sensors[i]->init() == ESP_OK) {
                registered_sensors[i]->initialized = true;
                ESP_LOGI(TAG, "%s initialized", registered_sensors[i]->name);
            } else {
                registered_sensors[i]->initialized = false;
                ESP_LOGW(TAG, "%s failed to initialize",
                         registered_sensors[i]->name);
            }
        }
    }

    return ESP_OK;
}


/* Collect all sensor data */
esp_err_t sensor_manager_collect(sensor_kv_t *out, int *total_count)
{
    if (!out || !total_count)
        return ESP_ERR_INVALID_ARG;

    int index = 0;

    for (int i = 0; i < sensor_count; i++) {

        sensor_driver_t *drv = registered_sensors[i];

        if (!drv->initialized || !drv->read)
            continue;

        int local_count = 0;

        if (drv->read(&out[index], &local_count) == ESP_OK) {

            index += local_count;

            if (index >= MAX_SENSOR_KV) {
                ESP_LOGW(TAG, "Sensor KV buffer full");
                break;
            }

        } else {
            ESP_LOGW(TAG, "%s read failed", drv->name);
        }
    }

    *total_count = index;
    return ESP_OK;
}