// 
//  Project: Experiment 2.10.8 Use DMA - Chapter 2 
//  File name: dmaReset.c   
//  Function(s): dmaReset()    
//
//  Description: This function resets the C55x DMA channle (dmaNum).
//
//  For the book "Real Time Digital Signal Processing: 
//                Implementation and Application, 2nd Ed"
//                By Sen M. Kuo, Bob H. Lee, and Wenshun Tian
//                Publisher: John Wiley and Sons, Ltd
//
//  Tools used: CCS v.2.12.07
//              TMS320VC5510 DSK Rev-C
//

#include "dma.h"

#pragma CODE_SECTION(dmaReset, ".text:example:dmaReset");

void dmaReset(short dmaNum)
{
  ioport volatile unsigned short *dmaRegPtr;
    
  dmaNum<<=5; // Select the correct DMA channel(0x20 apart for each channel)
  dmaRegPtr = (ioport volatile unsigned short*)(DMACH0_BASE+ dmaNum);

  *dmaRegPtr++ = DMACSDP_DEFAULT_VAL;
  *dmaRegPtr++ = DMACCR_DEFAULT_VAL;
  *dmaRegPtr++ = DMACICR_DEFAULT_VAL;
  *dmaRegPtr++ = DMACSR_DEFAULT_VAL; 
  *dmaRegPtr++ = 0;
  *dmaRegPtr++ = 0;
  *dmaRegPtr++ = 0;
  *dmaRegPtr++ = 0; 
}

