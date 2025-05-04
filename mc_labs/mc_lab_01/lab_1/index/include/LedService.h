#pragma once
#include "Led.h"
#include "Color.h"
#include <vector>


class LedService {
private:
  std::vector<Led*> leds;
  int currentIndex = 0;

public:
  LedService(std::vector<Led*> leds);

  void next(bool reverse = false);
  void blinkCurrent(int delayMs);
  void onCurrent();
  void offCurrent();
  int getCurrentIndex() const;
  Color getCurrentColor() const;
};
