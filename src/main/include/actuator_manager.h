#ifndef ACTUATOR_MANAGER_H
#define ACTUATOR_MANAGER_H

#include "device_config.h"
#include "system_state.h"

struct ActuatorConfig {
    std::string id;
    gpio_num_t gpio_pin;
    bool is_pwm;
    uint8_t channel;  // PWM channel if applicable
};

// Add your actuators here
const ActuatorConfig ACTUATOR_CONFIGS[] = {
    {"relay_1", GPIO_NUM_2, false, 0},
    {"led_1", GPIO_NUM_4, true, 0},
};

esp_err_t actuator_manager_init() {
    for (const auto& actuator : ACTUATOR_CONFIGS) {
        if (actuator.is_pwm) {
            // Configure PWM
            ledc_timer_config_t ledc_timer = {
                .speed_mode = LEDC_LOW_SPEED_MODE,
                .duty_resolution = LEDC_TIMER_8_BIT,
                .timer_num = LEDC_TIMER_0,
                .freq_hz = 5000,
                .clk_cfg = LEDC_AUTO_CLK
            };
            ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

            ledc_channel_config_t ledc_channel = {
                .gpio_num = actuator.gpio_pin,
                .speed_mode = LEDC_LOW_SPEED_MODE,
                .channel = (ledc_channel_t)actuator.channel,
                .timer_sel = LEDC_TIMER_0,
                .duty = 0,
                .hpoint = 0
            };
            ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
        } else {
            // Configure digital output
            gpio_config_t io_conf = {};
            io_conf.pin_bit_mask = (1ULL << actuator.gpio_pin);
            io_conf.mode = GPIO_MODE_OUTPUT;
            ESP_ERROR_CHECK(gpio_config(&io_conf));
        }
        // Initialize actuator state in system state
        updateActuatorState(actuator.id, false, 0);
    }
    return ESP_OK;
}

bool set_actuator_state(const std::string& actuator_id, bool active, int value) {
    for (const auto& actuator : ACTUATOR_CONFIGS) {
        if (actuator.id == actuator_id) {
            if (actuator.is_pwm) {
                ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, 
                    (ledc_channel_t)actuator.channel, value));
                ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, 
                    (ledc_channel_t)actuator.channel));
            } else {
                gpio_set_level(actuator.gpio_pin, active ? 1 : 0);
            }
            return updateActuatorState(actuator_id, active, value);
        }
    }
    return false;
}

#endif // ACTUATOR_MANAGER_H
