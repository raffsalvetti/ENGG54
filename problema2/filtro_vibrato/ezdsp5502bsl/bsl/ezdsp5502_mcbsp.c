//////////////////////////////////////////////////////////////////////////////
// * File name: ezdsp5502_mcbsp.c
// *                                                                          
// * Description:  I2C implementation.
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
 
#include "ezdsp5502_mcbsp.h"
#include "csl_mcbsp.h"

MCBSP_Handle aicMcbsp;

/*
 *
 *  EZDSP5502_MCBSP_init( )
 *
 *  Description
 *      Enable and initalize the MCBSP module for use
 *      with the AIC3204.
 *
 */
Int16 EZDSP5502_MCBSP_init( )
{
    /* Set values for MCBSP configuration structure */
    MCBSP_Config ConfigLoopBack32= {
         MCBSP_SPCR1_RMK(
          MCBSP_SPCR1_DLB_OFF,          // DLB    = 0
          MCBSP_SPCR1_RJUST_RZF,        // RJUST  = 0 (Right Justified)
          MCBSP_SPCR1_CLKSTP_DISABLE,   // CLKSTP = 0
          MCBSP_SPCR1_DXENA_NA,         // DXENA  = 0
          MCBSP_SPCR1_ABIS_DISABLE,     // ABIS   = 0
          MCBSP_SPCR1_RINTM_RRDY,       // RINTM  = 0
          MCBSP_SPCR1_RSYNCERR_NO ,     // RSYNCERR = 0
          MCBSP_SPCR1_RRST_DISABLE      // RRST   = 0
          ),
        MCBSP_SPCR2_RMK(
          MCBSP_SPCR2_FREE_NO,          // FREE   = 1 (Free running)
          MCBSP_SPCR2_SOFT_NO,          // SOFT   = 0
          MCBSP_SPCR2_FRST_RESET,       // FRST   = 0 (External FS)
          MCBSP_SPCR2_GRST_RESET,       // GRST   = 0 (All Clocks External)
          MCBSP_SPCR2_XINTM_XRDY,       // XINTM  = 0
          MCBSP_SPCR2_XSYNCERR_NO,      // XSYNCERR = N/A
          MCBSP_SPCR2_XRST_DISABLE      // XRST   = 0
          ),
        MCBSP_RCR1_RMK( 
          MCBSP_RCR1_RFRLEN1_OF(1),     // RFRLEN1 = 1 (2 Words)
          MCBSP_RCR1_RWDLEN1_16BIT      // RWDLEN1 = 2 (16-bit)
          ),
        MCBSP_RCR2_RMK(    
          MCBSP_RCR2_RPHASE_SINGLE,     // RPHASE  = 0 (Single Phase)
          MCBSP_RCR2_RFRLEN2_OF(0),     // RFRLEN2 = 0
          MCBSP_RCR2_RWDLEN2_8BIT,      // RWDLEN2 = 0
          MCBSP_RCR2_RCOMPAND_MSB,      // RCOMPAND = 0
          MCBSP_RCR2_RFIG_YES,          // RFIG    = 0
          MCBSP_RCR2_RDATDLY_1BIT       // RDATDLY = 11 (1-bit delay)
          ),  
        MCBSP_XCR1_RMK(    
          MCBSP_XCR1_XFRLEN1_OF(1),     // XFRLEN1 = 1 (2 Words)
          MCBSP_XCR1_XWDLEN1_16BIT      // XWDLEN1 = 2 (16-bit)
          ),   
        MCBSP_XCR2_RMK(   
          MCBSP_XCR2_XPHASE_SINGLE,     // XPHASE  = 0 (Single Phase)
          MCBSP_XCR2_XFRLEN2_OF(0),     // XFRLEN2 = 0
          MCBSP_XCR2_XWDLEN2_8BIT,      // XWDLEN2 = 0
          MCBSP_XCR2_XCOMPAND_MSB,      // XCOMPAND= 0
          MCBSP_XCR2_XFIG_YES,          // XFIG    = 0
          MCBSP_RCR2_RDATDLY_1BIT       // XDATDLY = 1 (1-bit delay)
          ),            
        MCBSP_SRGR1_RMK( 
          MCBSP_SRGR1_FWID_OF(0),      // FWID    = 0 (All Clocks External)
          MCBSP_SRGR1_CLKGDV_OF(0)     // CLKGDV  = 0 (All Clocks External)
          ),   
        MCBSP_SRGR2_RMK(  
          MCBSP_SRGR2_GSYNC_FREE,      // FREE    = 0
          MCBSP_SRGR2_CLKSP_RISING,    // CLKSP   = 0
          MCBSP_SRGR2_CLKSM_CLKS,      // CLKSM   = 0
          MCBSP_SRGR2_FSGM_DXR2XSR,    // FSGM    = 0
          MCBSP_SRGR2_FPER_OF(0)       // FPER    = 0
          ),  
        MCBSP_MCR1_DEFAULT,
        MCBSP_MCR2_DEFAULT, 
        MCBSP_PCR_RMK(
          MCBSP_PCR_XIOEN_SP,          /* XIOEN    = 0   */
          MCBSP_PCR_RIOEN_SP,          /* RIOEN    = 0   */
          MCBSP_PCR_FSXM_EXTERNAL,     /* FSXM     = 0   */
          MCBSP_PCR_FSRM_EXTERNAL,     /* FSRM     = 0   */
          MCBSP_PCR_CLKXM_INPUT,       /* CLKXM    = 0   */
          MCBSP_PCR_CLKRM_INPUT,       /* CLKRM    = 0   */
          MCBSP_PCR_SCLKME_NO,         /* SCLKME   = 0   */
          0,                           /* DXSTAT = N/A   */
          MCBSP_PCR_FSXP_ACTIVEHIGH,   /* FSXP     = 0   */
          MCBSP_PCR_FSRP_ACTIVELOW,   /* FSRP     = 0   */
          MCBSP_PCR_CLKXP_FALLING,     /* CLKXP    = 1   */
          MCBSP_PCR_CLKRP_FALLING      /* CLKRP    = 1   */
          ),
        MCBSP_RCERA_DEFAULT, 
        MCBSP_RCERB_DEFAULT, 
        MCBSP_RCERC_DEFAULT, 
        MCBSP_RCERD_DEFAULT, 
        MCBSP_RCERE_DEFAULT, 
        MCBSP_RCERF_DEFAULT, 
        MCBSP_RCERG_DEFAULT, 
        MCBSP_RCERH_DEFAULT, 
        MCBSP_XCERA_DEFAULT,
        MCBSP_XCERB_DEFAULT,
        MCBSP_XCERC_DEFAULT,
        MCBSP_XCERD_DEFAULT,  
        MCBSP_XCERE_DEFAULT,
        MCBSP_XCERF_DEFAULT,  
        MCBSP_XCERG_DEFAULT,
        MCBSP_XCERH_DEFAULT
        };

    /* Close any previous instance of the McBSP */
    EZDSP5502_MCBSP_close( );

    /* Open McBSP port and associate with handle */
    aicMcbsp = MCBSP_open(MCBSP_PORT1, MCBSP_OPEN_RESET);

    /* Configure McBSP with values */
    MCBSP_config(aicMcbsp, &ConfigLoopBack32);
    
    /* Enable MCBSP transmit and receive */
    MCBSP_start(aicMcbsp,MCBSP_RCV_START | MCBSP_XMIT_START,0 );
    EZDSP5502_waitusec( 42 ); // Wait 2 sample rate periods

    return 0;
}

/*
 *
 *  EZDSP5502_MCBSP_close( )
 *
 *  Description
 *      Closes MCBSP associated with handle
 *
 */
Int16 EZDSP5502_MCBSP_close( )
{
    MCBSP_close(aicMcbsp);

    return 0;
}

/*
 *
 *  EZDSP5502_MCBSP_write( data )
 *
 *      Int16 data   <-  16-bit audio data
 * 
 *  Description
 *      Sends data out the McBSP
 *
 */
void EZDSP5502_MCBSP_write( Int16 data )
{
    while(!MCBSP_xrdy(aicMcbsp));
    MCBSP_write16(aicMcbsp, data);
}

/*
 *
 *  EZDSP5502_MCBSP_read( *data )
 *
 *      Int16* data   <-  Pointer to location to store received data
 * 
 *  Description
 *      Copies data received by the McBSP to the location of the pointer
 *
 */
void EZDSP5502_MCBSP_read( Int16* data )
{
    while(!MCBSP_rrdy(aicMcbsp));
    *data = MCBSP_read16(aicMcbsp);
}



