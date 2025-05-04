#include "../include/WebServerService.h"
#include "../include/HtmlPageService.h" 
#include <LittleFS.h>

HtmlPageService htmlService;

WebServerService::WebServerService() : server(80) {}

void WebServerService::setHandlers(
  std::function<void()> press,
  std::function<void()> release
) {
  pressCallback = std::move(press);
  releaseCallback = std::move(release);
}


void WebServerService::begin() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", htmlService.home());
  });

  server.on("/press", HTTP_POST, [this](AsyncWebServerRequest *request) {
    if (pressCallback) pressCallback();
    request->send(200, "text/plain", "OK");
  });

  server.on("/press_out", HTTP_POST, [this](AsyncWebServerRequest *request) {
    if (releaseCallback) releaseCallback();
    request->send(200, "text/plain", "OK");
  });

  server.onNotFound([](AsyncWebServerRequest *request) {
    request->send(404, "text/html", htmlService.notFound());
  });

  server.begin();
  Serial.println("Async Web server started");
}
