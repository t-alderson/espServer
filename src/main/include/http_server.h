#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif
const int RX_TX_BUFF_LEN = 100; // Message standard will be 100 bytes



/* Handler function for getting specific actuator state 

recv format: actuator_id(string),state(1/0),channel(0-255)
resp format: "success" or "error"
*/
esp_err_t rx_handler(httpd_req_t *req) {
    esp_err_t ret = ESP_OK;
    std::string resp = "";
    char rx_buf[RX_TX_BUFF_LEN]; // buffer for receiving data
    std::string command;
    std::string actuator_id;
    bool state = false;
    uint8_t channel = 0;

    // Receive the data
    int rx = httpd_req_recv(req, rx_buf, MIN(req->content_len, sizeof(rx_buf) - 1));

    if (rx <= 0) {
        if (rx == HTTPD_SOCK_ERR_TIMEOUT) {
            httpd_resp_send_408(req);
        }
        resp = "error";
        ret = ESP_FAIL;
        goto end;
    }

    rx_buf[rx] = 0; // add null terminator
    ESP_LOGI(TAG_HTTP, "Received: %s", rx_buf);

    // Process the command here (e.g., enable actuator(s))
    command = rx_buf;
    actuator_id = command.substr(0, command.find(','));
    if (actuator_id.empty()) {
        ESP_LOGI(TAG_HTTP, "No actuator id provided");
        resp = "error";
        ret = ESP_FAIL;
        goto end;
    }

    // Simple check for state and channel
    state = command.substr(command.find(',') + 1, 1) == "1";
    channel = std::stoi(command.substr(command.find(',') + 2, command.length() - 1));

    if (xSemaphoreTake(system_state.state_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        if (set_actuator_state(actuator_id, state, channel)) {
            resp = "success";
        } else {
            ESP_LOGI(TAG_HTTP, "Failed to set actuator state");
            resp = "error";
            ret = ESP_FAIL;
        }
        xSemaphoreGive(system_state.state_mutex);
    }
end:
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_send(req, resp.c_str(), HTTPD_RESP_USE_STRLEN);
    ESP_LOGI(TAG_HTTP, "Sent Receipt Confirmation: %s", resp.c_str());

    return ret;
}


/* Handler function for sending state data */
esp_err_t tx_handler(httpd_req_t *req) {
    // Get the system state json
    std::string json_state = stateToJson();

    // Send the data
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, json_state.c_str(), HTTPD_RESP_USE_STRLEN);
    ESP_LOGI(TAG_HTTP, "Sent: %s", json_state.c_str());
    return ESP_OK;
}

/* Function to start the web server */
httpd_handle_t start_webserver(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    // One URI for receiving, one for sending
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t rx_uri = {
            .uri = "/esp_rx",
            .method = HTTP_POST,
            .handler = rx_handler,
            .user_ctx = NULL
        };

        httpd_uri_t tx_uri = {
            .uri = "/esp_tx",
            .method = HTTP_GET,
            .handler = tx_handler,
            .user_ctx = NULL
        };

        httpd_register_uri_handler(server, &rx_uri);
        httpd_register_uri_handler(server, &tx_uri);
    }

    return server;
}

/* Function to stop the web server */
void stop_webserver(httpd_handle_t &server) {
    if (server) {
        httpd_stop(server);
    }
}

#endif // HTTP_SERVER_H
