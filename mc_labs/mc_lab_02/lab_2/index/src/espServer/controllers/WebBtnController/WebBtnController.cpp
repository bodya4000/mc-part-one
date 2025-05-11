#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "WebBtnController.h"
#include "../../../services/StateManager/StateManager.h"

WebBtnController::WebBtnController(AsyncWebServer& srv, StateManager& sm)
  : server(srv), ws("/ws"), stateManager(sm) {}

void WebBtnController::setupRoutes() {
  server.on("/press", HTTP_POST, [this](AsyncWebServerRequest *request) {
		Serial.println("Message: Web Pressed");
    stateManager.setWebBtnPressed(true);
    request->send(200, "text/plain", "Pressed");
  });

  server.on("/press_out", HTTP_POST, [this](AsyncWebServerRequest *request) {
		Serial.println("Message: Web Released");
    stateManager.setWebBtnPressed(false);
    request->send(200, "text/plain", "Released");
  });

  setupWebSocket();
  server.addHandler(&ws);
}

void WebBtnController::setupWebSocket() {
  ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client,
  AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_DATA) {
      AwsFrameInfo *info = (AwsFrameInfo *)arg;
      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        String message = String((char *)data).substring(0, len);
        message.trim();
				Serial.print("Message from socket: ");
				Serial.println(message.c_str());
				
        if (message == "press") {
          stateManager.setWebBtnPressed(true);
          client->text("WebBtn: Pressed");
        } else if (message == "release") {
          stateManager.setWebBtnPressed(false);
          client->text("WebBtn: Released");
        } else {
          client->text("Unknown command");
        }
      }
    }
  });
}


void WebBtnController::handleWebSoclet() {
  ws.cleanupClients();
}
