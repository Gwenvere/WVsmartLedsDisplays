#include "Arduino.h"
#include "moleGame.h"
#include "millisDelay.h"




#define COMMON_ANODE
millisDelay ledDelay;
millisDelay baseDelay;

moleGame::moleGame(int rPin, int gPin,int bPin,int iPin){
  _redPin = rPin;
  _greenPin = gPin;
  _bluePin = bPin;
  _irPin = iPin;
  _threshAdd = 15;
  prevAverage = 999;
  
}

void moleGame::start(){
  pinMode(_redPin, OUTPUT);
  pinMode(_greenPin, OUTPUT);
  pinMode(_bluePin, OUTPUT);
  
  setColor(0, 255, 0);
  Serial.begin(9600);
  clearPin(_irPin); 
  int rand = random(0,20); //to randomize the red flashing
  if (rand > 14){
    activated();
  }
  setColor(0, 255, 0); // Green Color
  delay(800);
}

void moleGame::activated(){
  int threshold = getBaseValue() + _threshAdd;
  setColor(255, 0, 0); // Red 
  /*Serial.println("Threshold:");
  Serial.println(threshold);
  Serial.println("");*/
  
  ledDelay.start(3000); //time the player has to disable the red led
  boolean notCorrected = true;
  while (!ledDelay.isFinished()&& notCorrected){
    int val = analogRead(_irPin);
    
    if (val > threshold){
      setColor(0,0,255); // Blue 
      delay(400);
      setColor(0,255,0);//Green
      notCorrected = false;
    }
  }
  if (notCorrected){
    flicker();
    setColor(0,255,0);
  }
}

int moleGame::getBaseValue(){ //get a base reading of the IR value

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

float moleGame::getAverage (int * array, int len)  
{
  long sum = 0L ;  
  for (int i = 0 ; i < len ; i++)
    sum += array [i] ;
  return  ((float) sum) / len ;  
}

void moleGame::flicker(){ //the flickering when the led was not disabled
  for ( int i = 0; i<5; i++){
    setColor(255,180,0);//orange
    delay(100);
    setColor(255,0,0);
    delay(100);
  }
}
void moleGame::setColor(int redValue, int greenValue, int blueValue) {
  #ifdef COMMON_ANODE
    redValue = 255 - redValue;
    greenValue = 255 - greenValue;
    blueValue = 255 - blueValue;
  #endif
  analogWrite(_redPin, redValue);
  analogWrite(_greenPin, greenValue);
  analogWrite(_bluePin, blueValue);  
}

//remove residual current
void moleGame::clearPin(int pin) {
  for (int i = 0; i < 10; i ++)
    analogRead(pin);

}