//////////////////////////////////////////////////////////////////////////////
// * File name: usbstk5502_gpio.c
// *                                                                          
// * Description:  GPIO implementation.
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

#include "ezdsp5502_gpio.h"
#include "csl_gpio.h"
#include "csl_chip.h"

/* GPIO handles for each group */
GPIO_Handle hGPIO_group0;
GPIO_Handle hGPIO_group1;
GPIO_Handle hGPIO_group2;
GPIO_Handle hGPIO_group3;

/*
 *
 *  EZDSP5502_GPIO_init( gpSel )
 *
 *      Uint32 gpSel       <- GPIO selections for a particular group
 *                            
 *
 *  Description
 *      Configures the selected pins "gpSel" of a particular GPIO 
 *      group to for GPIO use. Pins of a particular group must be set 
 *      together.
 *
 */
Int16 EZDSP5502_GPIO_init( Uint32 gpSel )
{
    Uint32 group = gpSel >> 16;  // Get group

    /* Set to PGPIO mode */
//    CHIP_RSET(XBSR, CHIP_XBSR_PPMODE_PPXPGPIO); // PGPIO mode disables SDRAM
    
    /* Open each group separately */
    switch(group)
    {
        case 0:
            hGPIO_group0 = GPIO_open(gpSel, 0);
            GPIO_pinEnable(hGPIO_group0, gpSel);
            break;
        case 1:
            hGPIO_group1 = GPIO_open(gpSel, 0);
            GPIO_pinEnable(hGPIO_group1, gpSel);
            break;          
        case 2:
            hGPIO_group2 = GPIO_open(gpSel, 0);
            GPIO_pinEnable(hGPIO_group2, gpSel);
            break;          
        case 3:
            hGPIO_group3 = GPIO_open(gpSel, 0);
            GPIO_pinEnable(hGPIO_group3, gpSel);
            break;          
    }
    return 0;
}

/*
 *
 *  EZDSP5502_GPIO_setDirection( gpSel, gpDirection )
 *
 *      Uint32 gpSel       <- GPIO selections for a particular group
 *      Uint16 gpDirection <- GPIO directions for each pin in a group
 *                            
 *
 *  Description
 *      Configures the selected pins "gpSel" of a particular GPIO 
 *      group to "gpDirection". Pins of a particular group must be set 
 *      together.
 *
 */
Int16 EZDSP5502_GPIO_setDirection( Uint32 gpSel, Uint16 gpDirection )
{
    Uint32 group = gpSel >> 16;  // Get group

    /* Select Direction for each group separately */
    switch(group)
    {
        case 0:
            GPIO_pinDirection(hGPIO_group0, gpSel, gpDirection);
            break;
        case 1:
            GPIO_pinDirection(hGPIO_group1, gpSel, gpDirection);
            break;
        case 2:
            GPIO_pinDirection(hGPIO_group2, gpSel, gpDirection);
            break;
        case 3:
            GPIO_pinDirection(hGPIO_group3, gpSel, gpDirection);
            break;
    }

    return 0;
}

/*
 *
 *  EZDSP5502_GPIO_setOutput( gpSel, output )
 *
 *      Uint32 Uint32 gpSel   <- GPIO number
 *      Uint16 output         <- 0 : GPIO outputs are logic low 
 *                               1 : GPIO outputs are logic high
 *
 *  Description
 *      Configures the selected pins "gpSel" of a particular GPIO 
 *      group to to the high or low state. The GPIO must be an output.
 *
 */
Int16 EZDSP5502_GPIO_setOutput( Uint32 gpSel, Uint16 output )
{
    Uint32 group = gpSel >> 16;  // Get group

    /* Set GPIO outputs for groups separately */
    switch(group)
    {
        case 0:
            GPIO_pinWrite(hGPIO_group0, gpSel, output);
            break;
        case 1:
            GPIO_pinWrite(hGPIO_group1, gpSel, output);
            break;
        case 2:
            GPIO_pinWrite(hGPIO_group2, gpSel, output);
            break;
        case 3:
            GPIO_pinWrite(hGPIO_group3, gpSel, output);
            break;
    }     
    
    return 0;
}

/*
 *
 *  EZDSP5502_GPIO_getInput( pinId )
 *
 *      Uint32 pinId            <-  GPIO pin ID
 *
 *      Returns: Uint16 readVal ->  0 : Input to the GPIO is logic low
 *                                  1 : Input to the GPIO is logic high
 *
 *  Description
 *      Returns 0 if the GPIO "number" is in the low state and 1 if it is 
 *      in the high state.
 *
 */
Int16 EZDSP5502_GPIO_getInput( Uint32 gpSel )
{
    Uint16 readVal;
    Uint32 group = gpSel >> 16;  // Get group

    /* Get GPIO inputs for groups separately */ 
    switch(group)
    {
        case 0:
            readVal = GPIO_pinRead(hGPIO_group0,gpSel);
            break;
        case 1:
            readVal = GPIO_pinRead(hGPIO_group1,gpSel);
            break;
        case 2:
            readVal = GPIO_pinRead(hGPIO_group2,gpSel);
            break;
        case 3:
            readVal = GPIO_pinRead(hGPIO_group3,gpSel);
            break;
    }
    
    return readVal;
}
