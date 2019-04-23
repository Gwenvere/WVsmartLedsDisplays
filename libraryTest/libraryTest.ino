#include <moleGame.h>

moleGame mole(1,2,3,1);
void setup() {
  Serial.begin(9600);
}

void loop() {
  mole.start();

}
