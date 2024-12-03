#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

// ADC configuration
#define ADC_SAMPLES     64
#define ADC_ATTEN      ADC_ATTEN_DB_12
#define ADC_WIDTH      ADC_WIDTH_BIT_12
#define ADC_UNIT       ADC_UNIT_1

struct SensorConfig {
    std::string id;
    adc1_channel_t adc_channel;
    gpio_num_t gpio_pin;
    bool is_analog;
};

// Add your sensors here
const SensorConfig SENSOR_CONFIGS[] = {
    {"analog_1", ADC1_CHANNEL_0, GPIO_NUM_36, true},
    {"analog_2", ADC1_CHANNEL_3, GPIO_NUM_39, true},
    {"digital_1", ADC1_CHANNEL_MAX, GPIO_NUM_5, false}
};

esp_err_t sensor_manager_init() {
    for (const auto& sensor : SENSOR_CONFIGS) {
        if (sensor.is_analog) {
            ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH));
            ESP_ERROR_CHECK(adc1_config_channel_atten(sensor.adc_channel, ADC_ATTEN));
        } else {
            gpio_config_t io_conf = {};
            io_conf.pin_bit_mask = (1ULL << sensor.gpio_pin);
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
            ESP_ERROR_CHECK(gpio_config(&io_conf));
        }
    }
    return ESP_OK;
}

void update_sensor_readings() {
    for (const auto& sensor : SENSOR_CONFIGS) {
        float value = 0;
        if (sensor.is_analog) {
            // Take multiple samples and average
            int32_t adc_reading = 0;
            for (int i = 0; i < ADC_SAMPLES; i++) {
                adc_reading += adc1_get_raw(sensor.adc_channel);
            }
            adc_reading /= ADC_SAMPLES;
            value = static_cast<float>(adc_reading);
        } else {
            value = gpio_get_level(sensor.gpio_pin);
        }
        addSensorReading(sensor.id, value);
    }
}

#endif // SENSOR_MANAGER_H
