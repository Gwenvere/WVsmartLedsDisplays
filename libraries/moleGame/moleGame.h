#ifndef moleGame_h
#define moleGame_h

#include "millisDelay.h"
#include "Arduino.h"
#include <baseLed.h>

class moleGame {
  public:
    moleGame(int ir); 
	void start(int playTime);
  private:
    void flicker();   
    void activated();
	int _threshAdd;
	int _irPin;

};

#endif