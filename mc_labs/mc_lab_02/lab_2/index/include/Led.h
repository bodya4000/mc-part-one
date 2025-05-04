#pragma once
#include "ILed.h"
#include "Color.h"

class Led : public ILed {
private:
  int pin;
  Color color;
  bool state = false;

public:
  Led(int pin, Color color);

  void on() override;
  void off() override;
  void toggle() override;
  bool isOn() const;
  Color getColor() const override;
};
