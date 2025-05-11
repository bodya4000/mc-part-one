#include <Arduino.h>
#include "controllers/PageController/PageController.h"
#include "controllers/WebBtnController/WebBtnController.h"
#include "../services/HtmlPageBuilder/HtmlPageBuilder.h"
#include "../services/StateManager/StateManager.h"
#include <ESPAsyncWebServer.h>


class EspServer {
	private:
		StateManager& stateManager; 
		AsyncWebServer server;
		HtmlPageBuilder htmlService;
		PageController pageController;
		WebBtnController webBtnController;
	public:
  	EspServer(StateManager& sm);
		void begin();
		void handleWebSocket();
	};
	