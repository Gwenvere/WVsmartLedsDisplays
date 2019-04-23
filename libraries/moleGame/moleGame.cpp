#include "Arduino.h"
#include "moleGame.h"
#include "millisDelay.h"
#include <baseled.h>




#define COMMON_ANODE
millisDelay ledDelay;
millisDelay baseDelay;
millisDelay playDelayMole;
baseLed ledM(1,2,3,1);

moleGame::moleGame(int ir){
  _threshAdd = 15;
  _irPin = ir;
  
}

void moleGame::start(int playTime){

  ledM.setColor(0, 255, 0);
  Serial.begin(9600);
  ledM.clearIRpin();

	playDelayMole.start(playTime); //playTime in milliseconds
	while (!playDelayMole.isFinished()){  
	  int rand = random(0,25); //to randomize the red flashing
	  if (rand > 14){
		activated();
	  }
	  ledM.setColor(0, 255, 0); // Green Color
	  delay(800);
	}
	ledM.setColor(0, 0, 255); //blue to indicate that the game has ended
	delay(2000);
}

void moleGame::activated(){
  int threshold = ledM.getBaseValue() + _threshAdd;
  ledM.setColor(255, 255, 0); // mole
  /*Serial.println("Threshold:");
  Serial.println(threshold);
  Serial.println("");*/
  
  ledDelay.start(3000); //time the player has to disable the red led
  boolean notCorrected = true;
  while (!ledDelay.isFinished()&& notCorrected){
    int val = analogRead(_irPin);
    
    if (val > threshold){
      ledM.setColor(0,0,255); // Blue 
      delay(400);
      ledM.setColor(0,255,0);//Green
      notCorrected = false;
    }
  }
  if (notCorrected){
    flicker();
    ledM.setColor(0,255,0);
  }
}

void moleGame::flicker(){ //the flickering when the led was not disabled
  for ( int i = 0; i<5; i++){
    ledM.setColor(255,180,0);//orange
    delay(100);
    ledM.setColor(255,0,0);
    delay(100);
  }
}

