#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "mqtt_client.h"

static const char *TAG = "WIFI_TEST"; // Tag for logging

// MQTT event handler
static void mqtt_event_handler(
    void *handler_args, // Pointer to user data, not used here
    esp_event_base_t base, // Event base, e.g., MQTT_EVENT
    int32_t event_id, // Event ID, e.g., MQTT_EVENT_CONNECTED or MQTT_EVENT_DATA
    void *event_data) // Pointer to event data, e.g., esp_mqtt_event_handle_t
    {
        esp_mqtt_event_handle_t event = event_data; // Cast event_data to esp_mqtt_event_handle_t
        // Check if the event is valid
        switch ((esp_mqtt_event_id_t)event_id) {
            // Handle different MQTT events
            case MQTT_EVENT_CONNECTED:
                // Handle MQTT connection event
                ESP_LOGI(TAG, "MQTT Connected");
                esp_mqtt_client_subscribe(event->client, "test/topic", 0);
                esp_mqtt_client_publish(event->client, "test/topic", "Hello from ESP32!", 0, 1, 0);
                break;
            case MQTT_EVENT_DATA:
                // Handle incoming data
                ESP_LOGI(TAG, "MQTT Data: %.*s = %.*s",
                    event->topic_len, event->topic,
                    event->data_len, event->data);
                break;
            default:
                break;
        }
}

// Wi-Fi event handler
static void wifi_event_handler(
    void* arg, // Pointer to user data, not used here
    esp_event_base_t event_base, // Event base, e.g., WIFI_EVENT or IP_EVENT
    int32_t event_id, // Event ID, e.g., WIFI_EVENT_STA_START or IP_EVENT_STA_GOT_IP
    void* event_data) // Pointer to event data, not used here
    {
        if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
            esp_wifi_connect();
        } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
            ESP_LOGI(TAG, "Disconnected, retrying...");
            esp_wifi_connect();
        } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
            ESP_LOGI(TAG, "Got IP");
        }
}

void app_main(void) {
    nvs_flash_init(); // Initialize NVS (Non-Volatile Storage) for Wi-Fi credentials and other settings
    esp_netif_init(); // Initialize the network interface
    esp_event_loop_create_default(); // Create the default event loop
    esp_netif_create_default_wifi_sta(); // Create a default Wi-Fi station interface

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT(); // Initialize Wi-Fi configuration with default settings
    esp_wifi_init(&cfg); // Initialize the Wi-Fi driver with the configuration

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL); // Register the Wi-Fi event handler for all Wi-Fi events
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL); // Register the IP event handler for the STA interface

    wifi_config_t wifi_config = {}; // Create a Wi-Fi configuration structure
    strcpy((char*)wifi_config.sta.ssid, "AKEELHOME"); // Set the SSID of the Wi-Fi network
    strcpy((char*)wifi_config.sta.password, "011265225"); // Set the password for the Wi-Fi network

    esp_wifi_set_mode(WIFI_MODE_STA); // Set the Wi-Fi mode to Station (STA)
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config); // Set the Wi-Fi configuration for the STA interface
    esp_wifi_start(); // Start the Wi-Fi driver

    // MQTT v5 configuration
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://broker.hivemq.com:1883", // MQTT broker URI
        .session.protocol_ver = MQTT_PROTOCOL_V_5, // Set MQTT protocol version to 5
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg); // Initialize the MQTT client with the configuration
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL); // Register the MQTT event handler for all events
    esp_mqtt_client_start(client); // Start the MQTT client

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }
}