#include "Arduino.h"
#include <Brush.h>

Brush::Brush(int red, int green, int blue, int ir) {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  _redPin = red;
  _greenPin = green;
  _bluePin = blue;
  _irPin = ir;
  setColor(255, 0, 0);
  
  clearpin(_irPin);
}

void Brush::setColor(int redValue, int greenValue, int blueValue) {
  #ifdef COMMON_ANODE
    redValue = 255 - redValue;
    greenValue = 255 - greenValue;
    blueValue = 255 - blueValue;
  #endif
  analogWrite(_redPin, redValue);
  analogWrite(_greenPin, greenValue);
  analogWrite(_bluePin, blueValue);
}

void Brush::start() {
  int val = analogRead(irPin);
  Serial.println(val);

  if (val > 420){
    setColor(255, 0, 0); // Red Color
    delay(1000);
  }
  setColor(0, 255, 0); // Green Color
}

void Brush::clearpin(int pin) {
  for (int i = 0; i < 10; i ++)
    analogRead(pin);
}
