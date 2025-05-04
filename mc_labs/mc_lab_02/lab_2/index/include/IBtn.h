#ifndef IBTN_H
#define IBTN_H

class IBtn {
public:
  virtual bool isPressed() = 0;
  virtual ~IBtn() {}
};

#endif
