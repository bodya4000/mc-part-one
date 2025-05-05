#include <Arduino.h>
#include "controllers/PageController/PageController.h"
#include "controllers/WebBtnController/WebBtnController.h"
#include "../services/HtmlPageBuilder/HtmlPageBuilder.h"
#include "../services/StateManager/StateManager.h"
#include <ESPAsyncWebServer.h>


class EspServer {
	private:
		AsyncWebServer server;
		HtmlPageBuilder htmlService;
		StateManager stateManager;
		PageController pageController;
		WebBtnController webBtnController;
	public:
		EspServer();
		void begin();
	};
	