#pragma once
#include <ESPAsyncWebServer.h>
#include "../../../services/HtmlPageBuilder/HtmlPageBuilder.h"

class PageController {
private:
  AsyncWebServer& server;
  HtmlPageBuilder& htmlService;

public:
  PageController(AsyncWebServer& srv, HtmlPageBuilder& htmlBuilder);
  void setupRoutes();
};
