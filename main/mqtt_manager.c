#include "mqtt_manager.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_event.h"

#define MQTT_URI "mqtt://broker.hivemq.com"

#define TAG "MQTT_MANAGER"

/* MQTT client handle */
static esp_mqtt_client_handle_t mqtt_client = NULL;

/* Connection state flag */
static bool mqtt_connected = false;

/* -----------------------------------------------------------
 * MQTT EVENT HANDLER
 * ----------------------------------------------------------- */
static void mqtt_event_handler(void *handler_args,
                               esp_event_base_t base,
                               int32_t event_id,
                               void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;

    switch (event->event_id) {

    case MQTT_EVENT_CONNECTED:
        mqtt_connected = true;
        ESP_LOGI(TAG, "Connected to MQTT broker");
        break;

    case MQTT_EVENT_DISCONNECTED:
        mqtt_connected = false;
        ESP_LOGW(TAG, "Disconnected from MQTT broker");
        break;

    case MQTT_EVENT_ERROR:
        ESP_LOGE(TAG, "MQTT error occurred");
        break;

    default:
        break;
    }
}

/* -----------------------------------------------------------
 * MQTT INITIALIZATION
 * ----------------------------------------------------------- */
void mqtt_init(void)
{
    esp_mqtt_client_config_t mqtt_cfg =
     {
        .broker.address.uri = MQTT_URI,

        /* Credentials (optional for freemqtt, kept as provided) */
        .credentials.username = "freemqtt",
        .credentials.authentication.password = "public",

        /* Session parameters */
        .session.keepalive = 60,
        .session.disable_clean_session = false,

        /* Network robustness */
        .network.timeout_ms = 15000,
        .network.disable_auto_reconnect = false,

    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    if (mqtt_client == NULL) 
    {
        ESP_LOGE(TAG, "Failed to create MQTT client");
        return;
    }

    /* Register event handler */
    esp_mqtt_client_register_event(
        mqtt_client,
        ESP_EVENT_ANY_ID,
        mqtt_event_handler,
        NULL
    );

    /* Start client */
    esp_mqtt_client_start(mqtt_client);
    ESP_LOGI(TAG, "MQTT client started, connecting...");
}

/* -----------------------------------------------------------
 * PUBLISH DATA (SAFE)
 * ----------------------------------------------------------- */
bool mqtt_publish(const char *topic, const char *payload)
{
    if (!mqtt_client) 
    {
        ESP_LOGE(TAG, "MQTT client not initialized");
        return false;
    }

    if (!mqtt_connected)
    {
        ESP_LOGW(TAG, "trying to connect broker........");
        return false;
    }

    int msg_id = esp_mqtt_client_publish(
        mqtt_client,
        topic,
        payload,
        0,      /* payload length (0 = strlen) */
        1,      /* QoS */
        0       /* retain */
    );

    if (msg_id >= 0) 
    {
        ESP_LOGI(TAG, "data sent");
        return (msg_id >= 0);
    }
    else 
    {
        ESP_LOGE(TAG, "Failed to send data");
        return false;
    }
}

/* -----------------------------------------------------------
 * CONNECTION STATUS QUERY
 * ----------------------------------------------------------- */
bool mqtt_is_connected(void)
{
    return mqtt_connected;
}
