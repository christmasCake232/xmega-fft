

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "system.h"
#include "stdioWrapper.h"
#include "spi.h"
#include "usart_spi.h"
#include "adc.h"
#include "dma.h"


#define BUFFER_SIZE 1024


//void ADC_Init(void);
void dma_test(void);



ISR(ADCA_CH0_vect)
{
    // If the DMA transaction is incomplete, then start the next conversion.
    if(bit_is_clear(DMA.INTFLAGS, DMA_CH0TRNIF_bp))
        ADCA.CH0.CTRL |= ADC_CH_START_bm; // Start ADC'ing for CH0.

}




volatile uint16_t dma_data[BUFFER_SIZE];


void main(void)
{
    uint16_t index = 0;

    // System setup.    
    stdioWrapper_init(&USARTC0);
    system_gpio_init();
    
    adcx_init();
    spix_init(&SPIC);
    dma_init((uint8_t *)dma_data, BUFFER_SIZE *2);
    
    PMIC.CTRL = PMIC_HILVLEN_bm;
    sei();
    
    // Test start.
    printf("Start\n");
    PORTD.OUTCLR = (uint8_t)(_BV(4));
    
    
    for(;;)
    {
        
        adcx_start();
        
        loop_until_bit_is_set(DMA.INTFLAGS, DMA_CH0TRNIF_bp);
        DMA.INTFLAGS |= DMA_CH0TRNIF_bm;
        
        DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;
        
        
        PORTR.OUTTGL = (uint8_t)(_BV(0));        
    }
    
} // End of main().


