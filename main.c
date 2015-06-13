

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "stdioWrapper.h"

#define BUFFER_SIZE 50


void ADC_Init(void);
void dma_test(void);



ISR(ADCA_CH0_vect)
{
    // If the DMA transaction is incomplete, then start the next conversion.
    if(bit_is_clear(DMA.INTFLAGS, DMA_CH0TRNIF_bp))
        ADCA_CH0_CTRL |= ADC_CH_START_bm; // Start ADC'ing for CH0.

}




volatile uint16_t dma_data[BUFFER_SIZE];


void main(void)
{
    uint16_t index = 0;

    // System setup.
    // Set the LED to output.
    PORTR.DIRSET = _BV(1) | _BV(0);
    PORTR.OUTSET = _BV(1);
    
    stdioWrapper_init(&USARTC0);
    ADC_Init();
    dma_test();
    
    PMIC.CTRL = PMIC_HILVLEN_bm;
    sei();
    
    // Test start.
    printf("Start\n");
    

    // Start the ADC for the first time.
    ADCA_CH0_CTRL |= ADC_CH_START_bm;
    
    // Block until the DMA transaction is done.
    loop_until_bit_is_set(DMA.INTFLAGS, DMA_CH0TRNIF_bp);
    DMA.INTFLAGS |= DMA_CH0TRNIF_bm;
    
    // Display the data from the ADC.
    for(index = 0; index < BUFFER_SIZE; ++index)
    {
        printf("%03X\n", dma_data[index]);
    }
    

    
    for(;;)
    {
        PORTR.OUTTGL = _BV(1) | _BV(0);
    }
    
} // End of main().


static inline void setAddr(volatile void * addrS, 
                           volatile register8_t * addr0, 
                           volatile register8_t * addr1, 
                           volatile register8_t * addr2)
{
    *addr0 = (((uint32_t)addrS) >> 0*8) & 0xFF;
    *addr1 = (((uint32_t)addrS) >> 1*8) & 0xFF;
    *addr2 = (((uint32_t)addrS) >> 2*8) & 0xFF;
    
} // End of setAddr().



void dma_test(void)
{
    DMA.CTRL = DMA_ENABLE_bm;
    
    // Single mode. (Stop at every burst)
    DMA.CH0.CTRLA = DMA_CH_SINGLE_bm | DMA_CH_BURSTLEN_2BYTE_gc;
    
    DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_BURST_gc | DMA_CH_SRCDIR_INC_gc | DMA_CH_DESTDIR_INC_gc;
    
    // Trigger off ACDA CH0.
    DMA.CH0.TRIGSRC = DMA_CH_TRIGSRC_ADCA_CH0_gc;

    // The data size is 16bits.
    // Transaction count is 2X the size of the array.
    DMA.CH0.TRFCNT = BUFFER_SIZE * 2;
    
    // Data from tht ADCA CH0.
    setAddr(&ADCA_CH0_RES, &(DMA.CH0.SRCADDR0), &(DMA.CH0.SRCADDR1), &(DMA.CH0.SRCADDR2));
    
    // Data to the array.
    setAddr(dma_data, &(DMA.CH0.DESTADDR0), &(DMA.CH0.DESTADDR1), &(DMA.CH0.DESTADDR2));
    
    
    DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;
     
} // End of dma_test().



void ADC_Init(void)        //NOT A COMPLETE INITIALIZER ... just cleaning the main()
{
    ADCA_CH0_CTRL = ADC_CH_INPUTMODE_SINGLEENDED_gc | ADC_CH_GAIN_1X_gc;
    
    ADCA_CH0_MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;   //
    ADCA_CTRLA = ADC_ENABLE_bm;
    ADCA_CTRLB = ADC_RESOLUTION_12BIT_gc;
    ADCA_PRESCALER = ADC_PRESCALER_DIV4_gc;
    ADCA_REFCTRL = ADC_REFSEL_INTVCC_gc ;//ADC_REFSEL_VCC_gc;           // ADC_REFSEL_AREFA_gc;
    ADCA_CH0_INTCTRL = ADC_CH_INTLVL_HI_gc;
    
} // End of ADC_Init().
