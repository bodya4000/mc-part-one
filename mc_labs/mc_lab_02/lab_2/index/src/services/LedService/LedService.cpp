#include <Arduino.h>
#include "LedService.h"
#include "../../hardware/Led/Led.h"
#include "../../hardware/Led/Color.h"

LedService::LedService(std::vector<Led*> leds) : leds(std::move(leds)) {}

bool LedService::getIsRunning() const {
  return isRunning;
}


void LedService::go() {
  if (leds.empty()) return;
	if (isRunning) {
		leds[currentIndex]->off();
		currentIndex = (currentIndex + 1) % leds.size();
		leds[currentIndex]->on();
	}else {
		leds[currentIndex]->on();
		isRunning = true;
	}
}

void LedService::stop() {
  if (leds.empty() || !isRunning) return;
  leds[currentIndex]->off();
	isRunning = false;
}

