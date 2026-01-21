#include "dynamic_data.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "sensor_manager.h"
#include "esp_wifi.h"
#include "esp_netif.h"

cJSON* get_dynamic_data_json(void)
{
    cJSON *obj = cJSON_CreateObject();
    wifi_ap_record_t ap;
    if (esp_wifi_sta_get_ap_info(&ap) == ESP_OK) {
        cJSON_AddNumberToObject(obj, "rssi", ap.rssi);
        cJSON_AddNumberToObject(obj, "channel", ap.primary);
    }

    esp_netif_ip_info_t ip;
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (netif && esp_netif_get_ip_info(netif, &ip) == ESP_OK) {
        char ip_str[16];
        esp_ip4addr_ntoa(&ip.ip, ip_str, sizeof(ip_str));
        cJSON_AddStringToObject(obj, "ip", ip_str);
    }

    cJSON_AddNumberToObject(obj, "free_heap", esp_get_free_heap_size());
    cJSON_AddNumberToObject(obj, "uptime_sec",
                            esp_timer_get_time() / 1000000);
    sensor_manager_collect(obj);
    return obj;
}
