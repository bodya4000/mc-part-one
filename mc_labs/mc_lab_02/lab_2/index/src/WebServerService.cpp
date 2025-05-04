#include "../include/WebServerService.h"

WebServerService::WebServerService() : server(80) {}

void WebServerService::setHandlers(std::function<String()> html, std::function<void()> press, std::function<void()> release) {
  htmlCallback = html;
  pressCallback = press;
  releaseCallback = release;
}

void WebServerService::begin() {
  server.on("/", HTTP_GET, [this]() {
    server.send(200, "text/html", htmlCallback());
  });

  server.on("/press", HTTP_POST, [this]() {
    if (pressCallback) pressCallback();
    server.send(200, "text/plain", "OK");
  });

  server.on("/press_out", HTTP_POST, [this]() {
    if (releaseCallback) releaseCallback();
    server.send(200, "text/plain", "OK");
  });

  server.begin();
}

void WebServerService::handle() {
  server.handleClient();
}
