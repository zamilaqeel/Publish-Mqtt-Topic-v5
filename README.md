# ESP32 MQTT Publisher Example

This project demonstrates how to connect an ESP32 to Wi-Fi and publish MQTT messages to a broker using the [256dpi/MQTT](https://github.com/256dpi/arduino-mqtt) library.

## Features

- Connects ESP32 to a Wi-Fi network
- Connects to a public MQTT broker
- Publishes a test message to a topic
- Reconnects automatically if Wi-Fi or MQTT connection is lost

## Requirements

- ESP32-S3 development board
- PlatformIO (recommended) or Arduino IDE
- MQTT broker (public: [test.mosquitto.org](https://test.mosquitto.org/))
- MQTT client for testing ([MQTT Explorer](https://mqtt-explorer.com/))

## Getting Started

### 1. Clone this repository

```sh
git clone https://github.com/zamilaqeel/Publish-Mqtt-Topic.git
cd Publish-Mqtt-Topic
```

### 2. Open with PlatformIO (via VS Code)

- Open the folder in VS Code with the PlatformIO extension installed.

### 3. Configure Wi-Fi and MQTT

Edit `src/main.cpp` and set your Wi-Fi and MQTT broker details:

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* mqtt_server = "BROKER_IP_OR_HOSTNAME";
const int mqtt_port = 1883;
```

### 4. Build and Upload

- Connect your ESP32 board via USB.
- Click the **PlatformIO: Upload** button or run:
  ```
  pio run --target upload
  ```

### 5. Monitor Serial Output

- Click the **PlatformIO: Monitor** button or run:
  ```
  pio device monitor
  ```
- You should see connection status and publish messages.

### 6. Test with MQTT Client

- Use an MQTT client to subscribe to the topic:
  ```
  akeelhome/esp32/test
  ```
- Example using `mosquitto_sub`:
  ```
  mosquitto_sub -h 5.196.78.28 -t akeelhome/esp32/test -v
  ```

## File Structure

```
Publish-Mqtt-Topic/
├── .gitignore
├── platformio.ini
├── README.md
├── include/
│   └── README
├── lib/
│   └── README
├── src/
│   └── main.cpp
├── test/
│   └── README
└── .pio/                # PlatformIO build system folder (auto-generated, can be ignored)
    └── ...              # Build artifacts and dependencies
└── .vscode/             # VS Code settings (optional)
    └── ...
```

## Example Output

```
Connecting to AKEELHOME
............................................
WiFi connected to AKEELHOME
IP address: 192.168.18.97
Connecting to MQTT...connected!
```

## Troubleshooting

- **Not connecting to Wi-Fi:** Check your SSID and password.
- **Not connecting to MQTT:** Check broker address, port, and network firewall.
- **No messages in MQTT client:**  
  Make sure you are subscribing to the correct topic and broker.  
  If you see connection failures in the serial monitor, your ESP32 may not be able to resolve the broker’s hostname.  
  To fix this, use the broker’s IP address instead of its hostname:

  1. **Open Command Prompt** on your computer.
  2. Type:
     ```
     ping test.mosquitto.org
     ```
  3. Copy the IP address shown in the output (e.g., `5.196.78.28`).
  4. In your code, set:
     ```cpp
     const char* mqtt_server = "5.196.78.28"; // Use the IP you found
     ```
