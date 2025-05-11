#include <Arduino.h>
#include "Led.h"
#include "Color.h"
#include "../Pins.h"

Led::Led(Pin pin, Color color) : pin(pin), color(color) {
	pinMode(static_cast<uint8_t>(pin), OUTPUT);
}

void Led::on() {
	digitalWrite(static_cast<uint8_t>(pin), HIGH);
	state = true;
}

void Led::off() {
	digitalWrite(static_cast<uint8_t>(pin), LOW);
	state = false;
}

void Led::toggle() {
	state = !state;
	digitalWrite(static_cast<uint8_t>(pin), state ? HIGH : LOW);
}

bool Led::isOn() const {
	return state;
}

Color Led::getColor() const {
	return color;
}
