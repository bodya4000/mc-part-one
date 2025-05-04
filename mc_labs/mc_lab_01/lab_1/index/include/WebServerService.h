#pragma once
#include <ESPAsyncWebServer.h>
#include <functional>

class WebServerService {
private:
	AsyncWebServer  server;
  std::function<void()> pressCallback;
  std::function<void()> releaseCallback;

public:
  WebServerService();
  void setHandlers(std::function<void()> press, std::function<void()> release);
  void begin();
};
