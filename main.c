

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <math.h>

#include "system.h"
#include "stdioWrapper.h"
#include "usart_spi.h"
#include "adc.h"
#include "dma.h"
#include "lcd.h"
#include "dac.h"

#define BUFFER_SIZE (1024)



ISR(DMA_CH0_vect)
{
    timer_stop();
}



uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



volatile uint16_t dmaData[BUFFER_SIZE];
volatile uint8_t lcdData[128];
volatile double lcdDatad[128];


void main(void)
{

    // System setup.
    system_clocks_init();
    system_gpio_init();
    stdioWrapper_init(&USARTC0);
    
    // Peripheral setup.
    adc_init(20);
    //dac_init();
    dma_init((uint8_t *)dmaData, BUFFER_SIZE *2);
    lcd_init();
    
    PMIC.CTRL = PMIC_HILVLEN_bm;
    sei();
    
    // Test start.
    printf("Start\n");
    
    
    
    for(;;)
    {
        
        //adc_start();
        timer_start();
        dma_block();
        LED_0_TGL();
        
        

        
        lcd_barGraph((uint8_t *)lcdData);
        
    } // End of for ever loop.
        
} // End of main().












 





