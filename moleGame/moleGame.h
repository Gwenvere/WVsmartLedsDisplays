#ifndef moleGame_h
#define moleGame_h

#include "millisDelay.h"
#include "Arduino.h"

class moleGame {
  public:
    moleGame(int redPin, int greenPin,int bluePin,int irPin); 
	void start();
	int prevAverage;
  private:
    void setColor(int redValue, int greenValue, int blueValue);
    void clearPin(int pin);
    int getBaseValue();
    float getAverage(int * array, int len);
    void flicker();   
    void activated();
	
	int _redPin;
	int _greenPin;
	int _bluePin;
	int _irPin;
	int _threshAdd;
};

#endif