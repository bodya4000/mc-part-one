#pragma once
#include "ILed.h"
#include "Color.h"
#include <vector>


class LedService {
private:
  std::vector<ILed*> leds;
  int currentIndex = 0;

public:
  explicit LedService(std::vector<ILed*> leds);

  void next(bool reverse = false);
  void blinkCurrent(int delayMs);
  void onCurrent();
  void offCurrent();
  int getCurrentIndex() const;
  Color getCurrentColor() const;
};
