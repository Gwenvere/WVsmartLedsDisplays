#include "Arduino.h"
#include <coloredBrush.h>
#include <millisDelay.h>
#include <baseled.h>


#define COMMON_ANODE
millisDelay playDelayColoredBrush;
millisDelay colorDelay;

baseLed ledCB(1,2,3,1);

coloredBrush::coloredBrush(int ir) {
  _threshAdd = 60;
  _irPin = ir;
}

void coloredBrush::start(int playTime) {
	bool playing = true;
	ledCB.clearIRpin();
	Serial.begin(9600);
	ledCB.setColor(150, 150, 150);
	delay(300);
	int threshold = ledCB.getBaseValue() - _threshAdd;
	
	Serial.println("Threshold:");
	Serial.println(threshold);
	Serial.println("");
	
	int colorDetected;
	
	playDelayColoredBrush.start(playTime); //playTime in milliseconds
	while (!playDelayColoredBrush.isFinished() && playing){
	colorDetected = ledCB.getButton(threshold);

			switch (colorDetected) {
		  case 0:
		  if(colorDelay.isFinished()) ledCB.setColor(150,150,150);
			break;
		  case 1:
			ledCB.setColor(255,0,0);//red
			colorDelay.start(700);
			break;
		  case 2:
			ledCB.setColor(0,255,0);//green
			colorDelay.start(700);
			break;
		  case 3:
			ledCB.setColor(0,0,255);//blue
			colorDelay.start(700);
			break;
		  case 9:
			ledCB.setColor(0,0,0);
			//playing=false;
			break;
		  default:
			// statements
			break;
			
		}
		if(colorDelay.isFinished()) ledCB.setColor(150,150,150);
	}
	 /* ledCB.setColor(150,150,255);
	  delay(1000);
	  for ( int i = 0; i<255; i++){
		ledCB.setColor(150,150,255-i);
		delay(12);
		
	  }	
	  delay(2000);
	  ledCB.setColor(0,0,0);
	  delay(1000);

*/

}
