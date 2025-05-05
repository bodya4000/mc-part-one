#include <Arduino.h>
#include "EspServer.h"

EspServer::EspServer()
  : server(80),
    htmlService(),
		stateManager(),
    pageController(server, htmlService),
    webBtnController(server, stateManager) {}


void EspServer::begin() {
  pageController.setupRoutes();
  webBtnController.setupRoutes();
  server.begin();
}
