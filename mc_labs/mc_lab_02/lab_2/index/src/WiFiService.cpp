#include "../include/WiFiService.h"

void WiFiService::startAP(const char* ssid, const char* password) {
  WiFi.softAP(ssid, password);
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());
}

void WiFiService::connect(const char* ssid, const char* password) {
	WiFi.begin(ssid, password);

	Serial.print("Connecting to WiFi");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println(" connected!");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());
	
}
