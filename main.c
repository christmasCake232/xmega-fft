

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "system.h"
#include "stdioWrapper.h"
#include "usart_spi.h"
#include "adc.h"
#include "dma.h"
#include "lcd.h"


#define BUFFER_SIZE (1024 << 1)


//void ADC_Init(void);
void dma_test(void);



ISR(ADCA_CH0_vect)
{
    PORTR.OUTTGL = (uint8_t)(_BV(1));
    
    // If the DMA transaction is incomplete, then start the next conversion.
    if(bit_is_clear(DMA.INTFLAGS, DMA_CH0TRNIF_bp))
        ADCA.CH0.CTRL |= ADC_CH_START_bm; // Start ADC'ing for CH0.

}


volatile uint16_t dma_data[BUFFER_SIZE];


void main(void)
{

    // System setup.
    system_clocks_init();
    system_gpio_init();
    stdioWrapper_init(&USARTC0);
    
    // Peripheral setup.
    adcx_init();
    dma_init((uint8_t *)dma_data, BUFFER_SIZE *2);
    
    PMIC.CTRL = PMIC_HILVLEN_bm;
    sei();
    
    // Test start.
    printf("Start\n");
    
    lcd_init();
    
    
    for(;;)
    {
        adcx_start();
        
        loop_until_bit_is_set(DMA.INTFLAGS, DMA_CH0TRNIF_bp);
        DMA.INTFLAGS |= DMA_CH0TRNIF_bm;
        
        DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;
        
        PORTR.OUTTGL = (uint8_t)(_BV(0));
        
    } // End of forever loop.
    
} // End of main().


