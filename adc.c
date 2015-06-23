


#include <avr/io.h>
#include <avr/interrupt.h>

#include "adc.h"

void adcx_init(void)
{
    
    
    //ADCB.CH0.CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCB.CH0.CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_INTERNAL_gc;
    
    //ADCB.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
    ADCB.CH0.MUXCTRL = (0x03<<3);
    
    ADCB.CH0.INTCTRL = ADC_CH_INTLVL_HI_gc;
    
    ADCB.CTRLA = ADC_ENABLE_bm;
    ADCB.CTRLB = ADC_RESOLUTION_12BIT_gc;
    ADCB.PRESCALER = ADC_PRESCALER_DIV512_gc;
    //ADCB.REFCTRL = ADC_REFSEL_INTVCC_gc;
    ADCB.REFCTRL = ADC_REFSEL_INT1V_gc;
    

    
} // End of adcx_init().


void adcx_start(void)
{
    
    ADCB.CH0.CTRL |= ADC_CH_START_bm;
    
} // End of adcx_start().

