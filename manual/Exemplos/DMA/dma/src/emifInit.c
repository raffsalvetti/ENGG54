// 
//  Project: Experiment 2.10.8 Use DMA - Chapter 2 
//  File name: emifInit.c   
//  Function(s): emifInit()        
//
//  Description: This function initialize the C5510 DSK EMIF registers
//
//  For the book "Real Time Digital Signal Processing: 
//                Implementation and Application, 2nd Ed"
//                By Sen M. Kuo, Bob H. Lee, and Wenshun Tian
//                Publisher: John Wiley and Sons, Ltd
//
//  Tools used: CCS v.2.12.07
//              TMS320VC5510 DSK Rev-C
//

#include "emif.h"

#pragma CODE_SECTION(emifInit,    ".text:example:emifInit");

void emifInit(void)
{
  *EGCR    = 0x0200   // MEMFREQ: 1/2 of the CPU clock 
           | 0x0000   // WPE: disabled
           | 0x0020   // MEMCEN: enabled
           | 0x0001;  // NOHOLD: hold is disabled
  *EMI_RST = 0x0001;  // EMIF RESET: reset
  *EMI_BE  = 0x0000;  // Clean all error status bits

  // SET CE0 for SDRAM    
  *CE0_1   = 0x3000   // MTYPE: 32-bit synchronous DRAM (SDRAM)
           | 0x0400   // READ SETUP: 4 cycles
           | 0x0020   // READ STRBE: 8 cycles
           | 0x0002;  // READ HOLD: 2 cycles
  *CE0_2   = 0x4000   // READ EXT HOLD 1 cycle
           | 0x1000   // WRITE EXT HOLD 1 cycle
           | 0x0400   // WRITE SETUP: 4 cycles
           | 0x0020   // WRITE STRBE: 8 cycles
           | 0x0002;  // WRITE HOLD: 2 cycles
  *CE0_3   = 0x0000;  // TIMEOUT: ignored for SDRAM

  // SET CE1 at default value for FLASH        
  *CE1_1   = 0x1038;
  *CE1_2   = 0x0038;
  *CE1_3   = 0x0038;    

  // Configure SDRAM    
  *SDC1    = 0x2800   // TRC: 5 CLKMEM cycles
           | 0x0200   // SDSIZE:SDWID: 2Mx32bits (8 MBytes)
           | 0x0100   // RFEN: refresh is enabled
           | 0x0010   // TRCD: 1 CLKMEM cycle
           | 0x0001;  // TRP: 1 CLKMEM cycle
  *SDC2    = 0x0400   // SDCC: select 32-bit SDRAM interface
           | 0x0100   // TMRD: 1 CLKMEM cycle
           | 0x0030   // TRAS: 3 CLKMEM cycles
           | 0x0005;  // TACTV2ACTV: 5 CLKMEM cycles
  *SDPER   = 0x0080;  // REFRESH PERIOD: 128 CLKMEM cycles
  *SDCNT   = 0x0080;  // REFRESH PERIOD COUNTER: 128 CLKMEM cycles
  *INIT    = 0x0001;  // Enable the SDRAM
}
