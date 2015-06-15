


#include <avr/io.h>

#include "system.h"


 // I can't find the names for the pins. 
void system_gpio_init(void)
{
    // SW setup.
    // Set the SW0 pin as an input. 
    PORTE.DIRCLR = (uint8_t)(_BV(5));
    // Set both SW2 and SW1 as inputs.
    PORTF.DIRCLR = (uint8_t)(_BV(2) | _BV(1));
    
    
    // LED setup.
    // Set both POWER LED and STATUS LED pins as outputs.
    PORTD.DIRSET = (uint8_t)(_BV(5) | _BV(4));
    // To turn on the POWER LED.
    PORTD.OUTSET = (uint8_t)(_BV(5)); 
    // Set both LED1 and LED0 pins as outputs.
    PORTR.DIRSET = (uint8_t)(_BV(1) | _BV(0));
    
    
    // Data Flash setup.
    // Set SS/CS as an output and pull it high.
    PORTF.DIRSET = (uint8_t)(_BV(4));
    PORTF.OUTSET = (uint8_t)(_BV(4));
    
    
    // LCD setup.
    // Set SS/CS as an output and pull it high.
    PORTF.DIRSET = (uint8_t)(_BV(3));
    PORTF.OUTSET = (uint8_t)(_BV(3));
    // Set RS as an output and pull it high. 
    PORTD.DIRSET = (uint8_t)(_BV(0));
    PORTD.OUTSET = (uint8_t)(_BV(0));
    // Set LCD reset as an output and pull it high.
    PORTA.DIRSET = (uint8_t)(_BV(3));
    PORTA.OUTSET = (uint8_t)(_BV(3));
    // LCD backlight setup.
    PORTE.DIRSET = (uint8_t)(_BV(4));
    PORTE.OUTSET = (uint8_t)(_BV(4));

} // End of system_gpio_init().
