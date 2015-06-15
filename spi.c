/*
 * SPID uses the same pins as USB, POWER LED and STATUS LED.
 * SPID should not be used. 
 * 
 * SPIC passed the loopback test.
 * 
 */

#include <avr/io.h>

#include "spi.h"



static inline void spix_io_init(const SPI_t *);


void spix_init(SPI_t *spix)
{
    spix_io_init(spix);
    
    spix->CTRL = SPI_ENABLE_bm | SPI_MASTER_bm | SPI_MODE_1_gc | SPI_PRESCALER_DIV128_gc;
    
} // End of spix_init().


uint8_t spix_readWrite(SPI_t *spix, const uint8_t dataOut)
{
    spix->STATUS = SPI_IF_bm;
    
    // Clock the data out.
    spix->DATA = dataOut;
    
    // Block until data is clocked out and in.
    loop_until_bit_is_set(spix->STATUS, SPI_IF_bp);
    
    // Read the data clocked in.
    return spix->DATA;
    
} // End of spix_readWrite().


/* <----- static inline fun() -----> */

static inline void spix_io_init(const SPI_t *spix)
{
    
    if(spix == &SPID) {
        
        // MOSI, SCK, SS.
        PORTD.DIRSET = (uint8_t)(_BV(3) | _BV(1) | _BV(0));
        // Pull SS high.
        PORTD.OUTSET = (uint8_t)(_BV(0));
        // MISO.
        PORTD.DIRCLR = (uint8_t)(_BV(2));
    
    } else if(spix == &SPIC) {
        
        // SCK, MOSI, SS
        PORTC.DIRSET = (uint8_t)(_BV(7) | _BV(5) | _BV(4));
        // Pull SS high.
        PORTD.OUTSET = (uint8_t)(_BV(4));
        // MISO.
        PORTC.DIRCLR = (uint8_t)(_BV(6));
        
    } else {
        // That SPI is not supported!!
        // Add more as needed.
    }
    
} // End of spix_io_init().
