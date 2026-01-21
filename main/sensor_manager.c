#include "sensor_manager.h"
#include "esp_log.h"

#define MAX_SENSORS 8
static const char *TAG = "SENSOR_MGR";

static sensor_driver_t *sensor_list[MAX_SENSORS];
static int sensor_count = 0;

void sensor_manager_register(sensor_driver_t *driver)
{
    if (sensor_count < MAX_SENSORS) {
        sensor_list[sensor_count++] = driver;
        ESP_LOGI(TAG, "Registered sensor: %s", driver->name);
    }
}

void sensor_manager_init(void)
{
    for (int i = 0; i < sensor_count; i++) {
        if (sensor_list[i]->init) {
            sensor_list[i]->init();
        }
    }
}

void sensor_manager_collect(cJSON *dynamic_obj)
{
    for (int i = 0; i < sensor_count; i++) {
        sensor_data_t data;


        if (sensor_list[i]->read &&
            sensor_list[i]->read(&data) == ESP_OK) {

            cJSON_AddNumberToObject(dynamic_obj,
                                    data.sensor_name,
                                    data.value);
        }
    }
}
