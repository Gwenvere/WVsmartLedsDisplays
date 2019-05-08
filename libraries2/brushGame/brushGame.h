/*
  Library for sending signals
*/
#ifndef brushGame_h
#define brushGame_h

#include "Arduino.h"
#include "millisDelay.h"
#include <baseLed.h>

class brushGame {
  public:
    brushGame(int ir);
    void start(int playTime);
  private:
    int _irPin;
	int _threshAdd;

};

#endif
