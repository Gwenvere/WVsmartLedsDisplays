#ifndef baseLed_h
#define baseLed_h

#include "millisDelay.h"
#include "Arduino.h"

class baseLed {
  public:
    baseLed(int red, int green, int blue, int ir);
    void setColor(int redValue, int greenValue, int blueValue);
    void clearIRpin();
    int getBaseValue();
    float getAverage(int * array, int len);	
	
  private:
	int _redPin;
	int _greenPin;
	int _bluePin;
	int _irPin;
	int _prevAverage;
	int _threshAdd;

};

#endif