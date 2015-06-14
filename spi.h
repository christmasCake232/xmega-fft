


#ifndef SPIX_H
#define SPIX_H


#include <avr/io.h>

void spix_init(SPI_t *);


uint8_t spix_readWrite(SPI_t *, const uint8_t);

#endif /* SPIX_H */
