#include "json_builder.h"
#include "static_data.h"
#include "dynamic_data.h"

#include "esp_log.h"
#include "cJSON.h"
#include <stdlib.h>

#define TAG "JSON_BUILDER"

char *build_device_json(void)
{
    static_data_t sdata;
    dynamic_data_t ddata;

    /* ---------- Collect Static ---------- */
    if (static_data_collect(&sdata) != ESP_OK) {
        ESP_LOGE(TAG, "Static data collection failed");
        return NULL;
    }

    /* ---------- Collect Dynamic ---------- */
    if (dynamic_data_collect(&ddata) != ESP_OK) {
        ESP_LOGE(TAG, "Dynamic data collection failed");
        return NULL;
    }

    if (ddata.sensor_count == 0) {
        ESP_LOGW(TAG, "No sensor data collected");
        return NULL;
    }

    /* ---------- Create Root ---------- */
    cJSON *root = cJSON_CreateObject();
    if (!root)
        return NULL;

    /* ================= STATIC ================= */

    cJSON *static_obj = cJSON_AddObjectToObject(root, "static");

    cJSON_AddStringToObject(static_obj, "device_id", sdata.device_id);
    cJSON_AddStringToObject(static_obj, "firmware", sdata.firmware_version);
    cJSON_AddStringToObject(static_obj, "chip", sdata.chip_model);
    cJSON_AddNumberToObject(static_obj, "cpu_mhz", sdata.cpu_freq_mhz);
    cJSON_AddNumberToObject(static_obj, "flash_mb", sdata.flash_size_mb);
    cJSON_AddNumberToObject(static_obj, "cores", sdata.core_count);

    /* ================= DYNAMIC ================= */

    cJSON *dynamic_obj = cJSON_AddObjectToObject(root, "dynamic");

    cJSON_AddNumberToObject(dynamic_obj,
                            "sensor_count",
                            ddata.sensor_count);
                            
	 /* ================= SENSOR DATA ================= */
 
    cJSON *sensor_array = cJSON_AddArrayToObject(dynamic_obj, "sensors");

    for (int i = 0; i < ddata.sensor_count; i++) {

        cJSON *sensor_obj = cJSON_CreateObject();

        cJSON_AddStringToObject(sensor_obj,
                                "key",
                                ddata.sensor_kv[i].key);

        cJSON_AddNumberToObject(sensor_obj,
                                "value",
                                ddata.sensor_kv[i].value);
		if (ddata.sensor_kv[i].unit)
    cJSON_AddStringToObject(sensor_obj,
                            "unit",
                            ddata.sensor_kv[i].unit);

        cJSON_AddItemToArray(sensor_array, sensor_obj);
    }
    
     /* ================= HEALTH DATA ================= */
	
		cJSON *health_array = cJSON_AddArrayToObject(dynamic_obj, "health");

for (int i = 0; i < ddata.health_count; i++) {

    cJSON *health_obj = cJSON_CreateObject();

    cJSON_AddStringToObject(health_obj,
                            "key",
                            ddata.health_kv[i].key);

    cJSON_AddNumberToObject(health_obj,
                            "value",
                            ddata.health_kv[i].value);

    if (ddata.health_kv[i].unit)
        cJSON_AddStringToObject(health_obj,
                                "unit",
                                ddata.health_kv[i].unit);

    cJSON_AddItemToArray(health_array, health_obj);
}

    /* ================= PORT DATA ================= */
    
cJSON *ports = cJSON_CreateArray();

for (int i = 0; i < ddata.port_count; i++)
{
    cJSON *port = cJSON_CreateObject();

    cJSON_AddStringToObject(port, "name", ddata.ports[i].name);
    cJSON_AddBoolToObject(port, "active", ddata.ports[i].active);
    cJSON_AddNumberToObject(port, "tx", ddata.ports[i].tx_count);
    cJSON_AddNumberToObject(port, "rx", ddata.ports[i].rx_count);

    cJSON_AddItemToArray(ports, port);
}

cJSON_AddItemToObject(dynamic_obj, "ports", ports);


    /* ================= SYSTEM META ================= */

    cJSON_AddStringToObject(root, "status", "ok");

    /* ---------- Finalize ---------- */
    
    if (!static_obj || !dynamic_obj) {
    cJSON_Delete(root);
    return NULL;
}

    char *json_string = cJSON_PrintUnformatted(root);

    cJSON_Delete(root);

    if (!json_string) {
        ESP_LOGE(TAG, "JSON build failed");
        return NULL;
    }

    return json_string;  // caller must free()
}