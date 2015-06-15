
#ifndef USART_SPI_H
#define USART_SPI_H

#include <avr/io.h>



void usartxx_spi_init(USART_t *);


uint8_t usartxx_spi_readWrite(const uint8_t);




#endif /* USART_SPI_H */

