#include "Arduino.h"
#include "Send.h"

Send::Send(bool bit_str[], int bit_dly, int NB_BITS)
{
  pinMode(A1, OUTPUT); //sender PIN
  pinMode(2, INPUT); //buttonPin
  pinMode(1, OUTPUT); //inputIndicatorPin

  _bit_dly = 1000/10; // 1000/bps
  Serial.begin(9600);
}

void Send::send_bit_str(bool bit_str[]){
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

void Send::led_high(){
  analogWrite(A1, 255);
}

void Send::led_low(){
    analogWrite(A1, 0);
}
