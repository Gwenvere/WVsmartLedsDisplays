#include "Arduino.h"
#include <baseLed.h>
#include <millisDelay.h>
#include <reader.h>


#define COMMON_ANODE
#define NB_SAMPLES 5
#define NB_BITS_REC 4
#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))
millisDelay ledReadingDelay;

bool top_str[] = {1,1,0,0};
bool middle_str[] = {1,0,1,0};
bool bottom_str[] = {0,1,1,0};
bool nextMode_str[] = {1,1,1,0};

reader cbReader(1);
baseLed::baseLed(int red, int green, int blue, int ir) {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  _redPin = red;
  _greenPin = green;
  _bluePin = blue;
  _irPin = ir;
  _threshAdd = 80;
  _prevAverage = 999;
  
  int bit_dly = 50; //delay

  
  
  pinMode(_redPin, OUTPUT);
  pinMode(_greenPin, OUTPUT);
  pinMode(_bluePin, OUTPUT);
  
  clearIRpin();

  
}
void baseLed::setColor(int redValue, int greenValue, int blueValue) {
  #ifdef COMMON_ANODE
    redValue = 255 - redValue;
    greenValue = 255 - greenValue;
    blueValue = 255 - blueValue;
  #endif
  analogWrite(_redPin, redValue);
  analogWrite(_greenPin, greenValue);
  analogWrite(_bluePin, blueValue);
}

void baseLed::clearIRpin() {
  for (int i = 0; i < 10; i ++)
    analogRead(_irPin);
}

int baseLed::getBaseValue(){ //get a base reading of the IR value

  int numReadings = 10;           // the number of readings
  int readings[numReadings];      // the readings from the analog input
  int readIndex = 0;              // the index of the current reading
  int average = 0;                // the average

  for (int thisReading = 0; thisReading < numReadings; thisReading++) { //get readings
    readings[thisReading] = analogRead(_irPin);
    delay(50);
  }
  average = (int) getAverage (readings, numReadings);
  /*if (average < _prevAverage - _threshAdd){ 
    average = _prevAverage;
  }*/
  _prevAverage = average;
  return average;
 
}

int baseLed::getValue(){ //get a base reading of the IR value
  ledReadingDelay.start(bit_dly);
  int numReadings = 10;           // the number of readings
  int readings[numReadings];      // the readings from the analog input
  int readIndex = 0;              // the index of the current reading
  int average = 0;                // the average

  for (int thisReading = 0; thisReading < numReadings; thisReading++) { //get readings
    readings[thisReading] = analogRead(_irPin);
    delay(4);
  }
  average = (int) getAverage (readings, numReadings);
  /*if (average < _prevAverage - 10){ //the pen was probably still pointed at the led, so take the previous average
    average = _prevAverage;
  }*/
  _prevAverage = average;
  while(!ledReadingDelay.isFinished());
  return average;
 
}

float baseLed::getAverage (int * array, int len)  {
  long sum = 0L ;  
  for (int i = 0 ; i < len ; i++)
    sum += array [i] ;
  return  ((float) sum) / len ;  
}

void baseLed::read_bit_str(bool *bit_str, int th) {

  for (int count = 0; count < 20; count ++) {
    bit_str[count] = _read_bit(th);
	//delay(bit_dly);
	//Serial.println("bit_str: ");
	//Serial.println(bit_str[count]);

  }
  return;
}
bool baseLed::_read_bit(int threshold) {
	int val = getValue();
	//Serial.println(val);
	if (val < threshold){
		//Serial.println("val:");
		
		return true;
	}
	return false;
}
int baseLed::_getRatio(bool bit_str[]){
	//Serial.println("RATIO");
	int one_count = 0;
	int zero_count = 0;
	for(int i = 0; i < 20; i++){
		//Serial.println(bit_str[i]);
		if (bit_str[i] == 1){
			one_count ++;
		}
		else if (bit_str[i] == 0){
			zero_count ++;
		}
	}
	Serial.println("one:");
	Serial.println(one_count);
	Serial.println("zero:");
	Serial.println(zero_count);
	
	if (one_count > zero_count)
		if (((zero_count/one_count)*10) < 2) return 2; //1111  
		else if (((zero_count/one_count)*10) < 5) return 1; //1110
		else if (((zero_count/one_count)*10) < 11) return 3; //1100
	else if (zero_count > one_count)
		if(((zero_count/one_count)*10)>9)return 3;//1100
		
	return 0;
		
		
//	int ratio = zero_count/one_count;
	/*Serial.println("Ratio:");
	Serial.println(ratio);
	return ratio;*/
}
	
bool baseLed::check_top(int th){
	bool bit_str[NB_BITS_REC*NB_SAMPLES];
	read_bit_str(bit_str,th);
	float ratio = _getRatio(bit_str);
	if ((ratio < 0.4) && (ratio > 0.2)){
		return true;
	}
	return false;
}
bool baseLed::check_middle(int th){
	bool bit_str[NB_BITS_REC*NB_SAMPLES];
	read_bit_str(bit_str,th);
	float ratio = _getRatio(bit_str);
	if ((ratio < 1.1) && (ratio > 0.9)){
		return true;
	}
	return false;
}
bool baseLed::check_bottom(int th){
	bool bit_str[NB_BITS_REC*NB_SAMPLES];
	read_bit_str(bit_str,th);
	float ratio = _getRatio(bit_str);
	if ((ratio < 0.6) && (ratio > 0.4)){
		return true;
	}
	return false;
}

int baseLed::getButton(int th){
	bool bit_str[4];
	cbReader.start(bit_str, th);
	if (array_cmp(bit_str, top_str, 4, 4)) return 1; //top
	else if (array_cmp(bit_str, middle_str, 4, 4)) return 2; //middle
	else if (array_cmp(bit_str, bottom_str, 4, 4)) return 3; //bottom
	else if (array_cmp(bit_str, nextMode_str, 4, 4)) return 9; //nextMode
	else return 0;
	
	/*bool bit_str[20];
	read_bit_str(bit_str,th);
	int ratio = _getRatio(bit_str);
	return ratio;
	//Serial.println(bit_str);
	//Serial.println(ratio);
	/*if ((ratio < 0.4) && (ratio > 0.2)){
		return 1; //top button
	}
	else if ((ratio < 0.1) && (ratio > -0.1)){
		return 2; //middle button
	}
	else if ((ratio < 0.6) && (ratio > 0.4)){
		return 3; //bottom button
	}
	*/
	//else return 0; //no button found
}

bool baseLed::array_cmp(bool a[], bool b[], int len_a, int len_b){
     int n;

     // if their lengths are different, return false
     if (len_a != len_b) return false;

     // test each element to be the same. if not, return false
     for (n=0;n<len_a;n++) if (a[n]!=b[n]) return false;


	return true;
}

