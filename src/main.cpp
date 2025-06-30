#include <Arduino.h>
#include <WiFi.h>
#include <MQTT.h>


const char* ssid = "AKEELHOME"; // Your Wi-Fi SSID
const char* password = "011265225"; // Your Wi-Fi Password
const char* mqtt_server = "5.196.78.28"; // or your broker IP/domain
const int mqtt_port = 1883; // Default MQTT port is 1883

WiFiClient espClient;  // Create a WiFi client instance
MQTTClient client; // Create an MQTT client instance

void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("esp32client")) {
      Serial.println("connected!");
      client.publish("akeelhome/esp32/test", "Hello from ESP32 with MQTT");
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.lastError());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}

void setupWIFI(){
  // Initialize WiFi connection
  delay(100); // Wait for a moment to ensure the serial connection is established
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("\nWiFi connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);
  setupWIFI();
  client.begin(mqtt_server, mqtt_port, espClient); // Initialize MQTT client with server and port
  client.setOptions(
    60, // Set MQTT options: keep alive interval
    true,  // Set clean session
    1000); // Set reconnect timeout
  connectMQTT();
}

void loop() {
  client.loop();
  if (!client.connected()) {
    connectMQTT();
  }
}
