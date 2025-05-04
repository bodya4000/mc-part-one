#include "../include/AsyncWebServerService.h"
#include "../include/Color.h"
#include <LittleFS.h>

namespace {
  bool* webHoldPtr = nullptr;
}

AsyncWebServerService::AsyncWebServerService()
  : server(80), ws("/ws") {}

void AsyncWebServerService::begin() {
  LittleFS.begin();
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
  setHandlers(); 
  server.addHandler(&ws);
  server.begin();
}

void AsyncWebServerService::setControlRefs(bool* webHoldFlag) {
  webHoldPtr = webHoldFlag;
}

void AsyncWebServerService::cleanupClients() {
  ws.cleanupClients();
}

void AsyncWebServerService::sendColor(Color color) {
  switch (color) {
    case Color::RED: ws.textAll("red"); break;
    case Color::YELLOW: ws.textAll("yellow"); break;
    case Color::GREEN: ws.textAll("green"); break;
  }
}

void AsyncWebServerService::setHandlers() {
  server.on("/hold", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (webHoldPtr) *webHoldPtr = true;
    request->send_P(200, "text/html", "ok");
  });

  server.on("/release", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (webHoldPtr) *webHoldPtr = false;
    request->send_P(200, "text/html", "ok");
  });

  server.on("/start", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.print("h");
    request->send_P(200, "text/html", "ok");
  });

  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.print("r");
    request->send_P(200, "text/html", "ok");
  });
}
