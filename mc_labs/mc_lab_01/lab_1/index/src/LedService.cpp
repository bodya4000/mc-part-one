#include <Arduino.h>
#include "../include/LedService.h"
#include "../include/Color.h"
#include "../include/Led.h"

LedService::LedService(std::vector<Led*> leds) : leds(std::move(leds)) {}

void LedService::next(bool reverse) {
  if (leds.empty()) return;
  currentIndex = reverse ?
    (currentIndex - 1 + leds.size()) % leds.size() :
    (currentIndex + 1) % leds.size();
}

void LedService::blinkCurrent(int delayMs) {
  if (leds.empty()) return;
  leds[currentIndex]->on();
  delay(delayMs);
  leds[currentIndex]->off();
}

void LedService::onCurrent() {
  if (!leds.empty())
    leds[currentIndex]->on();
}

void LedService::offCurrent() {
  if (!leds.empty())
    leds[currentIndex]->off();
}

int LedService::getCurrentIndex() const {
  return currentIndex;
}

Color LedService::getCurrentColor() const {
  if (leds.empty()) return Color::RED;
  return leds[currentIndex]->getColor();
}
