# ESP Server Project Overview

## Purpose

ESP-IDF ESP32 Server is designed to communicate over HTTP requests (GET and POST) to control actuators and read sensor data, and be adaptable to most ESP32 hardware.

## Features

- **HTTP Server**: The server listens for incoming requests on two endpoints: `/esp_rx` for receiving commands from the host computer, and `/esp_tx` for sending data to the host computer.
- **Sensor Manager**: The sensor manager is used to read analog and digital sensors, and average out the readings.
- **Actuator Manager**: The actuator manager is used to control digital and PWM actuators.
- **System State**: The system state is used to store the current state of the system, including sensor readings and actuator states.

## Notes

- The system state is protected by a mutex to ensure thread safety.
- The HTTP server is designed to be as lightweight as possible to allow for fast response times.
- The sensor manager and actuator manager are designed to be as flexible as possible to allow for easy expansion.

## TODO

- Refactor the libraries in the `include` folder to be linked into the `main.cpp` file at compile time.
- Refactor system state and mutex system to be more efficient and easier to use.
- Refactor HTTP server handling of requests to be more efficient.
- Test with real sensors and actuators.
- Implement JSON configuration file support for dynamic system settings.
- Add WebSocket support for real-time communication between server and client.
- Improve real-time performance of the system.
  - Increase accuracy of sensor readings and actuator control.
- Improve error handling and logging system with more granular levels.
- Develop unit tests for core modules (sensor manager, actuator manager, HTTP server).
- Optimize sensor and actuator configuration parsing for easier hardware integration.


## Contributors

- Ported from [https://github.com/mcassar4/SmartGarden.git](https://github.com/mcassar4/SmartGarden.git)
  - credit to Manny Cassar for the original project
