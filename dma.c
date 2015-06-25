


#include <avr/io.h>

#include "dma.h"


static inline void setAddr(volatile void *, volatile register8_t *, volatile register8_t *, volatile register8_t *);



void dma_init(uint8_t *arr, const uint16_t arrSize)
{
    DMA.CTRL = DMA_ENABLE_bm;
    
    // Single mode. (Stop at every burst)
    DMA.CH0.CTRLA = DMA_CH_SINGLE_bm | DMA_CH_BURSTLEN_2BYTE_gc;
    
    DMA.CH0.CTRLB = DMA_CH_TRNINTLVL_HI_gc;
    
    DMA.CH0.ADDRCTRL = DMA_CH_SRCRELOAD_BURST_gc 
                     | DMA_CH_SRCDIR_INC_gc 
                     | DMA_CH_DESTRELOAD_TRANSACTION_gc 
                     | DMA_CH_DESTDIR_INC_gc;
    
    // Trigger off ACDA CH0.
    DMA.CH0.TRIGSRC = DMA_CH_TRIGSRC_ADCA_CH0_gc;

    // The data size is 16bits.
    // Transaction count is 2X the size of the array.
    DMA.CH0.TRFCNT = arrSize;
    
    // Data from tht ADCA CH0.
    setAddr(&ADCA.CH0.RES, &(DMA.CH0.SRCADDR0), &(DMA.CH0.SRCADDR1), &(DMA.CH0.SRCADDR2));
    
    // Data to the array.
    setAddr(arr, &(DMA.CH0.DESTADDR0), &(DMA.CH0.DESTADDR1), &(DMA.CH0.DESTADDR2));
    
    DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;
    
}

uint8_t dma_isDone(void)
{
    return bit_is_set(DMA.INTFLAGS, DMA_CH0TRNIF_bp);
}

void dma_clearFlag(void)
{
    DMA.INTFLAGS |= DMA_CH0TRNIF_bm;
}

void dma_block(void)
{
    loop_until_bit_is_set(DMA.INTFLAGS, DMA_CH0TRNIF_bp);
    DMA.INTFLAGS |= DMA_CH0TRNIF_bm;
    
    DMA.CH0.CTRLA |= DMA_CH_ENABLE_bm;
}


/* <----- static inline fun() -----> */

static inline void setAddr(volatile void * addrS, 
                           volatile register8_t * addr0, 
                           volatile register8_t * addr1, 
                           volatile register8_t * addr2)
{
    *addr0 = (((uint32_t)addrS) >> 0*8) & 0xFF;
    *addr1 = (((uint32_t)addrS) >> 1*8) & 0xFF;
    *addr2 = (((uint32_t)addrS) >> 2*8) & 0xFF;
    
} // End of setAddr().

