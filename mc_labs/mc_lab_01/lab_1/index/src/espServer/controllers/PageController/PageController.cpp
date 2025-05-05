
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "PageController.h"


PageController::PageController(AsyncWebServer& srv, HtmlPageBuilder& htmlBuilder)
  : server(srv), htmlService(htmlBuilder) {}

void PageController::setupRoutes() {
  server.on("/", HTTP_POST, [this](AsyncWebServerRequest *request) {
    request->send(200, "text/html", htmlService.home());
  });

  server.onNotFound([this](AsyncWebServerRequest *request) {
    request->send(404, "text/html", htmlService.notFound());
  });
}
