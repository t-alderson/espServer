#ifndef DEVICE_CONFIG_H
#define DEVICE_CONFIG_H

// Core includes
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

// ESP-IDF includes
#include "esp_log.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_http_server.h"
#include "nvs_flash.h"
#include "esp_timer.h"

// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

// Driver includes
#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/i2c.h"
#include "driver/ledc.h"

// Logging tags
#define TAG_SETUP     "system setup"
#define TAG_SYSTEM    "system mgr"
#define TAG_WIFI      "wifi mgr"
#define TAG_HTTP      "http mgr"
#define TAG_SENSOR    "sensor mgr"
#define TAG_ACTUATOR  "actuator mgr"

// WiFi configuration
#define WIFI_SSID "your_ssid"
#define WIFI_PASS "your_password"
#define WIFI_MAXIMUM_RETRY 5

// DigitalPin constants
#define HIGH 1
#define LOW 0

// Add Device Specific config
#include "heltec_unofficial.h"

void init_nvs(){
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

#endif // DEVICE_CONFIG_H
