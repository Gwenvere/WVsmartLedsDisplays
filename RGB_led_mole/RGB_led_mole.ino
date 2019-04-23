int redPin= 1;
int greenPin = 2;
int bluePin = 3;
int prevAverage = 999;
int theshAdd = 15;

#define NB_BITS 8
#define NB_SAMPLES 5 // must be uneven
#define NB_CHECKS 3

bool preamble[] = {0,1,0,1,0,1,0,1};
int program = 1;
int threshold;
bool bit_str_receive[NB_BITS];
bool samples[NB_SAMPLES];
bool previous_bit;
bool initial_run = 1;
int sample_dly;
int bit_dly;

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
  sample_dly = bit_dly / NB_SAMPLES;

  Serial.begin(9600);
  clearPin(irPin);
  delay(100);
  
}
void loop() {
  int checks = NB_CHECKS;
  while (checks > 0) {
    if (checkPreamble()) {
      changeProgram();
     break;
    }
    checks--;
  }
  if (program == 1) {
    mole();
  }
  delay(800);
}

void mole() {
  // int val = analogRead(irPin);
 // Serial.println(val);
  int rand = random(0,20);
  //Serial.println(rand);
  if (rand > 15){
    activated();
  }
 
  setColor(0, 255, 0); // Green Color
}

void activated(){

  threshold = getBaseValue() + theshAdd;
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

bool checkPreamble() {
  for (int count = 0; count < NB_BITS; count ++) {
    if (preamble[count] != read_bit()) {
      return false;
    }
  }
  return true;
}

void changeProgram() {
  program = 0;
  bool programBits[2];
  programBits[0] = read_bit();
  programBits[1] = read_bit();
  if (programBits[0]) {
    program += 2;
  }
  if (programBits[1]) {
    program += 1;
  }
}

bool read_bit() {
  int wrong_samples_offset = 0;
  if (initial_run) {
    // Don't check previous samples for syncing
    initial_run = 0;
  } else {
    // Determine max number of wrong samples, based on number of samples
    int nb_allowed_errors = (int) (NB_SAMPLES - 1) / 2;

    // Check whether first or last part has wrong samples.
    // If first sample is wrong check first part and determine offset
    if (samples[0] != previous_bit) {
      wrong_samples_offset++;
      for (int i = 1; i < nb_allowed_errors; i++) {
        if (samples[i] != previous_bit)
          wrong_samples_offset++;
        else
          break;
      }
    }
    // If last sample is wrong, check last part and determine offset
    else if (samples[NB_SAMPLES - 1] != previous_bit) {
      wrong_samples_offset--;
      for (int i = NB_SAMPLES - 2; i < nb_allowed_errors; i--) {
        if (samples[i] != previous_bit)
          wrong_samples_offset--;
        else
          break;
      }
    }
  }

  //Serial.println(wrong_samples_offset);

  // last part has wrong samples
  if (wrong_samples_offset < 0) {
    // Write previous last wrong samples into first current samples
    for (int i = 0; i < -wrong_samples_offset; i++)
      samples[i] = samples[NB_SAMPLES + wrong_samples_offset - 1];
    // Collect rest of current samples
    for (int i = -wrong_samples_offset; i < NB_SAMPLES; i++) {
      samples[i] = readOne();
      delay(sample_dly);
    }
  }
  else {
    // if first part has wrong samples, wait.
    if (wrong_samples_offset > 0) {
      //for (int i = 0; i < wrong_samples_offset; i++)
      //  delay(sample_dly);
      delay(sample_dly * wrong_samples_offset);
    }
    // Collect current samples
    for (int i = 0; i < NB_SAMPLES; i++) {
      samples[i] = readOne();
      delay(sample_dly);
    }
  }
  // Accept middle sample as read bit and return it.
  previous_bit = samples[(int) NB_SAMPLES / 2];
  return previous_bit;
}

bool readOne() { //preamble checken
  int val = analogRead(irPin);
  return val >= threshold;
}

//remove residual current
void clearPin(int pin) {
  for (int i = 0; i < 10; i ++)
    analogRead(pin);

}
