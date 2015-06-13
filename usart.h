
#ifndef USART_H
#define USART_H

#include <avr/io.h>
#include "CircularBuffer.h"


typedef struct {
    
    CircularBuffer rxbuf;
    CircularBuffer txbuf;
    USART_t *usartxx;
    
} usartxx_t;


// To setup.
void usartxx_init(USART_t *, usartxx_t *);

// To write data out.
void usartxx_putchar(usartxx_t *, const char);

// To test for data in the RX buffer.
uint8_t usartxx_isData(usartxx_t *);
// To read one char from the RX buffer.
char usartxx_getchar(usartxx_t *);






#endif /* USART_H */

