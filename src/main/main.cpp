#include "device_config.h"
#include "system_state.h"
#include "wifi_manager.h"
#include "sensor_manager.h"
#include "actuator_manager.h"
#include "http_server.h"

SystemState system_state;

static void initialize_system(void) {

    // Initialize WiFi
    init_wifi(WIFI_SSID, WIFI_PASS);

    // Initialize mutex
    system_state.state_mutex = xSemaphoreCreateMutex();
    if (system_state.state_mutex == NULL) {
        ESP_LOGE(TAG_SYSTEM, "Failed to create mutex");
        return;
    }
    
    // Initialize sensors and actuators
    sensor_manager_init();
    actuator_manager_init();
    
    // Start HTTP server
    init_webserver();
}

void sensor_reading_task(void *pvParameters) {
    while (1) {
        if (xSemaphoreTake(system_state.state_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            update_sensor_readings();
            xSemaphoreGive(system_state.state_mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Read sensors every second
    }
}

extern "C" void app_main(void) {
    // Initialize system components
    initialize_system();
    
    // Create sensor reading task
    xTaskCreate(sensor_reading_task, "sensor_task", 4096, NULL, 5, NULL);
    
    ESP_LOGI(TAG_SYSTEM, "System initialized and running");
    
    // Main loop for system monitoring
    while (1) {
        if (!wifi_manager_is_connected()) {
            ESP_LOGI(TAG_WIFI, "WiFi disconnected, attempting reconnection...");
            esp_wifi_connect();
        }
        vTaskDelay(pdMS_TO_TICKS(5000)); // Check system status every 5 seconds
    }
}

