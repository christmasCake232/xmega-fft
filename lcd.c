

#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"
#include "usart_spi.h"


#define CMD_DISPLAY_OFF   0xAE
#define CMD_DISPLAY_ON    0xAF

#define CMD_SET_DISP_START_LINE  0x40
#define CMD_SET_PAGE  0xB0

#define CMD_SET_COLUMN_UPPER  0x10
#define CMD_SET_COLUMN_LOWER  0x00

#define CMD_SET_ADC_NORMAL  0xA0
#define CMD_SET_ADC_REVERSE 0xA1

#define CMD_SET_DISP_NORMAL 0xA6
#define CMD_SET_DISP_REVERSE 0xA7

#define CMD_SET_ALLPTS_NORMAL 0xA4
#define CMD_SET_ALLPTS_ON  0xA5
#define CMD_SET_BIAS_9 0xA2 
#define CMD_SET_BIAS_7 0xA3

#define CMD_RMW  0xE0
#define CMD_RMW_CLEAR 0xEE
#define CMD_INTERNAL_RESET  0xE2
#define CMD_SET_COM_NORMAL  0xC0
#define CMD_SET_COM_REVERSE  0xC8
#define CMD_SET_POWER_CONTROL  0x28
#define CMD_SET_RESISTOR_RATIO  0x20
#define CMD_SET_VOLUME_FIRST  0x81
#define  CMD_SET_VOLUME_SECOND  0
#define CMD_SET_STATIC_OFF  0xAC
#define  CMD_SET_STATIC_ON  0xAD
#define CMD_SET_STATIC_REG  0x0
#define CMD_SET_BOOSTER_FIRST  0xF8
#define CMD_SET_BOOSTER_234  0
#define  CMD_SET_BOOSTER_5  1
#define  CMD_SET_BOOSTER_6  3
#define CMD_NOP  0xE3
#define CMD_TEST  0xF0





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


