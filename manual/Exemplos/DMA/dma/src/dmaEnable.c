// 
//  Project: Experiment 2.10.8 Use DMA - Chapter 2 
//  File name: dmaEnable.c   
//  Function(s): dmaEnable()    
//
//  Description: This function enables the C55x DMA channle according to dmaNum.
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

#pragma CODE_SECTION(dmaEnable, ".text:example:dmaEnable");

void dmaEnable(short dmaNum)
{
  ioport volatile unsigned short *dmaRegPtr;

  dmaNum <<= 5;                  // Make DMA channel offset
  dmaRegPtr = DMA_CCR + dmaNum;  // Pointer to DMA_CCR
  *dmaRegPtr |= 0x80;            // Enable DMA channel
}


