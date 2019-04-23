//Code om eerst een signaal op te vangen, en dat dan door te zenden

//geraakt na aantal iteraties out of sync, wss op te lossen door langere preamble


//eerst reader aanzetten, dan sender
//geen weerstand bij reader
//die LED's uit de arduino starter kit

int bps = 10; //Met huidige LED's, max 50
int analogPin = 1;
int val = 0;
int th = 470; //treshold
int bit_dly = 50; //delay
int bstr_dly = 10; //delay kleiner dan bij sender (of 0) 
int sample_dly;
int del = 5;

#define PIN 3 // pin we're sending on
#define NB_BITS 8
#define NB_SAMPLES 5 // must be uneven

bool bit_str_receive[NB_BITS];
bool samples[NB_SAMPLES];
bool previous_bit;
bool initial_run = 1;

//setup om te zenden
bool bit_str_send[NB_BITS];


const int buttonPin = 2;     // the number of the pushbutton pin
int buttonState = 0;  


//remove residual current
void clearPin(int pin) {
  for (int i = 0; i < 10; i ++)
    analogRead(pin);

}

/*bool read_preamble() {
  delay(bit_dly);
  for (int count = 0; count < preamble_length ; count ++) {
    bit_str[count] = read_bit();
    //Serial.print(bit_str[count]);
    //Serial.print(",");
  }
  //Serial.print("}");
  //Serial.print("\n");
  //delay(bstr_dly);
}
*/


bool readOne() { //preamble checken
  int val = analogRead(analogPin);
  //Serial.println(val);
  return val <= th;
}

void setup() {
  pinMode(PIN, OUTPUT);
  Serial.begin(9600);
  bit_dly = 1000/bps;
  // initialize samples array
  for (int i = 0; i < NB_SAMPLES; i++) {
    samples[i] = 0;
  }
  sample_dly = bit_dly / NB_SAMPLES;
  previous_bit = 0;
  clearPin(analogPin);
}

void loop() {
  delay(del);
  int val = analogRead(analogPin);
  Serial.println(val);
  
  delay(bit_dly);
  //Serial.println(analogRead(analogPin));
  if (readOne()) { //start signal bitstring
    delay(bit_dly); //maar half zo lang wachten, om te vermijden dat eerste bit wordt gemist
    read_bit_str(bit_str_receive);
    print_bit_str(bit_str_receive);
    //Serial.print(boolsToChar(bit_str));

    //stuurt zelde code weer uit, voorlopig met andere LED
    buttonState = digitalRead(buttonPin);
    send_bit_str(bit_str_receive);
    if (boolsToChar(bit_str_receive) == 'H'){
      Serial.print(" \n Eureka!!! \n");
      /*digitalWrite(PIN, HIGH);
      delay(1000);
      digitalWrite(PIN, LOW);*/
      /*buttonState = digitalRead(buttonPin);
      bool bit_str_H[] = {0,1,0,0,1,0,0,0};
      send_bit_str(bit_str_H);
      delay(bstr_dly);
      */
    } else {
      //Serial.print("Adjusting\n");
      //delay(bit_dly*1.2);
    }
  }
}



void read_bit_str(bool bit_str_receive[]) {
  //Serial.print("{");
  //delay(bit_dly);
  for (int count = 0; count < NB_BITS; count ++) {
    bit_str_receive[count] = read_bit();
    //Serial.print(bit_str_receive[count]);
    //Serial.print(",");
  }
  //Serial.print("}");
  //Serial.print("\n");
  delay(bstr_dly);
}

/* This function first determines the offset (via previous samples),
    then it collects the samples (taking this offset into account)
    and finally it returns the middle sample. */
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

void print_bit_str(bool bit_str[]) {
  for (int count = 0; count < NB_BITS; count ++) {
    if (bit_str_receive[count])
      Serial.print("1");
    else
      Serial.print("0");
  }
 Serial.println("");
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








void send_string(char* string) {
  for (int i = 0; i < strlen(string); i++) {
    char_to_bit_str(string[i], bit_str_send);
    send_bit_str(bit_str_send);
    print_bit_str(bit_str_send);
    Serial.print(string[i]);
  }
}

void char_to_bit_str(char c, bool bit_str_send[]) {
  for (int i = 0; i < NB_BITS; i++) {
    bit_str_send[NB_BITS - i - 1] = bitRead(c, i);
  }
}

void send_bit_str(bool bit_str_send[]) {
  led_high();
  delay(bit_dly);
  //Serial.print(sizeof(bit_str_send));
  //Serial.print(";");
  for (int i = 0; i < NB_BITS; i++) {
    if (bit_str_send[i])
      led_high();
    else
      led_low();
    delay(bit_dly);
  }
  led_low();
}


void led_high() {
  digitalWrite(PIN, HIGH);
}
void led_low() {
  digitalWrite(PIN, LOW);
}

bool charToBools(char c) {
  bool bits[8];
  for (int i = 0; i < 8; i++) {
    bool m  = bitRead(c, i);
    bits[8 - i - 1] = m;
  }
  return bits;

}

bool getInput(){
  bool input_str[NB_BITS];
  for (int count = 0; count < NB_BITS; count ++) {
    if (buttonState == LOW) { 
      input_str[count] = 0;
      delay(bit_dly);
    }
    else { 
      input_str[count] = 1;
      delay(bit_dly);
    }
  }
  return input_str;
}
