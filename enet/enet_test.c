//////////////////////////////////////////////////////////////////////////////
// * File name: enet_test.c
// *                                                                          
// * Description:  ENET testing file.
// *                                                                          
// * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/ 
// * Copyright (C) 2011 Spectrum Digital, Incorporated
// *                                                                          
// *                                                                          
// *  Redistribution and use in source and binary forms, with or without      
// *  modification, are permitted provided that the following conditions      
// *  are met:                                                                
// *                                                                          
// *    Redistributions of source code must retain the above copyright        
// *    notice, this list of conditions and the following disclaimer.         
// *                                                                          
// *    Redistributions in binary form must reproduce the above copyright     
// *    notice, this list of conditions and the following disclaimer in the   
// *    documentation and/or other materials provided with the                
// *    distribution.                                                         
// *                                                                          
// *    Neither the name of Texas Instruments Incorporated nor the names of   
// *    its contributors may be used to endorse or promote products derived   
// *    from this software without specific prior written permission.         
// *                                                                          
// *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS     
// *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT       
// *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR   
// *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT    
// *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,   
// *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT        
// *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   
// *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY   
// *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT     
// *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   
// *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.    
// *                                                                          
//////////////////////////////////////////////////////////////////////////////
 
//#include "stdio.h"
#include "ezdsp5502.h"
#include "ezdsp5502_i2cgpio.h"
#include "enet.h"

#define PACKET_LEN 64
static Uint8 tx[PACKET_LEN];
static Uint8 rx[PACKET_LEN];
static Uint8 testpacket[68] = {
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x00, 0x01,
    0x02, 0x03, 0x04, 0x05,
    0x1c, 0x2d, 0x3e, 0x4f,
    0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b,
    0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13,
    0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b,
    0x1c, 0x1d, 0x1e, 0x1f,
    0x20, 0x21, 0x22, 0x23,
    0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2a, 0x2b,
    0x2c, 0x2d, 0x2e, 0x2f,
    0xc1, 0xc2, 0xc3, 0xc4};

Uint16 regs[256];

/*
 *
 *  enet_test( )
 *
 */
Int16 enet_test( )
{
    Int16 i, j, len;
    Uint8* p8;
    Uint16 data;

    /* Print ENET registers */
    enet_regwrite(0x26, 0x0003);  // Global reset
    EZDSP5502_waitusec(1000);
    enet_regwrite(0x26, 0x0000);  // Deassert global reset
    EZDSP5502_waitusec(1000);
    
    enet_regwrite(0xc6, 0x0235);  // Configure link/activity LEDs

    /* Wait for link */
//    printf("Waiting for link...\n");

    EZDSP5502_I2CGPIO_writeLine(   LED1, LOW );
    while(1)
    {
        if ((enet_regread(0xe6) & 0x0004) != 0)
            break;
    }
    EZDSP5502_I2CGPIO_writeLine(   LED1, HIGH );
//    printf("   --> Link Detected\n");

    /* Transmit packet */
    enet_regwrite(0x70, 0x0000);  // Disable transmit
    enet_regwrite(0x70, 0x0010);  // Flush transmit queue

    enet_regwrite(0x70, 0x0007);  // TXCR - Configure auto-CRC on transmit, transmit padding, transmit enable
    enet_regwrite(0x84, 0x4000);  // TXFDPR - Enable auto increment of data pointer
    enet_regwrite(0x92, 0xffff);  // ISR - Clear all interrupts
    enet_regwrite(0x90, 0xffff);  // IER - Enable transmit/receive interrupts
    enet_regwrite(0x80, 0x0004);  // Transmit (0x04 for auto enqueue)
    enet_regwrite(0x74, 0x0013);  // RXCR1 receive enabled, promiscuous mode
    enet_regwrite(0x76, 0x0084);  // RXCR2 SPI receive single frame data burst
    enet_regwrite(0x9c, 0x0001);  // Interrupt after 1 frame
    enet_regwrite(0x82, 0x0030);  // RXQCR enable receive threshold, auto-dequeue
        
    enet_txpacket(testpacket, PACKET_LEN);  // Transmit packet
    
    EZDSP5502_waitusec(1000);

    while((enet_regread(0x92) & 0x2000) == 0);  // Wait for packet reception

    enet_regwrite(0x92, enet_regread(0x92) | 0x2000);  // Clear receive ISR bit

    len = enet_rxpacket(rx);
//    printf("   --> 0x%04x bytes received.\n", len);
    if (len != PACKET_LEN) {
//        printf("   --> Length should be 0x%04x\n", PACKET_LEN);
        return -1;
    }
    
    for (i = 0; i < PACKET_LEN; i++)
        if (rx[i] != testpacket[i])
        {
//            printf("    --> Data does not match\n");
            return -1;
        }
    return 0;
}


