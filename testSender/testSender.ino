#define PIN 3 // pin we're sending on
int bit_dly = 50; //delay
int bstr_dly = 100; //delay
int bps = 10;
#define NB_BITS 10
bool bit_str[] = {1,1,1,1,0,0,0,0,1,0};

void setup() {
  pinMode(3, OUTPUT);
  
  bit_dly = 1000/bps;
  Serial.begin(9600);
}

void loop() {
  send_bit_str(bit_str);
  delay(bstr_dly);
}

void send_bit_str(bool bit_str[]) {
  led_high();
  delay(bit_dly);
  for (int i = 0; i < NB_BITS; i++) {
    if (bit_str[i])
      led_high();
    else
      led_low();
    delay(bit_dly);
  }
  led_low();
}


void led_high() {
  //digitalWrite(PIN, HIGH);
  //connect the sending led to the analog pin A1
  analogWrite(3, 255);
}
void led_low() {
  //digitalWrite(PIN, LOW);
    analogWrite(3, 0);
}
