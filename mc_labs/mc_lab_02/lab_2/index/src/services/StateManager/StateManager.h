#pragma once

class StateManager {
private:
  bool webBtnPressed = false;
  bool physicalBtnPressed = false;
  bool locked = false;

public:
  void setWebBtnPressed(bool pressed);
  void setPhysicalBtnPressed(bool pressed);

  bool isWebBtnPressed() const;
  bool isPhysicalBtnPressed() const;

  bool isAnyPressed() const;
  bool isLocked() const;

	void setLocked(bool locked);

  void update(bool webState, bool physicalState);
};
