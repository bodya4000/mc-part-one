#include "../include/WiFiService.h"

void WiFiService::startAP(const char* ssid, const char* password) {
  WiFi.softAP(ssid, password);
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());
}

void WiFiService::connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(100);
  WiFi.begin("TP-Link_D9D2", "28824107");

  Serial.print("Connecting to WiFi");

  unsigned long startAttemptTime = millis();
  const unsigned long timeout = 10000;

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
    Serial.print(".");
    yield();
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi.");
  }
}

