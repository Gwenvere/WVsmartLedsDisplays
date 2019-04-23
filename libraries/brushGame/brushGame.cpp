#include "Arduino.h"
#include <brushGame.h>
#include <millisDelay.h>

#define COMMON_ANODE
millisDelay colorDelay;

brushGame::brushGame(int red, int green, int blue, int ir) {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  _redPin = red;
  _greenPin = green;
  _bluePin = blue;
  _irPin = ir;
  _threshAdd = 15;
  prevAverage = 999;
  

}

void brushGame::start(int playTime) {
	clearpin(_irPin);
	Serial.begin(9600);
	setColor(0, 255, 0);
	delay(300);
	int threshold = getBaseValue() + _threshAdd;
	Serial.println(prevAverage);
	
	colorDelay.start(playTime); //playTime in milliseconds
	while (!colorDelay.isFinished()){
	int val = analogRead(_irPin);
	Serial.println(val);
	
		if (val > threshold){
			setColor(255, 0, 0); // Red Color
			delay(500);
		}
		setColor(0, 255, 0); // Green Color
	}
	setColor(0, 0, 255); //blue to indicate that the game has ended
	delay(2000);

}

	

void brushGame::setColor(int redValue, int greenValue, int blueValue) {
  #ifdef COMMON_ANODE
    redValue = 255 - redValue;
    greenValue = 255 - greenValue;
    blueValue = 255 - blueValue;
  #endif
  analogWrite(_redPin, redValue);
  analogWrite(_greenPin, greenValue);
  analogWrite(_bluePin, blueValue);
}



void brushGame::clearpin(int pin) {
  for (int i = 0; i < 10; i ++)
    analogRead(pin);
}
int brushGame::getBaseValue(){ //get a base reading of the IR value

  int numReadings = 10;           // the number of readings
  int readings[numReadings];      // the readings from the analog input
  int readIndex = 0;              // the index of the current reading
  int average = 0;                // the average

  for (int thisReading = 0; thisReading < numReadings; thisReading++) { //get readings
    readings[thisReading] = analogRead(_irPin);
    delay(50);
  }
  average = (int) getAverage (readings, numReadings);
  if (average > prevAverage + 10){ //the pen was probably still pointed at the led, so take the previous average
    average = prevAverage;
  }
  prevAverage = average;
  return average;
 
}

float brushGame::getAverage (int * array, int len)  
{
  long sum = 0L ;  
  for (int i = 0 ; i < len ; i++)
    sum += array [i] ;
  return  ((float) sum) / len ;  
}
