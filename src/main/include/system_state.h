#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

struct SensorReading {
    std::string sensor_id;
    float value;
    uint32_t timestamp;
};

struct ActuatorState {
    std::string actuator_id;
    bool is_active;
    int value;
};

struct SystemState {
    SemaphoreHandle_t state_mutex;
    std::vector<SensorReading> sensor_readings;
    std::vector<ActuatorState> actuator_states;
    bool is_connected;
    std::string last_error;
};

extern SystemState system_state;

std::string stateToJson() {
    std::string json = "";
    if (xSemaphoreTake(system_state.state_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        json = "{\"sensors\":[";
        
        for (size_t i = 0; i < system_state.sensor_readings.size(); i++) {
            if (i > 0) json += ",";
            json += "{\"id\":\"" + system_state.sensor_readings[i].sensor_id + "\",";
            json += "\"value\":" + std::to_string(system_state.sensor_readings[i].value) + ",";
            json += "\"timestamp\":" + std::to_string(system_state.sensor_readings[i].timestamp) + "}";
        }
        
        json += "],\"actuators\":[";
        
        for (size_t i = 0; i < system_state.actuator_states.size(); i++) {
            if (i > 0) json += ",";
            json += "{\"id\":\"" + system_state.actuator_states[i].actuator_id + "\",";
            json += "\"active\":" + std::string(system_state.actuator_states[i].is_active ? "true" : "false") + ",";
            json += "\"value\":" + std::to_string(system_state.actuator_states[i].value) + "}";
        }
        
        json += "]}";
        xSemaphoreGive(system_state.state_mutex);
    }
    return json;
}

bool updateActuatorState(const std::string& actuator_id, bool active, int value) {
    if (xSemaphoreTake(system_state.state_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        for (auto& actuator : system_state.actuator_states) {
            if (actuator.actuator_id == actuator_id) {
                actuator.is_active = active;
                actuator.value = value;
                xSemaphoreGive(system_state.state_mutex);
                return true;
            }
        }
        // If actuator not found, add it
        ActuatorState new_state = {actuator_id, active, value};
        system_state.actuator_states.push_back(new_state);
        xSemaphoreGive(system_state.state_mutex);
        return true;
    }
    return false;
}

void addSensorReading(const std::string& sensor_id, float value) {
    if (xSemaphoreTake(system_state.state_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        SensorReading reading = {
            sensor_id,
            value,
            static_cast<uint32_t>(esp_timer_get_time() / 1000)  // timestamp in milliseconds
        };
        
        // Update existing reading or add new one
        bool found = false;
        for (auto& sensor : system_state.sensor_readings) {
            if (sensor.sensor_id == sensor_id) {
                sensor = reading;
                found = true;
                break;
            }
        }
        if (!found) {
            system_state.sensor_readings.push_back(reading);
        }
        xSemaphoreGive(system_state.state_mutex);
    }
}

#endif // SYSTEM_STATE_H
