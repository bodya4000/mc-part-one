#pragma once
#include <ESP8266WiFi.h>

class WiFiService {
public:
static void startAP(const char* ssid, const char* password);
static void connectToWiFi();
};
