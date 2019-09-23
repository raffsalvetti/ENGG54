// 
//  Project: Experiment 2.10.8 Use DMA - Chapter 2 
//  File name: dmaTest.c   
//  Function(s): main()    
//
//  Description: Experiment 2.10.8 demonstrates the C55x DMA data transfer.
//
//  For the book "Real Time Digital Signal Processing: 
//                Implementation and Application, 2nd Ed"
//                By Sen M. Kuo, Bob H. Lee, and Wenshun Tian
//                Publisher: John Wiley and Sons, Ltd
//
//  Tools used: CCS v.2.12.07
//              TMS320VC5510 DSK Rev-C
//

#include <stdio.h>
#include "dma.h"
#include "emif.h"

#define N              128  // Transfer data elements 
#define M              16   // Transfer data frames 
#define DMA_CHANNEL    3    // DMA channel


// SRC is in DARAM and DST is in SDRAM 
// Force SRC and DST to align at 32-bit boundary
#pragma DATA_SECTION(src, ".daram:example:dmaDemo")
#pragma DATA_SECTION(dst, ".sdram:example:dmaDemo")
#pragma DATA_ALIGN(src, 2);
#pragma DATA_ALIGN(dst, 2);
unsigned short src[N*M];
unsigned short dst[N*M];

#pragma CODE_SECTION(main, ".text:example:main");

void main(void)
{
  printf("DMA TESTEEEEEEEEEE\n");
  unsigned short i,frame,err;
  short dmaInitParm[DMA_REGS];
  unsigned long srcAddr,dstAddr;
    
  // Initialize EMIF
  emifInit();
        
  // Initialize source and destination memory for testing
  for (i = 0; i < (N*M); i++) 
  {
    dst[i] = 0;
    src[i] = i + 1;
  }
    
  // Convert word address to byte address, DMA uses byte address
  srcAddr = (unsigned long)src; 
  dstAddr = (unsigned long)dst; 
  srcAddr <<= 1;
  dstAddr <<= 1;
    
  // Setup DMA initialization values
  dmaInitParm[0]  = DMACSDP_INIT_VAL;
  dmaInitParm[1]  = DMACCR_INIT_VAL;
  dmaInitParm[2]  = DMACICR_INIT_VAL;
  dmaInitParm[3]  = DMACSR_INIT_VAL;    
  dmaInitParm[4]  = (short)(srcAddr & 0xFFFF);
  dmaInitParm[5]  = (short)(srcAddr >> 16);
  dmaInitParm[6]  = (short)(dstAddr & 0xFFFF);
  dmaInitParm[7]  = (short)(dstAddr >> 16);
  dmaInitParm[8]  = N; 
  dmaInitParm[9]  = M;   
  dmaInitParm[10] = DMACSFI_INIT_VAL;       
  dmaInitParm[11] = DMACSEI_INIT_VAL;
  dmaInitParm[12] = 0;
  dmaInitParm[13] = 0;
  dmaInitParm[14] = DMACDEI_INIT_VAL;
  dmaInitParm[15] = DMACDFI_INIT_VAL;
    
  // Initialize DMA channel           
  dmaInit(DMA_CHANNEL, dmaInitParm);

  // Enable DMA channel and begin data transfer
  dmaEnable(DMA_CHANNEL);

  // Do DMA data transfer at background
  frame = M;
  while (frame>0) 
  {
    if (dmaFrameStat(DMA_CHANNEL) != 0)
    {
      frame--;
    }   
  }

  // Close DMA channel
  dmaReset(DMA_CHANNEL);   
   
  // Check data transfer is correct or not
  err = 0;
  for (i = 0; i <(N*M); i++) 
  {
    if (dst[i] != src[i]) 
    {
      err++;
    }
  }
            
  printf("DMA Demo: error found = %d\n", err);
}
