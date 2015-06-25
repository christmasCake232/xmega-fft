

#ifndef ADC_H
#define ADC_H

#include <avr/io.h>

void adc_init(void);

void adc_start(void);

void timer_start(void);
void timer_stop(void);
 


#endif /* ADC_H */
