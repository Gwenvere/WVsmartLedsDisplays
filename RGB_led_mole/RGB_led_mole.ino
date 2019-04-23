int redPin= 1;
int greenPin = 2;
int bluePin = 3;

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

  
  setColor(255, 0, 0); // Red Color
  ledDelay.start(3000);
  boolean notCorrected = true;
  while (!ledDelay.isFinished()&& notCorrected){
    int val = analogRead(irPin);
    Serial.println(val);
    if (val > 380){
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

int getBaseValue(){

  const int numReadings = 10;

  int readings[numReadings];      // the readings from the analog input
  int readIndex = 0;              // the index of the current reading
  int total = 0;                  // the running total
  int average = 0;                // the average

  for (int i = 0; i < numReadings; i++) {
    
    readings[thisReading] = analogRead(irPin);
    delay(50);
  }

  
 
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
