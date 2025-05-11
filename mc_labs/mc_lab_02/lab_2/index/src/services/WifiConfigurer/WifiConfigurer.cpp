#include <Arduino.h>
#include "WiFiConfigurer.h"

bool WiFiConfigurer::startAP(const char* ssid, const char* password) {
  bool result = WiFi.softAP(ssid, password);
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());
  return result;
}

bool WiFiConfigurer::connectToWiFi(const char* ssid, const char* password) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);

  unsigned long startAttemptTime = millis();
  const unsigned long timeout = 10000;
  const unsigned long retryInterval = 500;

  Serial.print("Connecting to WiFi");

  WiFi.begin(ssid, password);

  unsigned long lastPrintTime = 0;

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
    unsigned long now = millis();
    if (now - lastPrintTime >= retryInterval) {
      Serial.print(".");
      lastPrintTime = now;
    }
    yield();
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("\nFailed to connect to WiFi.");
    return false;
  }
}
