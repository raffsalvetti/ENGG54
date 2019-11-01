//////////////////////////////////////////////////////////////////////////////
// * File name: ezdsp5502_i2c.c
// *                                                                          
// * Description:  I2C GPIO implementation.
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
#include "ezdsp5502_i2cgpio.h"

/*
 *
 *  EZDSP5502_I2CGPIO_configLine( line, dir )
 *
 *      Uint16 line        <-  I2C GPIO line (defined in ezdsp5502_i2cgpio.h)
 *      Uint16 dir         <-  I2C GPIO direction (defined in ezdsp5502_i2cgpio.h)
 * 
 *  Description
 *      Configure the I2C GPIO line as input or output.
 *
 */
Int16 EZDSP5502_I2CGPIO_configLine(  Uint16 line, Uint16 dir )
{
    Uint16 regnum;
    Uint16 regval;
    Uint16 bit = 0;
    
    if(line < 9)
        regnum = 6;
    else
    {
        regnum = 7;
        line = line - 8;
    }
        
    EZDSP5502_I2CGPIO_read(  regnum, &regval );
    
    bit = 1 << line;
    regval &= ~bit;
    dir = dir << line;
    regval = regval | dir; 
    
    EZDSP5502_I2CGPIO_write( regnum, regval );
    
    return 0;
}

/*
 *
 *  EZDSP5502_I2CGPIO_readLine( line )
 *
 *      Uint16 line        <-  I2C GPIO line (defined in ezdsp5502_i2cgpio.h)
 * 
 *      Returns:           ->  I2C GPIO STATE (defined in ezdsp5502_i2cgpio.h)
 * 
 *  Description
 *      Returns the state of I2C GPIO "line" as state high or low.
 *
 */
Int16 EZDSP5502_I2CGPIO_readLine(  Uint16 line )
{
    Uint16 regnum;
    Uint16 regval;
    
    if(line < 9)
        regnum = 0;
    else
    {
        regnum = 1;
        line = line - 8;
    }
    EZDSP5502_I2CGPIO_read(  regnum, &regval );
    
    return ((regval >> line) & 0x01);
}

/*
 *
 *  EZDSP5502_I2CGPIO_writeLine(  line, val )
 *
 *      Uint16 line        <-  I2C GPIO line  (defined in ezdsp5502_i2cgpio.h)
 *      Uint16 val         <-  I2C GPIO state (defined in ezdsp5502_i2cgpio.h)
 * 
 *  Description
 *      Sets the state of I2C GPIO "line" as state high or low.
 *
 */
Int16 EZDSP5502_I2CGPIO_writeLine(  Uint16 line, Uint16 val )
{
    Uint16 regnum;
    Uint16 regval;
    Uint16 bit = 0;
    
    if(line < 9)
        regnum = 2;
    else
    {
        regnum = 3;
        line = line - 8;
    }
        
    EZDSP5502_I2CGPIO_read(  regnum, &regval );
    
    bit = 1 << line;
    regval &= ~bit;
    val = val << line;
    regval = regval | val; 
    
    EZDSP5502_I2CGPIO_write( regnum, regval );
    
    return 0;
}

/*
 *
 *  EZDSP5502_I2CGPIO_read( regnum, *regval )
 *
 *      Uint16 regnum       <- I2C GPIO register number
 *      Uint16* regval      <- Pointer to store register value
 * 
 *  Description
 *      Return value of I2C GPIO register regnum to pointer revgal
 *
 */
Int16 EZDSP5502_I2CGPIO_read(  Uint16 regnum, Uint16* regval )
{
    Int16  retcode = 0;
    Uint16 cmd[2];

    cmd[0] = regnum & 0x007F;     // 7-bit Device Register
    cmd[1] = 0;

    /* Send I2C GPIO register name */
    retcode |= EZDSP5502_I2C_write( I2CGPIO_I2C_ADDR, cmd, 1 );
    
    /* Return I2C GPIO register value */
    retcode |= EZDSP5502_I2C_read( I2CGPIO_I2C_ADDR, cmd, 1 );
    *regval = cmd[0];
    EZDSP5502_waitusec( 50 );
    
    return retcode;
}

/*
 *
 *  EZDSP5502_I2CGPIO_write( regnum, regval )
 *
 *      Uint16 regnum       <- I2C GPIO register number
 *      Uint16 regval       <- Value to write into 8-bit register
 * 
 *  Description
 *      Writes the value regval to the I2C GPIO register regnum
 *
 */
Int16 EZDSP5502_I2CGPIO_write( Uint16 regnum, Uint16 regval )
{
    Uint16 cmd[2];
    
    cmd[0] = regnum & 0x007F;  // 7-bit Device Register
    cmd[1] = regval;           // 8-bit Register Data

    EZDSP5502_waitusec( 100 );

    /* Write to I2C GPIO Register */
    return EZDSP5502_I2C_write( I2CGPIO_I2C_ADDR, cmd, 2 );
}

