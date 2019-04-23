#include "Arduino.h"
#include <baseLed.h>
#include <millisDelay.h>

#define COMMON_ANODE
baseLed::baseLed(int red, int green, int blue, int ir) {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  _redPin = red;
  _greenPin = green;
  _bluePin = blue;
  _irPin = ir;
  _threshAdd = 15;
  _prevAverage = 999;
  
  pinMode(_redPin, OUTPUT);
  pinMode(_greenPin, OUTPUT);
  pinMode(_bluePin, OUTPUT);
  
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
  if (average > _prevAverage + 10){ //the pen was probably still pointed at the led, so take the previous average
    average = _prevAverage;
  }
  _prevAverage = average;
  return average;
 
}

float baseLed::getAverage (int * array, int len)  
{
  long sum = 0L ;  
  for (int i = 0 ; i < len ; i++)
    sum += array [i] ;
  return  ((float) sum) / len ;  
}