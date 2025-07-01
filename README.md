# ESP32 MQTT v5 Publisher

This project demonstrates how to connect an ESP32 to Wi-Fi and publish MQTT messages to a broker using the ESP-IDF framework with MQTT v5 support.

## Features

- Connects ESP32 to a Wi-Fi network
- Connects to a public MQTT broker (MQTT v5)
- Publishes a test message to a topic
- Subscribes to a topic and prints received messages
- Reconnects automatically if Wi-Fi or MQTT connection is lost

## Requirements

- ESP32-S3 development board
- PlatformIO (recommended) with ESP-IDF framework
- MQTT broker (public: [HiveMQ](https://www.hivemq.com/public-mqtt-broker/))
- MQTT client for testing ([MQTT Explorer](https://mqtt-explorer.com/))

## Getting Started

### 1. Clone this repository

```sh
git clone https://github.com/zamilaqeel/Publish-Mqtt-Topic-v5.git
cd Publish-Mqtt-Topic-v5
```

### 2. Open with PlatformIO (via VS Code)

- Open the folder in VS Code with the PlatformIO extension installed.

### 3. Configure Wi-Fi and MQTT

Edit `src/main.c` and set your Wi-Fi and MQTT broker details:

```c
strcpy((char*)wifi_config.sta.ssid, "YOUR_WIFI_SSID");
strcpy((char*)wifi_config.sta.password, "YOUR_WIFI_PASSWORD");

// For MQTT broker:
.broker.address.uri = "mqtt://broker.hivemq.com:1883", // or use your broker's IP
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
  pio device monitor --baud 115200 
  ```
- You should see connection status and publish messages.

### 6. Test with MQTT Client

- Use an MQTT client to subscribe to the topic:
  ```
  test/topic
  ```
- Example using `mosquitto_sub`:
  ```
  mosquitto_sub -h broker.hivemq.com -t test/topic -v
  ```

## File Structure

```
Publish-Mqtt-Topic-v5/
├── .gitignore
├── platformio.ini
├── README.md
├── include/
│   └── README
├── lib/
│   └── README
├── src/
│   └── main.c
├── test/
│   └── README
└── .pio/
    └── ...
└── .vscode/
    └── ...
```

## Example Output

```
I (xxxx) WIFI_TEST: Got IP
I (xxxx) WIFI_TEST: MQTT Connected
I (xxxx) WIFI_TEST: MQTT Data: test/topic = Hello from ESP32!
```

## Troubleshooting

- **Not connecting to Wi-Fi:** Check your SSID and password. (use a 2.4GHz Wi-Fi connection)
- **Not connecting to MQTT:** Check broker address, port, and network firewall.
- **No messages in MQTT client:**  
  Make sure you are subscribing to the correct topic and broker.  
  If you see connection failures in the serial monitor, your ESP32 may not be able to resolve the broker’s hostname.  
  To fix this, use the broker’s IP address instead of its hostname:

  1. **Open Command Prompt** on your computer.
  2. Type:
     ```
     ping broker.hivemq.com
     ```
  3. Copy the IP address shown in the output (e.g., `18.159.39.35`).
  4. In your code, set:
     ```c
     .broker.address.uri = "mqtt://18.159.39.35:1883";
     ```

---

**Project by [zamilaqeel](https://github.com/zamilaqeel)**
