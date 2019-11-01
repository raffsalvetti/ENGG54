//////////////////////////////////////////////////////////////////////////////
// * File name: main.c
// *                                                                          
// * Description:  Main function.
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

extern Int16 enet_test( );

int  TestFail    = (int)-1;   

void StopTest()
{
    //SW_BREAKPOINT;
    return;
}

/*
 *
 *  main( )
 *
 */
void main( void )
{
    /* Initialize BSL */
    EZDSP5502_init( );

    EZDSP5502_I2CGPIO_configLine(  LED0, OUT );
    EZDSP5502_I2CGPIO_configLine(  LED1, OUT );
    EZDSP5502_I2CGPIO_configLine(  LED2, OUT );
    EZDSP5502_I2CGPIO_configLine(  LED3, OUT );

    //desligar todos os leds
    EZDSP5502_I2CGPIO_writeLine(   LED0, HIGH );
    EZDSP5502_I2CGPIO_writeLine(   LED1, HIGH );
    EZDSP5502_I2CGPIO_writeLine(   LED2, HIGH );
    EZDSP5502_I2CGPIO_writeLine(   LED3, HIGH );

//    EZDSP5502_I2CGPIO_writeLine(   LED0, LOW ); //AZUL
//    EZDSP5502_I2CGPIO_writeLine(   LED1, LOW ); //LARANJA
//    EZDSP5502_I2CGPIO_writeLine(   LED2, LOW ); //VERMELHO
//    EZDSP5502_I2CGPIO_writeLine(   LED3, LOW ); //VERDE

    /* Display test ID */
//    printf( "\nTesting Ethernet...\n");

    /* Call test function */
    TestFail = enet_test( );

    /* Check for test fail */
    if ( TestFail != 0 )
    {
        /* Print error message */
//        printf( "     FAIL... error code %d... quitting\n", TestFail );
        EZDSP5502_I2CGPIO_writeLine(   LED2, LOW );
    }
    else
    {
        /* Print pass message */
//        printf( "    PASS\n" );
//        printf( "\n***ALL Tests Passed***\n" );
        EZDSP5502_I2CGPIO_writeLine(   LED3, LOW );
    }
    
    StopTest();
}
