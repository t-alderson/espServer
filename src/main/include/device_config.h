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
#define TAG_SYSTEM "system"
#define TAG_WIFI   "wifi"
#define TAG_HTTP   "http"
#define TAG_SENSOR "sensor"

// WiFi configuration
#define WIFI_SSID "your_ssid"
#define WIFI_PASS "your_password"
#define WIFI_MAXIMUM_RETRY 5

// GPIO definitions (customize based on your hardware)
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_NUM_2) | (1ULL<<GPIO_NUM_4))
#define GPIO_INPUT_PIN_SEL   ((1ULL<<GPIO_NUM_5) | (1ULL<<GPIO_NUM_18))

// ADC channels (customize based on your hardware)
#define ADC1_CHANNEL_0     ADC1_CHANNEL_0  // GPIO36
#define ADC1_CHANNEL_1     ADC1_CHANNEL_3  // GPIO39

// I2C configuration (customize based on your hardware)
#define I2C_MASTER_SCL_IO    22
#define I2C_MASTER_SDA_IO    21
#define I2C_MASTER_FREQ_HZ   400000

#endif // DEVICE_CONFIG_H
