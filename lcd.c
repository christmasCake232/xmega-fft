/*
 * reference: https://github.com/adafruit/ST7565-LCD
 * 
 * 
 */


#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"
#include "usart_spi.h"
#include "ST7565R.h"

#define PAGE_COUNT 4
#define COLUMN_COUNT 128



static inline void strobeReset(void);
static inline void sendCmd(const uint8_t);
static inline void sendData(const uint8_t);
static inline void setBrightness(uint8_t);

static inline uint8_t foo(const uint8_t, const uint8_t);



void lcd_init(void)
{
    
    usartxx_spi_init(&USARTD0);
    
    
    strobeReset();
    
    // LCD bias select 
    sendCmd(CMD_SET_BIAS_9);
     
    // ADC select 
    sendCmd(CMD_SET_ADC_NORMAL);
     
    // SHL select 
    sendCmd(CMD_SET_COM_NORMAL);
     
    // Initial display line 
    sendCmd(CMD_SET_DISP_START_LINE);
    
    // turn on voltage converter (VC=1, VR=0, VF=0) 
    sendCmd(CMD_SET_POWER_CONTROL | 0x4);
     
    // wait for 50% rising 
    _delay_ms(50);
     
    // turn on voltage regulator (VC=1, VR=1, VF=0) 
    sendCmd(CMD_SET_POWER_CONTROL | 0x6);
    
    // wait >=50ms 
    _delay_ms(50);
    
    // turn on voltage follower (VC=1, VR=1, VF=1) 
    sendCmd(CMD_SET_POWER_CONTROL | 0x7); 
    // wait 
    _delay_ms(10);
     
    // set lcd operating voltage (regulator resistor, ref voltage resistor) 
    sendCmd(CMD_SET_RESISTOR_RATIO | 0x2);    
    
    // ----
    
    sendCmd(CMD_DISPLAY_ON);
    
    
    sendCmd(CMD_SET_ALLPTS_NORMAL);
    
    setBrightness(0x18);
    
       
    lcd_clearScreen();
    
    
} // End of lcd_init().



void lcd_clearScreen(void) 
{
    uint8_t p = 0;
    uint8_t c = 0;
    
    for(p = 0; p < PAGE_COUNT; ++p)
    {
        sendCmd(CMD_SET_PAGE | p);
        for(c = 0; c < COLUMN_COUNT; ++c)
        {
            sendCmd(CMD_SET_COLUMN_LOWER | (c & 0x0F));
            sendCmd(CMD_SET_COLUMN_UPPER | ((c >> 4) & 0x0F));
            
            sendData(0x00);
        }
    }
    
} // End of lcd_clearScreen().






void lcd_barGraph(uint8_t *arr)
{
    uint8_t p = 0;
    uint8_t c = 0;
    
    for(p = 0; p < PAGE_COUNT; ++p)
    {
        sendCmd(CMD_SET_PAGE | p);
        for(c = 0; c < COLUMN_COUNT; ++c)
        {
            sendCmd(CMD_SET_COLUMN_LOWER | (c & 0x0F));
            sendCmd(CMD_SET_COLUMN_UPPER | ((c >> 4) & 0x0F));
            
            sendData(foo(arr[c], p));
            
        }
    }
    
} // End of lcd_barGraph().


/* <----- static inline fun() -----> */

static inline void strobeReset(void)
{
    PORTF.OUTCLR = (uint8_t)(_BV(3));
    PORTA.OUTCLR = (uint8_t)(_BV(3));
    
    _delay_ms(50);
    
    PORTA.OUTSET = (uint8_t)(_BV(3));
    
} // End of strobeReset().



static inline void sendCmd(const uint8_t c)
{
        
    PORTD.OUTCLR = (uint8_t)(_BV(0));
    
    // SS low.
    PORTF.OUTCLR = (uint8_t)(_BV(3));
    
    // Set the data to the LCD.
    usartxx_spi_readWrite(&USARTD0, c);
    
    // SS high
    PORTF.OUTSET = (uint8_t)(_BV(3));
    
} // End of sendCmd().

static inline void sendData(const uint8_t c)
{
    PORTD.OUTSET = (uint8_t)(_BV(0));
    
    // SS low.
    PORTF.OUTCLR = (uint8_t)(_BV(3));
    
    // Set the data to the LCD.
    usartxx_spi_readWrite(&USARTD0, c);
    
    // SS high
    PORTF.OUTSET = (uint8_t)(_BV(3));
    
} // End of sendData().

static inline void setBrightness(uint8_t val)
{
    sendCmd(CMD_SET_VOLUME_FIRST);
    sendCmd(CMD_SET_VOLUME_SECOND | (val & 0x3F));
    
} // End of setBrightness().


static inline uint8_t foo(const uint8_t d, const uint8_t p)
{
    static const uint8_t fooTable[] = {0, 1, 3 , 7, 15, 31, 63, 127};
    
    switch(p)
    {
        case 0:
            if(d > 7)
                return 0xFF;
            else 
                return fooTable[d];
            
        case 1:
            if(d > 15)
                return 0xFF;
            else if(d > 8)
                return fooTable[d -8];
            else 
                return 0;
            
        case 2:
            if(d > 23)
                return 0xFF;
            else if(d > 16)
                return fooTable[d -16];
            else 
                return 0;
                
        case 3:
            if(d > 31)
                return 0xFF;
            else if(d > 24)
                return fooTable[d -24];
            else 
                return 0;
        
        default:
            return 0;
    }
}



