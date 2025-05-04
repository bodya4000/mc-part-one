#include <Arduino.h>
#include "../include/Btn.h"

Btn::Btn(int p) : pin(p) {
  pinMode(pin, INPUT);
}

bool Btn::isPressed() {
  if (digitalRead(pin) == LOW) {
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
