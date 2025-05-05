#pragma once
#include <ESPAsyncWebServer.h>
#include "../../../services/StateManager/StateManager.h"

class WebBtnController {
public:
  WebBtnController(AsyncWebServer& server, StateManager& stateManager);
  void setupRoutes();

private:
  AsyncWebServer& server;
  StateManager& stateManager;
};
