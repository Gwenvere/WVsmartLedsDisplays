#include "Arduino.h"
#include "simonGame.h"
#include "millisDelay.h"
#include <baseled.h>




#define COMMON_ANODE

millisDelay ledDelaySim;
millisDelay baseDelaySim;
millisDelay playDelaySimon;
millisDelay colorDelaySim;
baseLed ledS(1,2,3,1);

simonGame::simonGame(int ir){
  _threshAdd = 60;
  _irPin = ir;
  _simonList[10] = {0};
  _inputTime = 4000;
  _flickerTime = 2000;

  
}

void simonGame::start(int playTime){
  _it = 1;
  ledS.setColor(255, 255, 255);
  ledS.clearIRpin();
  Serial.begin(9600);
  _playing = true;


	playDelaySimon.start(playTime); //playTime in milliseconds
	while (!playDelaySimon.isFinished()&& _playing){ 
		if(_it > 10){
			win();
		}
		int rand = random(1,3); //to randomize the color
		play(rand);
		_it ++;

	  delay(800);
	}
	ledS.setColor(0, 0, 255); //blue to indicate that the game has ended
	delay(2000);
}
void simonGame::win(){
	_it = 1;
	for (int i = 0; i < _it; i++){
		_simonList[i] = 0;
	}
	flicker(0);
	
}
void simonGame::failed(){
	_it = 1;
	for (int i = 0; i < _it; i++){
		_simonList[i] = 0;
	}
	flicker(1);
}	
void simonGame::flicker(int result){ //the flickering when the led was not disabled
	switch (result){
		case 0://win
			  for ( int i = 0; i<5; i++){
				ledS.setColor(0,255,0);//green
				delay(100);
				ledS.setColor(0,255,180);
				delay(100);
			  }
			  break;
		case 1://lose
			for ( int i = 0; i<5; i++){
				ledS.setColor(255,180,0);//orange
				delay(100);
				ledS.setColor(255,0,0);
				delay(100);
			}
			break;
	}
}	
void simonGame::play(int newColor){
	int threshold = ledS.getBaseValue() - _threshAdd;
	_simonList[_it] = newColor;
	int flash;
	bool error = false;
	int nextColor;
	
	//start showing section
	for (int color = 0; color < _it; color++){
		nextColor = _simonList[color];	

		switch (nextColor) {
		  case 0:
			break;
		  case 1:
			ledS.setColor(255,0,0); //red
			delay(_flickerTime);
			ledS.setColor(0,0,0);
			delay(_flickerTime/2);
			break;
		  case 2:
			ledS.setColor(0,255,0); //green
			delay(_flickerTime);
			ledS.setColor(0,0,0);
			delay(_flickerTime/2);
			break;
		  case 3:
			ledS.setColor(0,0,255); //blue
			delay(_flickerTime);
			ledS.setColor(0,0,0);
			delay(_flickerTime/2);
			break;
		  default:
			// statements
			break;
		}
	}
	ledS.setColor(255,255,255); //white
	delay(_flickerTime);
	ledS.setColor(0,0,0);
	//delay(_flickerTime);
	
	//start input section
	for (int color = 0; color < _it; color++){
	Serial.println("input");
	Serial.println(error);
	Serial.println(_playing);
		if (error || !_playing){
			Serial.println("Return");
			failed();
			return;
		}
	nextColor = _simonList[color];	
		switch (nextColor) {
		  case 0:
			break;
		  case 1://Red
			error = colorActive(1,threshold);
			break;
		  case 2://green
			error  = colorActive(2,threshold);
			break;
		  case 3://blue
			error  = colorActive(3,threshold);
			break;
		  default:
			// statements
			break;
		}
	}		
}
bool simonGame::colorActive(int color, int threshold){
	Serial.println("colorActive");
	colorDelaySim.start(_inputTime);
	int colorDetected;
	while (!colorDelaySim.isFinished()){ 
	colorDetected = ledS.getButton(threshold);
	
		if (colorDetected == 9){
			_playing = false;
			}
		else if (colorDetected == color){
				ledS.setColor(255*(0+color),255*(0+color/2),255*(0+color/3)); //corresp color
				delay(_flickerTime);
				ledS.setColor(0,0,0);
				return false;
		}
	}
	return true;	
}

