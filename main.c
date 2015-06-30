

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



ISR(DMA_CH0_vect)
{
    adc_stop();
}



uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

ADC_PRESCALER_t preTable[] = {ADC_PRESCALER_DIV4_gc,   ADC_PRESCALER_DIV8_gc, 
                                     ADC_PRESCALER_DIV16_gc,  ADC_PRESCALER_DIV32_gc, 
                                     ADC_PRESCALER_DIV64_gc,  ADC_PRESCALER_DIV128_gc, 
                                     ADC_PRESCALER_DIV256_gc, ADC_PRESCALER_DIV512_gc};
            

volatile uint16_t dmaData[BUFFER_SIZE];
volatile uint8_t lcdData[128];


void main(void)
{

    // System setup.
    system_clocks_init();
    system_gpio_init();
    stdioWrapper_init(&USARTC0);
    
    // Peripheral setup.
    adc_init(20, preTable[2]);
    //dac_init();
    dma_init((uint8_t *)dmaData, BUFFER_SIZE *2);
    lcd_init();
    
    PMIC.CTRL = PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm;
    sei();
    
    // Test start.
    printf("Start\n");
    
    uint16_t min;
    uint16_t max;
    uint16_t index;
    uint8_t ex = 9;
    uint8_t pre = 2;
    
    for(;;)
    {
        // SW0 used for cycling through the sample rate.
        if(SW0_IS_SET())
        {
            ++ex;
            if(ex > 20)
                ex = 9;
                
            adc_init(ex, preTable[pre]);
            
            while(SW0_IS_SET());
        }
        
        if(SW1_IS_SET())
        {
            
            ++pre;
            pre %= 8;
            
            adc_init(ex, preTable[pre]);
            
            while(SW1_IS_SET());
        }
        
        
        adc_start();
        dma_block();
        LED_0_TGL();
        
        for(index = 0, min = 0xFFFF, max = 0; index < 128; ++index)
        {
            if(dmaData[index] < min)
                min = dmaData[index];
            
            if(dmaData[index] > max)
                max = dmaData[index];
        }
        
        for(index = 0; index < 128; ++index)
        {
            lcdData[index] = map(dmaData[index], min, max, 0, 33); 
        }
        
        
        lcd_barGraph((uint8_t *)lcdData);
        
    } // End of for ever loop.
    
} // End of main().
