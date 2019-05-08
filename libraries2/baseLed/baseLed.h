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
	bool check_top(int th);
	bool check_middle(int th);
	bool check_bottom(int th);
	int getButton(int th);
  	void read_bit_str(bool *bit_str,int th);	
	int getValue();

	
  private:

	bool _read_bit(int threshold);
	int _getRatio(bool *_bit_str);
	bool _bit_str[];
	bool array_cmp(bool a[], bool b[], int len_a, int len_b);

	
	int _redPin;
	int _greenPin;
	int _bluePin;
	int _irPin;
	int _prevAverage;
	int _threshAdd;
	int bit_dly;


};

#endif