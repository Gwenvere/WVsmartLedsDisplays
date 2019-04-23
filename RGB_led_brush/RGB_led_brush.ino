int redPin= 1;
int greenPin = 2;
int bluePin = 3;

int irPin = 1;
#define COMMON_ANODE

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  setColor(255, 0, 0);

  Serial.begin(9600);
  clearPin(irPin);
  
}
void loop() {
  int val = analogRead(irPin);
  Serial.println(val);

  if (val > 420){
    setColor(255, 0, 0); // Red Color
    delay(1000);
  }
  setColor(0, 255, 0); // Green Color

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
