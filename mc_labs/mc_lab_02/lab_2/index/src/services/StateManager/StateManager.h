#pragma once

class StateManager {
private:
  bool webBtnPressed = false;
  bool physicalBtnPressed = false;

public:
  void setWebBtnPressed(bool pressed);
  void setPhysicalBtnPressed(bool pressed);

  bool isWebBtnPressed() const;
  bool isPhysicalBtnPressed() const;

  bool isAnyPressed() const;

  void update(bool webState, bool physicalState);
};
