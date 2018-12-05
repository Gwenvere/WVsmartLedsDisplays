#define PIN 3 // pin we're sending on
int bit_dly = 10000; //delay
int bstr_dly = 10000; //delay
#define NB_BITS 14
bool bit_str[NB_BITS];

void setup() {
  pinMode(PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  //bool test_bit_str[] = {1,1,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,0,1,0,1,0,1,0,0,1,1,0,1,1,0,0,0,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,0,0,1,1,1,0,0,0,0,1,1,0,1,1,1,1,1,0,0,1,1,0,0,1,0,1,0,0,1,1,1,1,0,1,1,1,1,1,1,1,0,0,0,1,0,1,1,0,0,0,1,0,0,1,0,0,1,0,0,0,1}; 
  //bool test_bit_str[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  bool test_bit_str[] = {1,1,0,1,1,1,0,1,0,0,1,0,1,0};
  send_bit_str(test_bit_str);
  delayMicroseconds(5000);

}


void send_string(char* string) {
  for (int i = 0; i < strlen(string); i++) {
    char_to_bit_str(string[i], bit_str);
    send_bit_str(bit_str);
    print_bit_str(bit_str);
    Serial.print(string[i]);
  }
}

void char_to_bit_str(char c, bool bit_str[]) {
  for (int i = 0; i < NB_BITS; i++) {
    bit_str[NB_BITS - i - 1] = bitRead(c, i);
  }
}

void send_bit_str(bool bit_str[]) {
  led_high();
  delayMicroseconds(10);
  Serial.print(sizeof(bit_str));
  Serial.print(";");
  for (int i = 0; i < NB_BITS; i++) {
    if (bit_str[i])
      led_high();
    else
      led_low();
    delayMicroseconds(bit_dly);
  }
  led_low();
}


void led_high() {
  digitalWrite(PIN, HIGH);
}
void led_low() {
  digitalWrite(PIN, LOW);
}

bool* charToBools(char c) {
  bool bits[8];
  for (int i = 0; i < 8; i++) {
    bool m  = bitRead(c, i);
    bits[8 - i - 1] = m;
  }
  return bits;

}

void print_bit_str(bool bit_str[]) {
  for (int count = 0; count < NB_BITS; count ++) {
    if (bit_str[count])
      Serial.print("1");
    else
      Serial.print("0");
  }
  Serial.println("");
}
