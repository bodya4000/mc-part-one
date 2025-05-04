#include "include/Led.h"
#include "include/Pins.h"
#include "include/Btn.h"
#include "include/LedService.h"
#include "include/WiFiService.h"
#include "include/AsyncWebServerService.h"
#include <vector>

const char* ssid = "dodkolox";
const char* password = "12345678";

const uint32_t BLINK_INTERVAL = 1000;
const uint32_t HOLD_INTERVAL = 500;

LedService* ledService;
Btn* btn;
AsyncWebServerService serverService;

bool hardIsHeld = false;
bool webIsHeld = false;
bool serialIsHeld = false;
uint32_t pressStartTime = 0;
uint8_t serialData;
uint32_t previousBlinkTime = 0;

void setupSerial() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Wemos started its job!");
}

void setupLeds() {
  static Led redLed(D4_PIN, Color::RED);
  static Led yellowLed(D5_PIN, Color::YELLOW);
  static Led greenLed(D7_PIN, Color::GREEN);

  std::vector<Led*> ledList = { &redLed, &yellowLed, &greenLed };
  ledService = new LedService(ledList);
}

void setupBtn() {
  btn = new Btn(D6_PIN);
}

void handleButton() {
  if (btn->isPressed()) {
    if (!btn->wasBtnPreviouslyPressed()) {
      pressStartTime = millis();
      btn->setBtnWasPressed();
    } else if (millis() - pressStartTime >= HOLD_INTERVAL) {
      hardIsHeld = true;
    }
  } else {
    if (btn->wasBtnPreviouslyPressed()) {
      hardIsHeld = false;
    }
    btn->setBtnWasNotPressed();
  }
}

void checkSerial() {
  if (Serial.available() > 0) {
    serialData = Serial.read();
    switch (serialData) {
      case 'h': serialIsHeld = true; break;
      case 'r': serialIsHeld = false; break;
    }
  }
}

void blinkLogic() {
  uint32_t now = millis();
  if (now - previousBlinkTime >= BLINK_INTERVAL) {
    previousBlinkTime = now;

    bool reverse = hardIsHeld || webIsHeld || serialIsHeld;
    ledService->next(reverse);
    ledService->blinkCurrent(1);

    serverService.sendColor(ledService->getCurrentColor());
  }
}

void setup() {
  setupSerial();
  WiFiService::startAP(ssid, password);
  serverService.setControlRefs(&webIsHeld);
  serverService.begin();
  setupLeds();
  setupBtn();
}

void loop() {
  handleButton();
  checkSerial();
  blinkLogic();
  serverService.cleanupClients();
}