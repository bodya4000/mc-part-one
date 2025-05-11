#include <ESPAsyncWebServer.h>
#include "../../../services/StateManager/StateManager.h"

class WebBtnController {
 private:
  AsyncWebServer& server;
  AsyncWebSocket ws;
  StateManager& stateManager;

 public:
  WebBtnController(AsyncWebServer& srv, StateManager& sm);
  void setupRoutes();
  void setupWebSocket();
  void handleWebSoclet();
};
