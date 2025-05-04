#pragma once
#include <ESP8266WebServer.h>
#include <functional>

class WebServerService {
private:
  ESP8266WebServer server;
  std::function<String()> htmlCallback;
  std::function<void()> pressCallback;
  std::function<void()> releaseCallback;

public:
  WebServerService();
  void setHandlers(std::function<String()> html, std::function<void()> press, std::function<void()> release);
  void begin();
  void handle();
};
