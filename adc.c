
/*
 * This ADC is using event CH0 and TCC0.
 */


#include <avr/io.h>

#include "adc.h"
#include "timeTable.h"


static inline void timer_init(const uint8_t);
static inline void timer_setOverFlow(const uint8_t);

static inline void eventCh0_init(void);

void adc_init(const uint8_t ex, const ADC_PRESCALER_t prescl)
{
    // Setup the timer.
    timer_init(ex);
    
    // Setup event CH0.
    eventCh0_init();
    
    ADCA.CH0.CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_SINGLEENDED_gc;
    //ADCB.CH0.CTRL = ADC_CH_GAIN_1X_gc | ADC_CH_INPUTMODE_INTERNAL_gc;
    
    ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
    //ADCB.CH0.MUXCTRL = (0x03<<3);
    
    ADCA.CTRLA = ADC_ENABLE_bm;
    ADCA.CTRLB = ADC_RESOLUTION_12BIT_gc;
    ADCA.PRESCALER = prescl;
    
    ADCA.REFCTRL = ADC_REFSEL_INTVCC_gc;
    //ADCB.REFCTRL = ADC_REFSEL_INT1V_gc;
    
    ADCA.EVCTRL = ADC_SWEEP_0_gc | ADC_EVSEL_0123_gc | ADC_EVACT_CH0_gc;

    
} // End of adc_init().


void adc_start(void)
{
    // Start the timer.
    TCC0.CTRLA = TC_CLKSEL_DIV1_gc;
    
    // The ADC is running off the timer.
    //ADCA.CH0.CTRL |= ADC_CH_START_bm;
    
} // End of adc_start().

void adc_stop(void)
{
    // Stop the timer.
    TCC0.CTRLA = 0;
    
} // End of adc_stop().


/* <----- static inline fun() -----> */




static inline void timer_init(const uint8_t ex)
{
    // Stop the timer.
    TCC0.CTRLA = 0;
    
    // Set timer in normal mode.
    TCC0.CTRLB = TC_WGMODE_NORMAL_gc;
    
    TCC0.CTRLC = 0;
    TCC0.CTRLD = 0;
    
    // Set timer in normal mode. 
    TCC0.CTRLE = TC_BYTEM_NORMAL_gc;
    
    // Reset the timer count.
    TCC0.CNT = 0;
    
    // Set the timer max/top. This is the overflow value.
    timer_setOverFlow(ex);
    
    //TCC0.PERBUF = 0xFFFF;
    
} // End of timer_init().



static inline void timer_setOverFlow(const uint8_t ex)
{
    switch(ex)
    {
        // Base 10.
        case 3:
            TCC0.PER = PER_10_03;
            return;
        case 4:
            TCC0.PER = PER_10_04;
            return;
        case 5:
            TCC0.PER = PER_10_05;
            return;
        case 6:
            TCC0.PER = PER_10_06; // (1M)
            return;
        case 7:
            TCC0.PER = PER_10_07;
            return;
            
        // Base 2
        case 9:
            TCC0.PER = PER_02_09;
            return;
        case 10:
            TCC0.PER = PER_02_10;
            return;
        case 11:
            TCC0.PER = PER_02_11;
            return;
        case 12:
            TCC0.PER = PER_02_12;
            return;
        case 13:
            TCC0.PER = PER_02_13;
            return;
        case 14:
            TCC0.PER = PER_02_14;
            return;
        case 15:
            TCC0.PER = PER_02_15;
            return;
        case 16:
            TCC0.PER = PER_02_16;
            return;
        case 17:
            TCC0.PER = PER_02_17;
            return;
        case 18:
            TCC0.PER = PER_02_18;
            return;
        case 19:
            TCC0.PER = PER_02_19;
            return;
        case 20:
            TCC0.PER = PER_02_20; // (1M)
            return;
        case 21:
            TCC0.PER = PER_02_21;
            return;
        case 22:
            TCC0.PER = PER_02_22;
            return;
        case 23:
            TCC0.PER = PER_02_23;
            return;
        case 24:
            TCC0.PER = PER_02_24;
            return;
        case 25:
            TCC0.PER = PER_02_25;
            return;
        default:
            TCC0.PER = 0xFFFF;
            return;
            
    } // End of switch.
    
} // End of timer_setOverFlow().


static inline void eventCh0_init(void)
{
    EVSYS_CH0MUX = EVSYS_CHMUX_TCC0_OVF_gc;
    
} // End of eventCh0_init().



