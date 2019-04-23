/*
  Library for sending signals
*/
#ifndef brushGame_h
#define brushGame_h

#include "Arduino.h"
#include "millisDelay.h"

class brushGame {
  public:
    brushGame(int redPin, int greenPin,int bluePin,int irPin);
    void setColor(int redValue, int greenValue, int blueValue);
    void start(int playTime);
	int prevAverage;
  private:
    int getBaseValue();
    float getAverage(int * array, int len);
    void clearpin(int pin);
    int _irPin;
    int _redPin;
    int _greenPin;
    int _bluePin;
	int _threshAdd;

};

#endif
