int redPin= 1;
int greenPin = 2;
int bluePin = 3;
int prevAverage = 999;
int theshAdd = 15;


int irPin = 1;
#define COMMON_ANODE
#include <millisDelay.h>
millisDelay ledDelay;
millisDelay baseDelay;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  setColor(0, 255, 0);

  Serial.begin(9600);
  clearPin(irPin);
  delay(100);
  
}
void loop() {
 // int val = analogRead(irPin);
 // Serial.println(val);
  int rand = random(0,20);
  //Serial.println(rand);
  if (rand > 15){
    activated();
  }
  setColor(0, 255, 0); // Green Color
  delay(800);
}

void activated(){

  int threshold = getBaseValue() + theshAdd;
  setColor(255, 0, 0); // Red Color
  Serial.println("Threshold:");
  Serial.println(threshold);
  Serial.println("");
  
  ledDelay.start(3000);
  boolean notCorrected = true;
  while (!ledDelay.isFinished()&& notCorrected){
    int val = analogRead(irPin);
    //Serial.println(val);
    if (val > threshold){
    setColor(0,0,255); // Blue Color
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

int getBaseValue(){ //get a base reading of the IR value

  int numReadings = 10;           //the number of readings
  int readings[numReadings];      // the readings from the analog input
  int readIndex = 0;              // the index of the current reading
  int average = 0;                // the average

  for (int thisReading = 0; thisReading < numReadings; thisReading++) { //get readings
    readings[thisReading] = analogRead(irPin);
    delay(50);
  }
  average = (int) getAverage (readings, numReadings);
  //Serial.println(average);
  //Serial.println(prevAverage);
  if (average > prevAverage + 10){ //the pen was probably still pointed at the led
    average = prevAverage;
  }
  prevAverage = average;
  return average;
 
}

float getAverage (int * array, int len)  
{
  long sum = 0L ;  
  for (int i = 0 ; i < len ; i++)
    sum += array [i] ;
  return  ((float) sum) / len ;  
}
void flicker(){
  for ( int i = 0; i<5; i++){
    setColor(255,180,0);//orange
    delay(100);
    setColor(255,0,0);
    delay(100);
  }
}
void setColor(int redValue, int greenValue, int blueValue) {
  #ifdef COMMON_ANODE
    redValue = 255 - redValue;
    greenValue = 255 - greenValue;
    blueValue = 255 - blueValue;
  #endif
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);  
}

//remove residual current
void clearPin(int pin) {
  for (int i = 0; i < 10; i ++)
    analogRead(pin);

}
