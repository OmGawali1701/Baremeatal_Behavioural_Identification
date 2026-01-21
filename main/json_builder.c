#include "json_builder.h"
#include "static_data.h"
#include "dynamic_data.h"
#include "cJSON.h"

char* build_device_json(void)
{
    cJSON *root = cJSON_CreateObject();

    cJSON_AddItemToObject(root, "static",
                          get_static_data_json());
    cJSON_AddItemToObject(root, "dynamic",
                          get_dynamic_data_json());

    char *json = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json;
}
