

#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

void adc_init(const uint8_t);

void adc_start(void);

void timer_start(void);
void timer_stop(void);
 


#endif /* ADC_H */
