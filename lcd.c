/*
 * ST7565R
 * 
 */


#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"
#include "ST7565R.h"
#include "usart_spi.h"








static inline void strobeReset(void);

static inline void sendCmd(const uint8_t);
static inline void sendData(const uint8_t);
static inline void send(const uint8_t);




static USART_t *spiPort = &USARTD0;


void lcd_init(void)
{
    
    //usartxx_spi_init(spiPort);
    strobeReset();
    
 
    sendCmd(CMD_SET_BIAS_7);
    
    sendCmd(CMD_SET_ADC_NORMAL);
    
    sendCmd(CMD_SET_COM_NORMAL);
    
    sendCmd(CMD_SET_DISP_START_LINE);
    
    
    sendCmd(CMD_SET_POWER_CONTROL | 0x4);
    _delay_ms(50);
    
    sendCmd(CMD_SET_POWER_CONTROL | 0x6);
    _delay_ms(50);
    
    sendCmd(CMD_SET_POWER_CONTROL | 0x7);
    _delay_ms(10);
    
    sendCmd(CMD_SET_RESISTOR_RATIO | 0x6);
    
    // ------------------
    
    sendCmd(CMD_DISPLAY_ON);
    
    sendCmd(CMD_SET_ALLPTS_NORMAL);
    
    // set brightness
    sendCmd(CMD_SET_VOLUME_FIRST);
    sendCmd(CMD_SET_VOLUME_SECOND | (0x18 & 0x3f));
    
    
    
    
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

inline void spiwrite(uint8_t c) {
    
    uint8_t i;
    
    for(i = 7; i >= 0; --i)
    {
        // SCLK low. 
        PORTD.OUTCLR = _BV(1);
        
        if(c & _BV(i))
            PORTD.OUTSET = _BV(3);
        else 
            PORTD.OUTSET =_BV(3);
        
        PORTD.OUTSET = _BV(1);
    }
    
}



static inline void send(const uint8_t data)
{
    // SS low.
    PORTF.OUTCLR = (uint8_t)(_BV(3));
    
    // Set the data to the LCD.
    usartxx_spi_readWrite(spiPort, data);
    //spiwrite(data);
    
    // SS high
    PORTF.OUTSET = (uint8_t)(_BV(3));
    
} // End of send();


