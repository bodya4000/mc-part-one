#pragma once


class Btn {
private:
  int pin;
  bool btnPreviouslyPressed = false;
  uint32_t pressStartTime = 0;

public:
  explicit Btn(int pin);
  bool isPressed();
  bool wasBtnPreviouslyPressed() const;
  void setBtnWasPressed();
  void setBtnWasNotPressed();
};
