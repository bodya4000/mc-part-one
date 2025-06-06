#include <Arduino.h>
#include "src/hardware/Led/Led.h"
#include "src/hardware/Btn/Btn.h"
#include "src/hardware/Led/Color.h"
#include "src/hardware/Pins.h"
#include "src/services/LedService/LedService.h"
#include "src/services/WifiConfigurer/WiFiConfigurer.h"
#include "src/espServer/EspServer.h"
#include "src/services/StateManager/StateManager.h"

LedService* ledService;
StateManager stateManager;
EspServer espServer(stateManager);
Btn* btn;

unsigned long lastChange = 0;
const unsigned long interval = 1200;

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
  stateManager.update(false, false);
}

void setupServer() {
  bool connected = WiFiConfigurer::connectToWiFi("Redmi 12", "b0000000");
  if (connected) {
    Serial.println("Connected");
    espServer.begin();
    Serial.println("Server started");
  }
}

void setup() {
  setupSerial();
  setupServer();
  setupHardware();
  setupState();
}

void loop() {
  if (millis() - lastChange >= interval) {
    checkSerial();
    handleLedAlgo();
    lastChange = millis();
  }
	espServer.handleWebSocket();
}

void checkSerial() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    if (command == '\r' || command == '\n' || command == ' ') {
      return;
    }

    Serial.print("Received command: ");
    Serial.println(command);

    if (command == 'g') {
      stateManager.setLocked(false);
    } else if (command == 's') {
      stateManager.setLocked(true);
    }
  }
}


void handleLedAlgo() {
  Serial.println("Is Locked 2: " + String(stateManager.isLocked()));
//  Serial.println("Is Locked 2: " + String(stateManager.isLocked()));

  stateManager.setPhysicalBtnPressed(btn->isPressed());
  if (!stateManager.isLocked() && stateManager.isAnyPressed()) {
    ledService->go();
  } else {
    ledService->stop();
  }
}
