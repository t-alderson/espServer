idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "./include"
                    REQUIRES RadioLib ssd1306 esp_wifi esp_http_server esp_event nvs_flash esp_timer esp_adc driver)
