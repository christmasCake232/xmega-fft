

#ifndef SYSTEM_H
#define SYSTEM_H

#include <avr/io.h>

#define SW0_IS_SET() (!(PORTE.IN & (_BV(5))))
#define SW1_IS_SET() (!(PORTF.IN & (_BV(1))))
#define SW2_IS_SET() (!(PORTF.IN & (_BV(2))))

#define LED_POWER_ON()      do { PORTD.OUTSET = (uint8_t)(_BV(5)); } while(0)
#define LED_POWER_OFF()     do { PORTD.OUTCLR = (uint8_t)(_BV(5)); } while(0)
#define LED_POWER_TGL()     do { PORTD.OUTTGL = (uint8_t)(_BV(5)); } while(0)

#define LED_STATUS_ON()     do { PORTD.OUTCLR = (uint8_t)(_BV(4)); } while(0)
#define LED_STATUS_OFF()    do { PORTD.OUTSET = (uint8_t)(_BV(4)); } while(0)
#define LED_STATUS_TGL()    do { PORTD.OUTTGL = (uint8_t)(_BV(4)); } while(0)

#define LED_0_ON()          do { PORTR.OUTCLR = (uint8_t)(_BV(0)); } while(0)
#define LED_0_OFF()         do { PORTR.OUTSET = (uint8_t)(_BV(0)); } while(0)
#define LED_0_TGL()         do { PORTR.OUTTGL = (uint8_t)(_BV(0)); } while(0)

#define LED_1_ON()          do { PORTR.OUTCLR = (uint8_t)(_BV(1)); } while(0)
#define LED_1_OFF()         do { PORTR.OUTSET = (uint8_t)(_BV(1)); } while(0)
#define LED_1_TGL()         do { PORTR.OUTTGL = (uint8_t)(_BV(1)); } while(0)

#define LED_BACKLIGHT_ON()  do { PORTE.OUTSET = (uint8_t)(_BV(4)); } while(0)
#define LED_BACKLIGHT_OFF() do { PORTE.OUTCLR = (uint8_t)(_BV(4)); } while(0)
#define LED_BACKLIGHT_TGL() do { PORTE.OUTTGL = (uint8_t)(_BV(4)); } while(0)





void system_clocks_init(void);

void system_gpio_init(void);

void system_enterSleep(void);



#endif /* SYSTEM_H */
