#pragma once
#include "Color.h"
#include <ESPAsyncWebServer.h>

class AsyncWebServerService {
private:
  AsyncWebServer server;
  AsyncWebSocket ws;

public:
  AsyncWebServerService();
  void begin();
  void cleanupClients();
  void sendColor(Color color);
  void setHandlers();
	void setControlRefs(bool* webHoldFlag);

};
