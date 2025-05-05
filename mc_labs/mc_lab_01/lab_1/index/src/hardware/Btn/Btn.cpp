#include <Arduino.h>
#include "Btn.h"
#include "../Pins.h"

Btn::Btn(Pin  pin) : pin(pin) {
  pinMode(static_cast<uint8_t>(pin), INPUT);
}

bool Btn::isPressed() {
  if (digitalRead(static_cast<uint8_t>(pin)) == LOW) {
    if (pressStartTime == 0) {
      pressStartTime = millis();
    }
    if (millis() - pressStartTime >= 1200) {
      return true;
    }
  } else {
    pressStartTime = 0;
  }
  return false;
}

bool Btn::wasBtnPreviouslyPressed() const {
  return btnPreviouslyPressed;
}

void Btn::setBtnWasNotPressed() {
  btnPreviouslyPressed = false;
}

void Btn::setBtnWasPressed() {
  btnPreviouslyPressed = true;
}
