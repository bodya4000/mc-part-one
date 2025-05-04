#pragma once
#include "Color.h"

class ILed {
public:
  virtual ~ILed() = default;
  virtual void on() = 0;
  virtual void off() = 0;
  virtual void toggle() = 0;
  virtual Color getColor() const = 0;
};
