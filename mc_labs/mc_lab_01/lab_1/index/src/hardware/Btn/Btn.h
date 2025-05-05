#pragma once
#include "../Pins.h"


class Btn {
private:
	Pin  pin;
  bool btnPreviouslyPressed = false;
  uint32_t pressStartTime = 0;

public:
  Btn(Pin  pin);
  bool isPressed();
  bool wasBtnPreviouslyPressed() const;
  void setBtnWasPressed();
  void setBtnWasNotPressed();
};
