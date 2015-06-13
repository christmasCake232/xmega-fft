#include "CircularBuffer.h"

void setup_cb(CircularBuffer *cb)
{
	cb->start = 0;
	cb->count = 0;
}
 
uint8_t cb_isFull(CircularBuffer *cb)
{
    return cb->count == CIRCULARBUFFER_SIZE; 
}
 
uint8_t cb_isEmpty(CircularBuffer *cb)
{
	return cb->count == 0;
}
 
void cb_write(CircularBuffer *cb, uint8_t elem)
{
	uint8_t end = (cb->start + cb->count) % CIRCULARBUFFER_SIZE;
	cb->elems[end] = elem;
	if (cb->count == CIRCULARBUFFER_SIZE)
		cb->start = (cb->start + 1) % CIRCULARBUFFER_SIZE; /* full, overwrite */
	else
		++(cb->count);
}
 
uint8_t cb_read(CircularBuffer *cb)
{
	uint8_t elem; 
	
	elem = cb->elems[cb->start];
	cb->start = (cb->start + 1) % CIRCULARBUFFER_SIZE;
	--(cb->count);
	
	return elem;
}
