#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <avr/io.h>

#define CIRCULARBUFFER_SIZE 16


typedef struct {
	uint8_t start; 
	uint8_t count; 
	uint8_t elems[CIRCULARBUFFER_SIZE];
	
} CircularBuffer;


void setup_cb(CircularBuffer *cb);

uint8_t cb_isFull(CircularBuffer *cb);
uint8_t cb_isEmpty(CircularBuffer *cb);

void cb_write(CircularBuffer *cb, uint8_t elem);
uint8_t cb_read(CircularBuffer *cb);


#endif /* CIRCULARBUFFER_H */
