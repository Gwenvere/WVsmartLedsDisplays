#ifndef simonGame_h
#define simonGame_h

#include "millisDelay.h"
#include "Arduino.h"
#include <baseLed.h>

class simonGame {
  public:
    simonGame(int ir); 
	void start(int playTime);
  private:
	void play(int newColor);
    void flicker(int result);   
    bool colorActive(int color, int threshold);
	void win();
	void failed();
	
	int _threshAdd;
	int _irPin;
	int _simonList[];
	int _it;
	int _inputTime;
	int _flickerTime;
	int _playing;

};

#endif