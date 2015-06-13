/*
 */ 


#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "stdioWrapper.h"
#include "usart.h"

static int usart_putchar(char, FILE *);
static int usart_getchar(FILE *);


static usartxx_t usartBuf;

static FILE usart_stdio = FDEV_SETUP_STREAM(usart_putchar, usart_getchar, _FDEV_SETUP_RW);


void stdioWrapper_init(USART_t * usartxx)
{
    
    // To setup the usart for stdio.
    usartxx_init(usartxx, &usartBuf);
    
    // To set both stdout and stdin.
    stdout = &usart_stdio;
    stdin  = &usart_stdio;
    
} // End of stdioWrapper_init().


/* <----- static fun() -----> */

static int usart_putchar(char c, FILE *stream)
{
    ///*
    if (c == '\n')
        usart_putchar('\r', stream);
    //*/
        
    // To wirte the char out to usart.
    usartxx_putchar(&usartBuf, c); 
    
    return 0;
    
} // End of usart_putchar();

static int usart_getchar(FILE *stream) 
{
    // Wait until data exists.
    while(!usartxx_isData(&usartBuf))
        ;
    
    return usartxx_getchar(&usartBuf);
    
} // End of uart_getchar().



