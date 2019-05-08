#ifndef reader_h
#define reader_h

#include "millisDelay.h"
#include "Arduino.h"

class reader {
  public:
    reader(int ir);
    void start(bool bit_str[], int threshold);

	
  private:


	int analogPin;
	int val;
	int th; //treshold
	int bit_dly; //delay
	int bstr_dly; //delay kleiner dan bij sender (of 0) 
	int sample_dly;
	int del;


	bool bit_str[];
	bool samples[];
	bool previous_bit;
	bool initial_run;
	
	void clearPin(int pin);
	bool readOne();
	bool readPre();
	void read_bit_str(bool bit_str[]);
	bool read_bit();
	void print_bit_str(bool bit_str[]);
	void print_samples();
	char boolsToChar(bool bools[]);


};

#endif