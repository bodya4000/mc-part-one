#include <Arduino.h>
#include "include/Led.h"
#include "include/Pins.h"
#include "include/Color.h"
#include "include/Btn.h"
#include "include/LedService.h"
#include "include/WiFiService.h"
#include "include/WebServerService.h"
#include <vector>

const char* ssid = "1234";
const char* password = "12345678";

LedService* ledService;
Btn* btn;
WebServerService serverService;
bool webBtnIsPressed = false;

void setupSerial() {
  Serial.begin(115200);
	Serial.setDebugOutput(true);
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

void setupWifi() {
	WiFiService::connectToWiFi();
}

void setupServer() {
	serverService.setHandlers(handleButtonPress, handleButtonPressOut);
	serverService.begin();
}

void setup() {
  setupSerial();
  setupWifi();
  setupServer();
  setupHardware();
}

void handleLedAlgo() {
  if (btn->isPressed() || webBtnIsPressed) {
    if (btn->wasBtnPreviouslyPressed() || webBtnIsPressed) {
      ledService->next();
    }
    Serial.println("Btn is being pressed!");
    ledService->blinkCurrent(1000);
    btn->setBtnWasPressed();
  } else {
    btn->setBtnWasNotPressed();
  }
}

void loop() {
  handleLedAlgo(); 
}

void handleButtonPress() {
  Serial.println("Virtual button pressed!");
  ledService->blinkCurrent(500);
  webBtnIsPressed = true;
  btn->setBtnWasPressed();
}

void handleButtonPressOut() {
  Serial.println("Virtual button released!");
  webBtnIsPressed = false;
}