/* 
 Library for sending led  
 */

#ifndef Send_h
#define Send_h

#include "Arduino.h"

class Send
{
  public:
   Send(bool bit_str[], int bit_dly, int NB_BITS);
   void send_bit_str(bool bit_str[]);
  private:
    void led_high();
    void led_low();
    int _pin;
};

#endif
