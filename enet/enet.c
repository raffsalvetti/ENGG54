
////////////////////////////////////////////////////////////////////////////
// * File name: enet.h
// *                                                                          
// * Description:  ENET functions.
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

#include "stdio.h"
#include "ezdsp5502.h"
#include "enet.h"

static Uint16 byteenable[4] = { 0x04, 0x08, 0x10, 0x20 };
static Uint8 cmdbuf[8];

Uint16 enet_regread(Uint16 regnum)
{
    if ((regnum & 0x02) == 0)
        ENET_CMD = 0x3000 | regnum;
    else
        ENET_CMD = 0xc000 | regnum;
    EZDSP5502_waitusec(1);
    return ENET_DATA;
}

void enet_regwrite(Uint16 regnum, Uint16 regdata)
{
    if ((regnum & 0x02) == 0)
        ENET_CMD = 0x3000 | regnum;
    else
        ENET_CMD = 0xc000 | regnum;
    EZDSP5502_waitusec(1);
    ENET_DATA = regdata;
}

Int16 enet_rxpacket(Uint8 *buf)
{
    Uint16 i;
    volatile Uint16 status, len, data;

    enet_regwrite(0x86, 0x4000);  // Set up receive auto-increment
 
    enet_regwrite(0x82, enet_regread(0x82) | 0x0008);  // RXQCR - Enable data FIFO access

    /* Receive packet header */
    status = ENET_DATA;      // Dummy read
    status = ENET_DATA;
    len = ENET_DATA - 4;
    
    /* Receive packet data */
    for (i = 0; i < len; i+=2)
    {
        data = ENET_DATA;
        buf[i] = data & 0xff;
        buf[i + 1] = (data >> 8) & 0xff;
    }

    enet_regwrite(0x82, enet_regread(0x82) & 0xfff7);  // RXQCR - Disable data FIFO access

    return len;
}

void enet_txpacket(Uint8 *buf, Uint16 len)
{
    Uint16 i;
    
    /* Enable transfer data mode */
    enet_regwrite(0x82, 0x08 | 0x0030);  // RXQCR - Enable data FIFO access

    /* Send packet header */
    ENET_DATA = 0x0000;       // Command word
    ENET_DATA = len;          // Len
      
    /* Send packet data */
    for (i = 0; i < len; i+=2)
        ENET_DATA = buf[i] | (buf[i+1] << 8);
        
    /* Disable transfer data mode */    
    enet_regwrite(0x82, 0x00 | 0x30);  // RXQCR - Disable data FIFO access
}

