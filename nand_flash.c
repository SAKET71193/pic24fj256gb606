#include <xc.h> // include processor files - each processor file is guarded.  
#include <p24FJ256GB606.h>
#define HIGH 0
#define LOW 1
#define INPUT 1
#define OUTPUT 0
#define TRUE 1
#define FALSE 0

#define PIN_D0 2
#define PIN_D1 3
#define PIN_D2 4
#define PIN_D3 5
#define PIN_C4 6
#define PIN_C5 7
#define PIN_C6 8
#define PIN_C7 9


#define PIN_F0 22 // not connected actually
#define PIN_F4 23
#define PIN_F3 24
#define PIN_B1 25
#define PIN_B0 26
#define PIN_B3 27
#define PIN_B2 28

int io_pins[] = { PIN_D0 , PIN_D1 , PIN_D2 , PIN_D3 , PIN_C4 , PIN_C5 , PIN_C6 , PIN_C7  }; // workshop
int io_pinslen = sizeof(io_pins)/sizeof(io_pins[0]);

void digitalWrite(uint8_t port ,uint8_t pin_num,uint8_t state)
{
    switch(port)
    {
        case 0: LATB |= (state << pin_num); break;
    }
}

uint8_t digitalRead(uint8_t port,uint8_t pin_num)
{

}

void pinMode(uint8_t port,uint8_t pin_num  , uint8_t mode)
{

}

/* set the global pin array to mode, with or without pullsups */
void io_pin_mode(uint8_t port,uint8_t mode, uint8_t pullups)
{
    uint8_t i=0;
  for (i = 0; i < io_pinslen; i++) {
    pinMode(port,io_pins[i], mode);
    if (mode == INPUT && pullups == TRUE)
      digitalWrite(port,io_pins[i], HIGH);
  }
}
void io_pin_mode1(uint8_t port,uint8_t mode)
{
  io_pin_mode(port,mode, FALSE);
}
// 8bit
void io_write_byte(uint8_t port,uint8_t b)
{
    uint8_t i;
  for (i = 0; i < 8; i++) {
    digitalWrite(port,io_pins[i], (b >> i ) & 1 );
  }
}

uint8_t io_read_byte()
{
  uint8_t ret = 0,i,port;
  int val;
  for(i = 0; i < 8; i++) {
    val = digitalRead(port,io_pins[i]);
    ret |= val << i;
  }
  return ret;
}
