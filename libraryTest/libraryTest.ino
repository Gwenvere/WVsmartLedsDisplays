#include <moleGame.h>
#include <brushGame.h>
#include <baseLed.h>
#include <coloredBrush.h>
#include <simonGame.h>

int irPin = 1;
moleGame mole(irPin);
brushGame brush(irPin);
coloredBrush colorB(irPin);
simonGame simon(irPin);

void setup() {
  Serial.begin(9600);
}

void loop() {
   colorB.start(60000);
   //brush.start(10000);
  //simon.start(30000);


}
