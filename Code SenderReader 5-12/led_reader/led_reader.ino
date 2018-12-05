int analogPin = 1;
int val = 0;
int th = 750; //treshold
int bit_dly = 10000; //delay
int bstr_dly = 1000; //delay
int sample_dly;

#define NB_BITS 14
#define NB_SAMPLES 5 // must be uneven

boolean bit_str[NB_BITS];
boolean samples[NB_SAMPLES];
boolean previous_bit;
boolean initial_run = 1;

void setup() {
  Serial.begin(9600);
  // initialize samples array
  for (int i = 0; i < NB_SAMPLES; i++) {
    samples[i] = 0;
  }
  sample_dly = bit_dly / NB_SAMPLES;
  previous_bit = 0;
  clearPin(analogPin);
}

void loop() {
  //delayMicroseconds(5);
  //Serial.println(analogRead(analogPin));
  if (readOne()) { //start signal bitstring
    read_bit_str(bit_str);
    print_bit_str(bit_str);
    //Serial.print(boolsToChar(bit_str));
  }
}

//remove residual current
void clearPin(int pin) {
  for (int i = 0; i < 10; i ++)
    analogRead(pin);

}

void read_bit_str(boolean bit_str[]) {
  Serial.print("{");
  delayMicroseconds(10);
  for (int count = 0; count < NB_BITS; count ++) {
    bit_str[count] = read_bit();
    //Serial.print(bit_str[count]);
    //Serial.print(",");
  }
  //Serial.print("}");
  //Serial.print("\n");
  delayMicroseconds(bstr_dly);
}

/* This function first determines the offset (via previous samples),
    then it collects the samples (taking this offset into account)
    and finally it returns the middle sample. */
boolean read_bit() {
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
      delayMicroseconds(sample_dly);
    }
  }
  else {
    // if first part has wrong samples, wait.
    if (wrong_samples_offset > 0) {
      //for (int i = 0; i < wrong_samples_offset; i++)
      //  delayMicroseconds(sample_dly);
      delayMicroseconds(sample_dly * wrong_samples_offset);
    }
    // Collect current samples
    for (int i = 0; i < NB_SAMPLES; i++) {
      samples[i] = readOne();
      delayMicroseconds(sample_dly);
    }
  }
  // Accept middle sample as read bit and return it.
  previous_bit = samples[(int) NB_SAMPLES / 2];
  return previous_bit;
}

void print_bit_str(boolean bit_str[]) {
  for (int count = 0; count < NB_BITS; count ++) {
    if (bit_str[count])
      Serial.print("1");
    else
      Serial.print("0");
  }
  Serial.println("");
}

boolean readOne() {
  int val = analogRead(analogPin);
  //Serial.println(val);
  return val >= th;
}

void print_samples() {
  Serial.print("[");
  for (int i = 0; i < NB_SAMPLES; i++) {
    Serial.print(samples[i]);
  }
  Serial.println("]");
}


//sauce: https://www.daniweb.com/programming/software-development/threads/158909/converting-bool-8-to-char
char boolsToChar(bool bools[]) {
  int c = 0;
  for ( int i = 0; i < 8; i++ )
    if ( bools[i] ) {
      c +=  (int) (pow(2, 8 - i - 1) + 0.5);
    }
  return (char)c;
}
