/*
  Library for sending signals
*/
#ifndef Brush_h
#define COMMON_ANODE
#define Brush_h

#include "Arduino.h"

Class Brush {
  public:
    Brush(int red, int green, int blue, int ir);
    void setColor(int redValue, int greenValue, int blueValue);
    void start();
  private:
    void _clearPin();
    int _irPin;
    int _redPin;
    int _greenPin;
    int _bluePin;
}

#endif
