

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


#define BUFFER_SIZE (128)


void timer_init(void)
{
    // Stop the timer.
    TCC0_CTRLA = 0;
    
    TCC0_CTRLB = TC_WGMODE_NORMAL_gc;
    
    TCC0_CTRLC = 0;
    TCC0_CTRLD = 0;
    
    //
    TCC0_CTRLE = TC_BYTEM_NORMAL_gc;
    
    
    TCC0_INTCTRLA = TC_OVFINTLVL_HI_gc;
    
    TCC0_INTFLAGS = TC0_OVFIF_bm;
    
    
    TCC0_CNT = 0;
    
    
    TCC0_PER = 0x4FF;
    
    
    //TCC0_PERBUF = 0xFFFF;
    
    
    
    
}


ISR(DMA_CH0_vect)
{
    TCC0_CTRLA = 0;
}


ISR(ADCA_CH0_vect)
{
    /*
    
    // If the DMA transaction is incomplete, then start the next conversion.
    if(bit_is_clear(DMA.INTFLAGS, DMA_CH0TRNIF_bp))
        adcx_start();
    */

}

uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



volatile uint16_t dmaData[BUFFER_SIZE];
volatile uint8_t lcdData[128];


void main(void)
{

    // System setup.
    system_clocks_init();
    system_gpio_init();
    stdioWrapper_init(&USARTC0);
    
    // Peripheral setup.
    adcx_init();
    //dac_init();
    dma_init((uint8_t *)dmaData, BUFFER_SIZE *2);
    lcd_init();
    
    PMIC.CTRL = PMIC_HILVLEN_bm;
    sei();
    
    // Test start.
    printf("Start\n");
    
    timer_init();
    
    uint8_t index;
    uint16_t min = 0;
    uint16_t max = 0x0FFF;
        
    for(;;)
    {
        if(SW1_IS_SET())
        {
            min += 8;
            while(SW1_IS_SET());
        }
        
        if(SW2_IS_SET())
        {
            max -= 64;
            while(SW2_IS_SET());
        }
        
        TCC0_CTRLA = TC_CLKSEL_DIV1024_gc;
        
        //adcx_start();
        dma_block();
        
        for(index = 0; index < 128; ++index)
            lcdData[index] = map(dmaData[index], min, max, 0, 32);
        
        
        lcd_barGraph((uint8_t *)lcdData);
    }
    

    
} // End of main().




ISR(TCC0_OVF_vect)
{
    //if(bit_is_clear(DMA.INTFLAGS, DMA_CH0TRNIF_bp))
    adcx_start();
    
    

}








 





