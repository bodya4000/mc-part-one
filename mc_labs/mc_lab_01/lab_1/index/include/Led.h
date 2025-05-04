#pragma once
#include "Color.h"
#include "Pins.h"


class Led {
private:
	Pin  pin;
  Color color;
  bool state = false;

public:
	Led(Pin pin, Color color);


  void on();
  void off();
  void toggle();
  bool isOn() const;
  Color getColor() const;
};
