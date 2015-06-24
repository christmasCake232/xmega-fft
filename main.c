

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "system.h"
#include "stdioWrapper.h"
#include "usart_spi.h"
#include "adc.h"
#include "dma.h"
#include "lcd.h"
#include "dac.h"


#define BUFFER_SIZE (1024)








//void ADC_Init(void);
void dma_test(void);



ISR(ADCA_CH0_vect)
{
    
    // If the DMA transaction is incomplete, then start the next conversion.
    if(bit_is_clear(DMA.INTFLAGS, DMA_CH0TRNIF_bp))
        adcx_start();

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
    //dac_init();
    dma_init((uint8_t *)dma_data, BUFFER_SIZE *2);
    lcd_init();
    
    PMIC.CTRL = PMIC_HILVLEN_bm;
    sei();
    
    // Test start.
    printf("Start\n");
    
    
    
    for(;;)
    {

        adcx_start();
        dma_block();
        
        
        lcd_barGraph(dma_data);
        
    }
    

    
} // End of main().


