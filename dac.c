

#include <avr/io.h>

#include "dac.h"



void dac_init(void)
{
    DACB.CTRLA = DAC_IDOEN_bm | DAC_ENABLE_bm;
    
    DACB.CTRLB = DAC_CHSEL_SINGLE_gc | DAC_CH0TRIG_bm;
    
    DACB.CTRLC = DAC_REFSEL_INT1V_gc;
}

void dac_write(uint16_t data)
{
    loop_until_bit_is_set(DACB.STATUS, DAC_CH0DRE_bp);
    DACB.CH0DATA = data;
}
    
