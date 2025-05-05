#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "WebBtnController.h"
#include "../../../services/StateManager/StateManager.h"

WebBtnController::WebBtnController(AsyncWebServer& srv, StateManager& sm)
  : server(srv), stateManager(sm) {}


void WebBtnController::setupRoutes() {
	server.on("/press", HTTP_POST, [this](AsyncWebServerRequest *request) {
		stateManager.setWebBtnPressed(true);
		request->send(200, "text/plain", "Pressed");
	});

	server.on("/release", HTTP_POST, [this](AsyncWebServerRequest *request) {
		stateManager.setWebBtnPressed(false);
		request->send(200, "text/plain", "Released");
	});
}
	