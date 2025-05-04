#include <Arduino.h>
#include "../include/Led.h"
#include "Led.h"

Led::Led(Pin pin, Color color) : pin(pin), color(color) {
	pinMode(pin, OUTPUT);
}

void Led::on() {
	digitalWrite(pin, HIGH);
	state = true;
}

void Led::off() {
	digitalWrite(pin, LOW);
	state = false;
}

void Led::toggle() {
	state = !state;
	digitalWrite(pin, state ? HIGH : LOW);
}

bool Led::isOn() const {
	return state;
}

Color Led::getColor() const {
	return color;
}