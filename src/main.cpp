#include <WiFiManager.h>   // Include the WiFiManager library
#include "WiFiCredentials.h"

#define WIFI_TIMEOUT_SECONDS 180
#define LED_PIN 2  // Onboard LED pin (GPIO 2 on ESP32)
#define BUTTON_PIN 0  // A button connected to GPIO 0 for resetting WiFi credentials

WiFiManager wifiManager;

void setup() {
  // Start Serial Monitor for debugging
  Serial.begin(115200);
  delay(1000);  // Short delay for startup stability

  // Set the LED pin as output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Turn off LED initially

  // Set the Button pin as input
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Use a pullup resistor (button pressed = LOW)
  wifiManager.resetSettings(); // Uncomment for deployment

  // Check if the button is pressed to reset WiFi credentials
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Button pressed, resetting WiFi credentials...");
    wifiManager.resetSettings();  // Clear saved WiFi credentials
    delay(1000);  // Wait a bit
    ESP.restart();  // Restart the ESP32 after resetting credentials
  }

  // Try to connect to saved WiFi credentials, or start AP if it fails
  wifiManager.setTimeout(WIFI_TIMEOUT_SECONDS);  // Timeout for WiFi connection attempts
  if (!wifiManager.autoConnect(AP_SSID, AP_PASSWORD)) {
    Serial.println("Failed to connect, starting AP");
    digitalWrite(LED_PIN, LOW);  // Keep LED off in AP mode
    delay(3000);
    ESP.restart();  // Restart ESP if unable to connect
  }

  // If we're here, it means we successfully connected to WiFi
  Serial.println("Connected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  // Print the device's IP address

  digitalWrite(LED_PIN, HIGH);  // Turn on LED when connected to WiFi
}

void loop() {
  // Check WiFi connection status and control the LED accordingly
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_PIN, HIGH);  // Keep LED on when WiFi is connected
    Serial.println("WiFi connected, running...");
  } else {
    digitalWrite(LED_PIN, LOW);  // Turn off LED if not connected
    Serial.println("WiFi not connected");
  }

  delay(5000);  // Delay for 5 seconds between status checks
}
