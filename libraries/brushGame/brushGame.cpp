#include "Arduino.h"
#include <brushGame.h>
#include <millisDelay.h>
#include <baseled.h>

#define COMMON_ANODE
millisDelay playDelayBrush;
baseLed ledB(1,2,3,1);
brushGame::brushGame(int ir) {
  _threshAdd = 15;
  _irPin = ir;
}

void brushGame::start(int playTime) {
	ledB.clearIRpin();
	Serial.begin(9600);
	ledB.setColor(255, 255, 255);
	delay(300);
	int threshold = ledB.getBaseValue() + _threshAdd;

	
	playDelayBrush.start(playTime); //playTime in milliseconds
	while (!playDelayBrush.isFinished()){
	int val = analogRead(_irPin);
	Serial.println(val);
	
		if (val > threshold){
			ledB.setColor(0,255,60); 
			delay(700);
		}
		ledB.setColor(255, 255, 255); 
	}
	ledB.setColor(0, 0, 255); //blue to indicate that the game has ended
	delay(2000);

}
