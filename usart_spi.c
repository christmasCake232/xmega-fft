/*
 * Both the LCD and dataFlash are on USARTD0_SPI.
 */


#include <avr/io.h>

#include "usart_spi.h"



static inline void usartxx_io_init(const USART_t *usartxx);


void usartxx_spi_init(USART_t *usartxx)
{
    
    // Setup USART GPIO.
    usartxx_io_init(usartxx);
    
    
    // Set USART baud rate.
    // Assuming F_CPU is fixed at 2MHz.
    // Baud Rate 9600 @ 2MHz.
    usartxx->BAUDCTRLB = 0; // BSCALE = 0 as well.
    usartxx->BAUDCTRLA = 12;
    
    // Setup USART frame format and size.
    // No stop bit.
    usartxx->CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_PMODE_DISABLED_gc | USART_CHSIZE_8BIT_gc;
    
    // Turn on USART TX and RX. 
    usartxx->CTRLB = USART_RXEN_bm | USART_TXEN_bm;
    
} // End of usartxx_init().


uint8_t usartxx_spi_readWrite(const uint8_t dataIn)
{
    return 0;
}


/* <----- static inline fun() -----> */

static inline void usartxx_io_init(const USART_t *usartxx)
{



    
} // End of usartxx_io_init().

