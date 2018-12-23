//drukknop toegevoegd, de reeks wordt enkel verstuurd indien de drukknop niet wordt ingeduwt
//na loslaten van de knop begint reeks terug van begin

#define PIN 3 // pin we're sending on
int bit_dly = 50; //delay
int bstr_dly = 100; //delay
int bps = 10;
#define NB_BITS 8
bool bit_str[] = {0,1,0,0,1,0,0,0};
bool bit_str_H[] = {0,1,0,0,1,0,0,0};


const int buttonPin = 2;     // the number of the pushbutton pin
const int inputIndicatorPin = 1;
int buttonState = 0;  

void setup() {
  pinMode(PIN, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(inputIndicatorPin, OUTPUT);

  
  bit_dly = 1000/bps;
  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  //enkel zenden als knop niet is ingedrukt
  if (buttonState == LOW) { 
   

    send_bit_str(bit_str);
    delay(bstr_dly);
  }
  else if (buttonState == HIGH) {
    getInput();
  
  }
  

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
  delay(bit_dly);
 /* led_high();
  delay(bit_dly);
  led_high();
  delay(bit_dly);
  led_high();
  delay(bit_dly); */
  //Serial.print(sizeof(bit_str));
  //Serial.print(";");
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

void print_bit_str(bool bit_str[]) {
  for (int count = 0; count < NB_BITS; count ++) {
    if (bit_str[count])
      Serial.print("1");
    else
      Serial.print("0");
  }
  Serial.println("");
}

void getInput(){
  digitalWrite(inputIndicatorPin, HIGH);
  for (int count = 0; count < NB_BITS; count ++) {

    //flash to indicate new bit
    digitalWrite(inputIndicatorPin, LOW);
    delay(50);
    digitalWrite(inputIndicatorPin, HIGH);
    
    if (buttonState == LOW) {
      digitalWrite(PIN, LOW); 
      bit_str[count] = 0;
      delay(bit_dly * 10);
    }
    else if (buttonState == HIGH) {
      digitalWrite(PIN, LOW); 
      bit_str[count] = 1;
      delay(bit_dly * 10);
    }
  }
  digitalWrite(inputIndicatorPin, LOW);
  delay(600);
  return;
}
