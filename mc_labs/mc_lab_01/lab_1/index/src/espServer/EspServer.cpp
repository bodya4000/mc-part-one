#include <Arduino.h>
#include "EspServer.h"

EspServer::EspServer(StateManager& sm)
	: stateManager(sm),
		server(80),
		htmlService(),
		pageController(server, htmlService),
		webBtnController(server, stateManager) {}

void EspServer::begin() {
  pageController.setupRoutes();
  webBtnController.setupRoutes();
	webBtnController.setupWebSocket();
  server.begin();
}

void EspServer::handleWebSocket() {
	webBtnController.setupWebSocket();
}