// 
//  Project: Experiment 2.10.8 Use DMA - Chapter 2 
//  File name: dmaFrameStat.c   
//  Function(s): dmaFrameStat()    
//
//  Description: This function checks the C55x DMA channle frame status bit.
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

#pragma CODE_SECTION(dmaFrameStat, ".text:example:dmaFrameStat");

short dmaFrameStat(short dmaNum)
{
  ioport volatile unsigned short *dmaRegPtr;

  dmaNum <<= 5;                  // Make DMA channel offset
  dmaRegPtr = DMA_CSR + dmaNum;  // Pointer to DMA_CSR
    
  return (*dmaRegPtr&0x8);       // Return frame status bit
}


