

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>


#include "stdioWrapper.h"



void ADC_Init(void);

void dma_test(void);
void dma_test_adc(void);



volatile uint16_t adcLIGHT = 0;




ISR(TCE0_OVF_vect)
{
    //PORTR.OUTTGL = _BV(1) | _BV(0);
    //ADCA_CH0_CTRL |= ADC_CH_START_bm;    //Start ADC'ing for CH0
}


ISR(ADCA_CH0_vect)
{
    adcLIGHT = ADCA_CH0_RES;    //CH0 ADC result here
    
    //printf("%04X\n", adcLIGHT);
    
    
    //ADCA_CH0_CTRL |= ADC_CH_START_bm;    //Start ADC'ing for CH0
    //PORTR.OUTTGL = _BV(1) | _BV(0);
}



volatile uint16_t adc_data[100];


int main(void)
{
    uint8_t data = 0;

    // Set the LED to output.
    PORTR.DIRSET = _BV(1) | _BV(0);
    PORTR.OUTSET = _BV(1);
    
    

    stdioWrapper_init(&USARTC0);
    ADC_Init();
    
    
    /*
    // Setup timer.
    TCE0.PER = 0xFF;
    
    TCE0.INTCTRLA = TC_OVFINTLVL_HI_gc;
    */
    
    PMIC.CTRL = PMIC_HILVLEN_bm;
    sei();
    
    /*
    // Start the Timer at F_CPU/1024.
    TCE0.CTRLA = TC_CLKSEL_DIV1024_gc;
    */
    
    ADCA_CH0_CTRL |= ADC_CH_START_bm;    
    

    
    for(;;)
    {
        
        // echo back for testing stdio.
        data = getchar();
        printf("%c", data);
        
        PORTR.OUTTGL = _BV(1) | _BV(0);
    
        /*
        if(TCE0.INTFLAGS & TC0_OVFIF_bm) {
            
            TCE0.INTFLAGS = _BV(TC0_OVFIF_bp);
            
            PORTR.OUTTGL = _BV(1) | _BV(0);
        }
        //*/
    }

    return 0;
}


static inline void setAddr(volatile void * addrS, 
                           volatile register8_t * addr0, 
                           volatile register8_t * addr1, 
                           volatile register8_t * addr2)
{
    *addr0 = (((uint32_t)addrS) >> 0*8) & 0xFF;
    *addr0 = (((uint32_t)addrS) >> 1*8) & 0xFF;
    *addr0 = (((uint32_t)addrS) >> 2*8) & 0xFF;
    
} // End of setAddr().



void dma_test(void)
{
    DMA.CTRL = DMA_ENABLE_bm;
    
    DMA.CH0.CTRLA = DMA_CH_BURSTLEN_8BYTE_gc;
    
    DMA.CH0.ADDRCTRL = DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTDIR_INC_gc;
    
    // Software triggers only.
    DMA.CH0.TRIGSRC = 0;
    
    DMA.CH0.TRFCNT = 0x0400; // count to 1024.
    
    /*
    setAddr(arrS, &(DMA.CH0.SRCADDR0), &(DMA.CH0.SRCADDR1), &(DMA.CH0.SRCADDR2));
    setAddr(arrD, &(DMA.CH0.DESTADDR0), &(DMA.CH0.DESTADDR1), &(DMA.CH0.DESTADDR2));
    //*/
    
    /*
    DMA.CH0.SRCADDR0 = 0xFF & (uint32_t)arrS;
    DMA.CH0.SRCADDR1 = 0xFF & ((uint32_t)arrS >> 8);
    DMA.CH0.SRCADDR2 = 0xFF & ((uint32_t)arrS >> 16);
    */
    
    /*
    DMA.CH0.DESTADDR0 = 0xFF & (uint32_t)arrD;
    DMA.CH0.DESTADDR1 = 0xFF & ((uint32_t)arrD >> 8);
    DMA.CH0.DESTADDR2 = 0xFF & ((uint32_t)arrD >> 16);
    */
    
    DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;
    DMA.CH0.CTRLA |= DMA_CH_TRFREQ_bm;
    
    loop_until_bit_is_set(DMA.INTFLAGS, DMA_CH0TRNIF_bp);
    DMA.INTFLAGS |= DMA_CH0TRNIF_bm;
    
    
    

}


void dma_test_adc(void)
{
    /*
    DMA.CTRL = DMA_ENABLE_bm;
    DMA.CH1.CTRLA = DMA_CH_SINGLE_bm | DMA_CH_BURSTLEN_2BYTE_gc;
    //DMA.CH1.ADDCTRL =
    
    DMA.CTRL = DMA_ENABLE_bm;
    
    DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_TRANSACTION_gc | DMA_CH_DESTRELOAD_BLOCK_gc | DMA_CH_DESTDIR_INC_gc;
    
    DMA.CH0.TRIGSRC = DMA_CH_TRIGSRC_ADCA_CH0_gc;
    DMA.CH0.TRFCNT = 100;
    
    setAddr(&ADCA_CH0_RES, &(DMA.CH0.SRCADDR0), &(DMA.CH0.SRCADDR1), &(DMA.CH0.SRCADDR2));
    setAddr(adc_data, &(DMA.CH0.DESTADDR0), &(DMA.CH0.DESTADDR1), &(DMA.CH0.DESTADDR2));
    
    DMA.CH0.CTRLA = DMA_CH_ENABLE_bm | DMA_CH_REPEAT_bm | DMA_CH_SINGLE_bm | DMA_CH_BURSTLEN_2BYTE_gc;
    
    loop_until_bit_is_set(DMA.INTFLAGS, DMA_CH0TRNIF_bp);
    DMA.INTFLAGS |= DMA_CH0TRNIF_bm;
    */
    
    /*
    // configure DMA
    DMA.CH0.ADDRCTRL = 0xC5;// Reload, fixed source
    DMA.CH0.TRIGSRC= 0x10;  // ADCA CH0 is trigger source
    DMA.CH0.TRFCNT = 100;   // Buffer is len bytes
    DMA.CH0.DESTADDR0  =(((uint32_t)gADCBuf)>>0*8) & 0xFF;
    DMA.CH0.DESTADDR1  =(((uint32_t)gADCBuf)>>1*8) & 0xFF;
    DMA.CH0.DESTADDR2  =(((uint32_t)gADCBuf)>>2*8) & 0xFF;
    DMA.CH0.SRCADDR0 =(((uint32_t)(&ADCA.CH0.RES))>>0*8)&0xFF;
    DMA.CH0.SRCADDR1 =(((uint32_t)(&ADCA.CH0.RES))>>1*8)&0xFF;
    DMA.CH0.SRCADDR2 =(((uint32_t)(&ADCA.CH0.RES))>>2*8)&0xFF;
    DMA.CH0.CTRLA = 0xA5;   // Enable, repeat, 2 byte, burst  
    */
    
}




void ADC_Init(void)        //NOT A COMPLETE INITIALIZER ... just cleaning the main()
{
    ADCA_CH0_CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc | ADC_CH_GAIN_1X_gc;
    
    ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;   //
    ADCA_CTRLA = ADC_ENABLE_bm;
    ADCA_CTRLB = ADC_RESOLUTION_12BIT_gc;
    ADCA_PRESCALER = ADC_PRESCALER_DIV512_gc;
    ADCA_REFCTRL = ADC_REFSEL_INTVCC_gc ;//ADC_REFSEL_VCC_gc;           // ADC_REFSEL_AREFA_gc;
    ADCA_CH0_INTCTRL = ADC_CH_INTLVL_HI_gc;
    
}
