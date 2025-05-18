#include <Arduino.h>
#include "StateManager.h"

void StateManager::setWebBtnPressed(bool pressed) {
  webBtnPressed = pressed;
}

void StateManager::setPhysicalBtnPressed(bool pressed) {
  physicalBtnPressed = pressed;
}

void StateManager::setLocked(bool value) {
  locked = value;
}

bool StateManager::isLocked() const { 
  return locked;
}

bool StateManager::isWebBtnPressed() const {
  return webBtnPressed;
}

bool StateManager::isPhysicalBtnPressed() const {
  return physicalBtnPressed;
}

bool StateManager::isAnyPressed() const {
  return webBtnPressed || physicalBtnPressed;
}

void StateManager::update(bool webState, bool physicalState) {
  webBtnPressed = webState;
  physicalBtnPressed = physicalState;
}
