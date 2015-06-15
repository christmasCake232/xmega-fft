/*
 * Both the LCD and dataFlash are on USARTD0_SPI.
 * USARTD0_SPI passed the loopback test.
 * I have no idea how to set the clock rate!!
 */


#include <avr/io.h>
#include <stdio.h>

#include "usart_spi.h"



static inline void usartxx_spi_io_init(const USART_t *usartxx);


void usartxx_spi_init(USART_t *usartxx)
{
    // Setup USART_SPI GPIO.
    usartxx_spi_io_init(usartxx);
    
    // TODO: Update this.
    // Set USART baud rate.
    // Assuming F_CPU is fixed at 2MHz.
    // Baud Rate 9600 @ 2MHz.
    usartxx->BAUDCTRLB = 0; // BSCALE = 0 as well.
    usartxx->BAUDCTRLA = 12;
    
    // Set USART to Master SPI mode. 
    usartxx->CTRLC = USART_CMODE_MSPI_gc;
    
    // Turn on USART TX and RX. 
    usartxx->CTRLB = USART_RXEN_bm | USART_TXEN_bm;
    
} // End of usartxx_spi_init().


uint8_t usartxx_spi_readWrite(USART_t *usartxx, const uint8_t dataIn)
{
    // Clock the data out.
    usartxx->DATA = dataIn;
    
    // Block until data is clocked out and in.
    loop_until_bit_is_set(usartxx->STATUS, USART_TXCIF_bp);
    usartxx->STATUS = USART_TXCIF_bm;
    
    // Read the data clocked in.
    return usartxx->DATA;
    
} // End of usartxx_spi_readWrite().


/* <----- static inline fun() -----> */

static inline void usartxx_spi_io_init(const USART_t *usartxx)
{
    if(usartxx == &USARTD0) {
        // IO setup for USARTD0_SPI.
        
        // Set MOSI, SCK and SS as outputs.
        PORTD.DIRSET = (uint8_t)(_BV(3) | _BV(1) | _BV(0));
        // Pull SS high.
        PORTD.OUTSET = (uint8_t)(_BV(0));
        // Set MISO as an input.
        PORTD.DIRCLR = (uint8_t)(_BV(2));
        
    } else {
        // That USART_SPI is not supported!!
        // Add more as needed.
    }
        
} // End of usartxx_spi_io_init().

