#include <Arduino.h>
#include "src/hardware/Led/Led.h"
#include "src/hardware/Btn/Btn.h"
#include "src/hardware/Led/Color.h"
#include "src/hardware/Pins.h"
#include "src/services/LedService/LedService.h"
#include "src/services/WifiConfigurer/WiFiConfigurer.h"
// #include "src/server/Server.h"
#include "src/services/StateManager/StateManager.h"


LedService* ledService;
StateManager* stateManager;
Btn* btn;

unsigned long lastChange = 0;
const unsigned long interval = 300;


void setupSerial(bool debug=false) {
  Serial.begin(115200);
	Serial.setDebugOutput(debug);
  Serial.println("Wemos started its job !");
}

void setupHardware() {
  static Led blueLed(Pin::D5, Color::GREEN);
	static Led redLed(Pin::D6, Color::RED);
  static Led yellowLed(Pin::D7, Color::YELLOW);
  std::vector<Led*> ledList = { &blueLed, &redLed, &yellowLed };
  ledService = new LedService(ledList);
  btn = new Btn(Pin::D0);
}

void setupState() {
	stateManager = new StateManager();
	stateManager->update(false, false);
}

void setupServer() {
	bool connected = WiFiConfigurer::startAP();
	if (connected) {
    Serial.println("Connected");
		// setup server
	}
}

void setup() {
  setupSerial();
  // setupServer();
  setupHardware();
  setupState();
}


void loop() {
  if (millis() - lastChange >= interval) {
    handleLedAlgo();
    lastChange = millis();
  }
}

void handleLedAlgo() {
	stateManager->setPhysicalBtnPressed(btn->isPressed());
  Serial.println(stateManager->isAnyPressed() ? "Btn state: true" : "Btn state: false");
	if (stateManager->isAnyPressed()) {
		ledService->go();
	} else {
		ledService->stop();
	}
  // ledService->go();

}