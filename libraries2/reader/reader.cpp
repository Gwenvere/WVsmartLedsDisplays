//eerst reader aanzetten, dan sender
//geen weerstand bij reader
//die LED's uit de arduino starter kit
#include "Arduino.h"
#include <reader.h>



#define NB_BITS 4
#define NB_SAMPLES 5 // must be uneven




reader::reader(int ir){
	val = 0;
	th = 0; //treshold
	bit_dly = 20; //delay
	bstr_dly = 100; //delay kleiner dan bij sender (of 0) 
	//del = 5;
	analogPin = ir;
	Serial.begin(9600);
	initial_run = 1;
	// initialize samples array
	for (int i = 0; i < NB_SAMPLES; i++) {
		samples[i] = 0;
	}
	sample_dly = bit_dly / NB_SAMPLES;
	previous_bit = 0;
	clearPin(analogPin);
}	
//remove residual current
void reader::clearPin(int pin) {
  for (int i = 0; i < 10; i ++)
    analogRead(pin);

}

bool reader::readOne() {
  int val = analogRead(analogPin);
  //Serial.println(val);
  return val <= th;
}

bool reader::readPre() { //preamble checken
  int val = analogRead(analogPin);
  //Serial.println(val);
  int ones = 0;
  bool bit;
  while (ones < 3){ //minstens 3 enen
  //Serial.println(val);
	bit = read_bit();
	  if (bit == true){ ones++;
	  Serial.print("Ones: ");
	  Serial.println(ones);
	  }
	  else if ((bit == false)&&(ones == 3)){ 
	  return true;
	  }
	  else if ((bit == false)&&(ones != 3)){ 
	  ones = 0;
	  }
	  //delay(bit_dly);
	  else{ Serial.println("Huh?!");}
  }
  
}



void reader::start(bool bit_str[], int threshold) {
	th = threshold;
	while (!readPre());  //wait until preamble is read
	delay(bit_dly);
	read_bit_str(bit_str);
	print_bit_str(bit_str);

	return;
 
}



void reader::read_bit_str(bool bit_str[]) {
  //Serial.print("{");
  //delay(10);
  for (int count = 0; count < NB_BITS; count ++) {
    bit_str[count] = read_bit();
    //Serial.print(bit_str[count]);
    //Serial.print(",");
  }
  //Serial.print("}");
  //Serial.print("\n");
  //delay(bstr_dly);
}

/* This function first determines the offset (via previous samples),
    then it collects the samples (taking this offset into account)
    and finally it returns the middle sample. */
bool reader::read_bit() {
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

void reader::print_bit_str(bool bit_str[]) {
  for (int count = 0; count < NB_BITS; count ++) {
    if (bit_str[count])
      Serial.print("1");
    else
      Serial.print("0");
  }
  Serial.println("");
}



void reader::print_samples() {
  Serial.print("[");
  for (int i = 0; i < NB_SAMPLES; i++) {
    Serial.print(samples[i]);
  }
  Serial.println("]");
}


//sauce: https://www.daniweb.com/programming/software-development/threads/158909/converting-bool-8-to-char
char reader::boolsToChar(bool bools[]) {
  int c = 0;
  for ( int i = 0; i < 8; i++ )
    if ( bools[i] ) {
      c +=  (int) (pow(2, 8 - i - 1) + 0.5);
    }
  return (char)c;
}
