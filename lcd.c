

#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"
#include "usart_spi.h"





static inline void strobeReset(void);

static inline void sendCmd(const uint8_t);
static inline void sendData(const uint8_t);
static inline void send(const uint8_t);




static USART_t *spiPort = &USARTD0;


void lcd_init(void)
{
    
    usartxx_spi_init(spiPort);
    strobeReset();
    
 
    
    // Set the LCD bias to 1/7th;
    sendCmd(0xA3);
    
    
    // Horizontally "normal" (not flipped)
    // ADC normal.
    sendCmd(0xA0);
    
    //SET_COM_NORMAL 
    sendCmd(0xC0);
    
    //SET_DISP_START_LINE
    sendCmd(0x40);
    
    sendCmd(0x28 | 0x04);
    _delay_ms(50);
    
    sendCmd(0x28 | 0x06);
    _delay_ms(50);
    
    sendCmd(0x28 | 0x07);
    _delay_ms(50);
    
    sendCmd(0x20 | 0x06);
    
    sendCmd(0xAF);
    sendCmd(0xA4);
    
    
    
    
    
    
    // Command 0x40: go back to the top left of the display
    sendCmd(0x40);
    
    uint16_t i = 0;
    
    for(i = 0; i < 1024; ++i)
    {
        sendData(0x80);
    }
    
    for(;;);
    
    
} // End of lcd_init().




    




/* <----- static inline fun() -----> */

static inline void strobeReset(void)
{
    PORTA.OUTCLR = (uint8_t)(_BV(3));
    
    _delay_ms(500);
    
    PORTA.OUTSET = (uint8_t)(_BV(3));
    
} // End of strobeReset().


static inline void sendCmd(const uint8_t data)
{
    PORTD.OUTCLR = (uint8_t)(_BV(0));
    
    sendData(data);
    
} // End of sendData().

static inline void sendData(const uint8_t data)
{
    PORTD.OUTSET = (uint8_t)(_BV(0));
    
    sendData(data);
    
} // End of sendData().

static inline void send(const uint8_t data)
{
    // SS low.
    PORTF.OUTCLR = (uint8_t)(_BV(3));
    
    // Set the data to the LCD.
    usartxx_spi_readWrite(spiPort, data);
    
    // SS high
    PORTF.OUTSET = (uint8_t)(_BV(3));
    
} // End of send();


