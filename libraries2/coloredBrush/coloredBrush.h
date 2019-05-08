/*
  Library for sending signals
*/
#ifndef coloredBrush_h
#define coloredBrush_h

#include "Arduino.h"
#include "millisDelay.h"
#include <baseLed.h>

class coloredBrush {
  public:
    coloredBrush(int ir);
    void start(int playTime);
  private:
    int _irPin;
	int _threshAdd;

};

#endif
