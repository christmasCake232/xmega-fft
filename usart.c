/*
 * Add support for interrupts.
 * Add buffers for both TX and RX.
 */


#include <avr/io.h>

#include "usart.h"
#include "CircularBuffer.h"


static inline void usartxx_io_init(const USART_t *usartxx);


void usartxx_init(USART_t *usartxx, usartxx_t * usartBuf)
{
    
    usartBuf->usartxx = usartxx;
    
    // Setup both the rx and tx buffer for usartxx.
    setup_cb(&(usartBuf->rxbuf));
    setup_cb(&(usartBuf->txbuf));
    
    
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
    //usartxx->CTRLB = USART_RXEN_bm | USART_TXEN_bm;
    usartxx->CTRLB = USART_TXEN_bm;
    
} // End of usartxx_init().


void usartxx_putchar(usartxx_t * usartBuf, const char c)
{
    loop_until_bit_is_set(usartBuf->usartxx->STATUS, USART_DREIF_bp);
    usartBuf->usartxx->DATA = c;
    
} // End of usartxx_putchar().



uint8_t usartxx_isData(usartxx_t *usartBuf)
{
    return bit_is_set(usartBuf->usartxx->STATUS, USART_RXCIF_bp); 
}

// This will not block and will stale data.
char usartxx_getchar(usartxx_t *usartBuf)
{
    return usartBuf->usartxx->DATA;
}


/* <----- static inline fun() -----> */

static inline void usartxx_io_init(const USART_t *usartxx)
{
    // Set TX pin as output and enable pull-up.
    // Set RX pin as input and disable pull-up.
    
    
    if(usartxx == &USARTC0) {
        // IO setup for USARTC0.
        
        // TX pin at output.
        PORTC.OUTSET = PIN3_bm;
        // Enable TX pin pull-up.
        PORTC.DIRSET = PIN3_bm;
        // Disable RX pin pull-up.
        PORTC.DIRCLR = PIN2_bm;
        
    } else {
        // That USART is not supported!!
        // Add more as needed.
    }

    
} // End of usartxx_io_init().

