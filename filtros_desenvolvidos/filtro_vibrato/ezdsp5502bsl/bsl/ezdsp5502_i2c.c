//////////////////////////////////////////////////////////////////////////////
// * File name: ezdsp5502_i2c.c
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
 
#include "ezdsp5502_i2c.h"
#include "csl_i2c.h"

/*
 *
 *  EZDSP5502_I2C_init( )
 *
 *  Description
 *      Enable and initalize the I2C module.
 *      The I2C clk is set to run at 100 KHz.
 *
 */
Int16 EZDSP5502_I2C_init( )
{
    /* Set I2C configuration Structure values for 100MHz Clock */
    I2C_Config Config = {
        0x03FF, // Own address
        0x0000, // Interrupt enable
        0xFFFF, // Status register
        0x000E, // Clock low-time divider  = 15
        0x000E, // Clock high-time divider = 15
        0x0001, // Data count register
        0x0018, // Slave address register
        0x0420, // Mode register
        0x0000, // Interrupt source register
        0x0013  // Prescaler register = 20
    };

    
    /* Configure I2C with values */
    I2C_config(&Config);

    return 0;
}

/*
 *
 *  EZDSP5502_I2C_reset( )
 *
 *  Description
 *      Resets I2C module
 *
 */
Int16 EZDSP5502_I2C_reset( )
{
    I2C_reset();
    
    return 0;
}

/*
 *
 *  EZDSP5502_I2C_write( i2c_addr, data, len )
 *
 *      Uint16 i2c_addr    <- I2C slave address
 *      Uint16* data       <- I2C data ptr
 *      Uint16 len         <- # of bytes to write
 *
 *  Description
 *      I2C write in Master mode: Writes to I2c device with address 
 *      "i2c_addr" from the location of "data" for length "len".
 *
 */
Int16 EZDSP5502_I2C_write( Uint16 i2c_addr, Uint16* data, Uint16 len )
{
    Int16         status;
    
    /* I2C Write (master, SADP, timeout = 3000) */
    status = I2C_write(data, len, 1,i2c_addr , 1, 3000);

    return status;
}

/*
 *
 *  EZDSP5502_I2C_read( i2c_addr, data, len )
 *
 *      Uint16 i2c_addr    <- I2C slave address
 *      Uint16* data       <- I2C data ptr
 *      Uint16 len         <- # of bytes to write
 *
 *  Description
 *      I2C read in Master mode: Reads from I2c device with address 
 *      "i2c_addr" and stores to the location of "data" for length "len".
 *      
 */
Int16 EZDSP5502_I2C_read( Uint16 i2c_addr, Uint16* data, Uint16 len )
{
    Int16         status;
    
    /* I2C Read (master, SADP, timeout = 3000, check bus) */
    status = I2C_read(data, len, 1,i2c_addr , 1, 3000, 1);
    
    return status;
}
