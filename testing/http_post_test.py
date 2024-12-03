import requests

# Replace '172.20.10.5' with your actual ESP32 IP address and '/esp_rx' with your actual endpoint
url = "http://172.20.10.5/esp_rx"
data = "";

headers = {
    'Content-Type': 'application/json'
}

try:
    response = requests.post(url, json=data, headers=headers)
    print(f'Status Code: {response.status_code}')
    print(f'Response: {response.text}')
except requests.exceptions.RequestException as e:
    print(f'Request failed: {e}')
