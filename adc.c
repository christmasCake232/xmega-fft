


#include <avr/io.h>
#include <avr/interrupt.h>

#include "system.h"
#include "adc.h"


static inline void timer_init(void);

void adc_init(void)
{
    
    ADCA.CH0.CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc;
    //ADCB.CH0.CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_INTERNAL_gc;
    
    ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
    //ADCB.CH0.MUXCTRL = (0x03<<3);
    
    //ADCA.CH0.INTCTRL = ADC_CH_INTLVL_HI_gc;
    
    ADCA.CTRLA = ADC_ENABLE_bm;
    ADCA.CTRLB = ADC_RESOLUTION_12BIT_gc;
    ADCA.PRESCALER = ADC_PRESCALER_DIV512_gc;
    
    ADCA.REFCTRL = ADC_REFSEL_INTVCC_gc;
    //ADCB.REFCTRL = ADC_REFSEL_INT1V_gc;
    
    timer_init();
    

    
} // End of adc_init().


void adc_start(void)
{    
    ADCA.CH0.CTRL |= ADC_CH_START_bm;
    
} // End of adc_start().


void timer_start(void)
{
    // Start the timer.
    TCC0.CTRLA = TC_CLKSEL_DIV4_gc;
    
    
} // End of timer_start().

void timer_stop(void)
{
    // Stop the timer.
    TCC0.CTRLA = 0;
    
} // End of timer_stop().


/* <----- static inline fun() -----> */

static inline void timer_init(void)
{
    // Assuming F_CPU is fixed at 32MHz.
    
    
    timer_stop();
    
    // Set timer in normal mode.
    TCC0.CTRLB = TC_WGMODE_NORMAL_gc;
    
    TCC0.CTRLC = 0;
    TCC0.CTRLD = 0;
    
    // Set timer in normal mode. 
    TCC0.CTRLE = TC_BYTEM_NORMAL_gc;
    
    // Enable timer overflow interrupt.
    TCC0.INTCTRLA = TC_OVFINTLVL_HI_gc;
    
    // Clear the timer overflow count.
    TCC0.INTFLAGS = TC0_OVFIF_bm;
    
    // Reset the timer count.
    TCC0.CNT = 0;
    
    // Set the timer max/top. This is the overflow value.
    // Let F_CPU = 32MHz and TOP = 0xFFFF -1536.
    // @DIV8 overflow interval =   62.5Hz
    // @DIV4 overflow interval =  125.0Hz
    // @DIV2 overflow interval =  250.0Hz
    // @DIV1 overflow interval =  500.0Hz
    // ---
    // Let F_CPU = 32MHz and TOP = 31249.
    // @DIV1 overflow interval = 1024.0Hz
    TCC0.PER = 0xFFFF -1536;
    
    
    //TCC0.PERBUF = 0xFFFF;
    
} // End of timer_init().





ISR(TCC0_OVF_vect)
{
    //static uint8_t state = 0;
    
    // Start the ADC on timer overflow.
    ADCA.CH0.CTRL |= ADC_CH_START_bm;
    
}

/*
ISR(ADCA_CH0_vect)
{
    Not used.
}
*/


