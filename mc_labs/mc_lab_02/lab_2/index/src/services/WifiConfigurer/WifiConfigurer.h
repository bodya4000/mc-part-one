#pragma once
#include <ESP8266WiFi.h>

class WiFiConfigurer {
public:
  static bool startAP(const char* ssid = "ESP8266-Access-Point", const char* password = "12345678");
  static bool connectToWiFi(const char* ssid = "TP-Link_D9D2", const char* password = "28824107");
};
