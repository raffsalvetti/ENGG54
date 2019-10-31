//Inicialização da placa e esperas
#include "ezdsp5502.h"
//Comunicar I2C com o codec para configurar
#include "ezdsp5502_i2c.h"
//Selecionar o canal do codec na MCBSP
#include "ezdsp5502_i2cgpio.h"
//Enviar e receber dados do codec
#include "ezdsp5502_mcbsp.h"
#include "csl_mcbsp.h"
//Para utilizar os botões
#include "csl_gpio.h"
//dados guardados em segmentos da memória

#include "vibrato.h"

//Endereço I2C do codec
#define AIC3204_I2C_ADDR 0x18

//Selecionar codec no mux da mcbsp
void select_codec( void )
{
	//Selecionar canal do codec na MCBSP
	EZDSP5502_I2CGPIO_configLine( BSP_SEL1, OUT );
	EZDSP5502_I2CGPIO_writeLine(  BSP_SEL1, LOW );
	//Habilitar canal do codec na MCBSP
	EZDSP5502_I2CGPIO_configLine( BSP_SEL1_ENn, OUT );
	EZDSP5502_I2CGPIO_writeLine(  BSP_SEL1_ENn, LOW );
}

//O regnum é o número do registrador no codec e regval o valor desejado
Int16 AIC3204_rset( Uint16 regnum, Uint16 regval )
{
    //Array de dois elementos de 16 bits para conter os dados a serem enviados
    Uint16 cmd[2];
    cmd[0] = regnum & 0x007F;       // 7-bit Registrador
    cmd[1] = regval;                // 8-bit Dados
    EZDSP5502_waitusec( 100 );      // Esperar 100 microsegundos
    //Mandar via I2C para ser escrito no registrador do codec
    //Argumento 1 é o endereço I2C, 2 é um array de dados 16 bits, 3 é o tamanho da array
    return EZDSP5502_I2C_write( AIC3204_I2C_ADDR, cmd, 2 );
}

//também é possível ler uma configuração atual de forma similar
Int16 AIC3204_rget(  Uint16 regnum, Uint16* regval )
{
    Int16  retcode = 0;
    Uint16 cmd[2];
    cmd[0] = regnum & 0x007F;       // 7-bit Registrador
    cmd[1] = 0;
    //Informar qual registrador
    retcode |= EZDSP5502_I2C_write( AIC3204_I2C_ADDR, cmd, 1 );
    //Receber valor do registrador
    retcode |= EZDSP5502_I2C_read( AIC3204_I2C_ADDR, cmd, 1 );
    *regval = cmd[0];
    //Esperar 50 microsegundos
    EZDSP5502_waitusec( 50 );
    return retcode;
}

//Exemplo de função para definir configuração do codec
void configure_AIC3204( void )
{
	/* ---------------------------------------------------------------- *
     *  Configure AIC3204                                               *
     * ---------------------------------------------------------------- */
    AIC3204_rset( 0, 0 );      // Select page 0
    AIC3204_rset( 1, 1 );      // Reset codec
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 1, 8 );      // Disable crude AVDD generation from DVDD
    AIC3204_rset( 2, 1 );      // Enable Analog Blocks, use LDO power
    AIC3204_rset( 0, 0 );
    
    /* PLL and Clocks config and Power Up  */
    AIC3204_rset( 27, 0x0d );  // BCLK and WCLK are set as o/p; AIC3204(Master)
    AIC3204_rset( 28, 0x00 );  // Data ofset = 0
    AIC3204_rset( 4, 3 );      // PLL setting: PLLCLK <- MCLK, CODEC_CLKIN <-PLL CLK
    AIC3204_rset( 6, 7 );      // PLL setting: J=7
    AIC3204_rset( 7, 0x06 );   // PLL setting: HI_BYTE(D=1680)
    AIC3204_rset( 8, 0x90 );   // PLL setting: LO_BYTE(D=1680)
    AIC3204_rset( 30, 0x9C );  // For 32 bit clocks per frame in Master mode ONLY
                               // BCLK=DAC_CLK/N =(12288000/8) = 1.536MHz = 32*fs
    AIC3204_rset( 5, 0x91 );   // PLL setting: Power up PLL, P=1 and R=1
    AIC3204_rset( 13, 0 );     // Hi_Byte(DOSR) for DOSR = 128 decimal or 0x0080 DAC oversamppling
    AIC3204_rset( 14, 0x80 );  // Lo_Byte(DOSR) for DOSR = 128 decimal or 0x0080
    AIC3204_rset( 20, 0x80 );  // AOSR for AOSR = 128 decimal or 0x0080 for decimation filters 1 to 6
    AIC3204_rset( 11, 0x83 );  // Power up NDAC and set NDAC value to 3***
    AIC3204_rset( 12, 0x87 );  // Power up MDAC and set MDAC value to 7
    AIC3204_rset( 18, 0x87 );  // Power up NADC and set NADC value to 7
    AIC3204_rset( 19, 0x83 );  // Power up MADC and set MADC value to 3***

    /* DAC ROUTING and Power Up */
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 0x0c, 8 );   // LDAC AFIR routed to HPL
    AIC3204_rset( 0x0d, 8 );   // RDAC AFIR routed to HPR
    AIC3204_rset( 0, 0 );      // Select page 0
    AIC3204_rset( 64, 2 );     // Left vol=right vol
    AIC3204_rset( 65, 0);      // Left DAC gain to 0dB VOL; Right tracks Left
    AIC3204_rset( 63, 0xd4 );  // Power up left,right data paths and set channel
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 9, 0x30 );   // Power up HPL,HPR
    AIC3204_rset( 0x10, 0x00 );// Unmute HPL , 0dB gain
    AIC3204_rset( 0x11, 0x00 );// Unmute HPR , 0dB gain
    AIC3204_rset( 0, 0 );      // Select page 0
    EZDSP5502_waitusec( 100 ); // wait
        
    /* ADC ROUTING and Power Up */
    AIC3204_rset( 0, 1 );      // Select page 1
    AIC3204_rset( 0x34, 0x30 );// STEREO 1 Jack
                               // IN2_L to LADC_P through 40 kohm
    AIC3204_rset( 0x37, 0x30 );// IN2_R to RADC_P through 40 kohmm
    AIC3204_rset( 0x36, 3 );   // CM_1 (common mode) to LADC_M through 40 kohm
    AIC3204_rset( 0x39, 0xc0 );// CM_1 (common mode) to RADC_M through 40 kohm
    AIC3204_rset( 0x3b, 0 );   // MIC_PGA_L unmute
    AIC3204_rset( 0x3c, 0 );   // MIC_PGA_R unmute
    AIC3204_rset( 0, 0 );      // Select page 0
    AIC3204_rset( 0x51, 0xc0 );// Powerup Left and Right ADC
    AIC3204_rset( 0x52, 0 );   // Unmute Left and Right ADC
    AIC3204_rset( 0, 0 );    
    EZDSP5502_waitusec( 200 ); // Wait
}

void main( void )
{
	Int16 samplePtr = 0;
	Int16 tablePtr = 0;
	Int16 signalPtr = 0;
	Int16 ampPtr = 0;
	Int16 tmp;
	
	const Int16 * samples;
	const Int16 * signal[SIGNAL_DURATION];

    //Inicializar BSL
    EZDSP5502_init( );
    
    //Selecionar codec
    select_codec();
    
    //Configurar codec
    configure_AIC3204();
    
    //Configurar push buttons
    EZDSP5502_I2CGPIO_configLine(  SW0, IN );
    EZDSP5502_I2CGPIO_configLine(  SW1, IN );
    
    //Iniciar funcionamento da MCBSP
    EZDSP5502_MCBSP_init( );
    
    //
    signal[0] = samples0;
    signal[1] = samples1;
    signal[2] = samples2;
    signal[3] = samples3;
    signal[4] = samples4;
    signal[5] = samples5;
    signal[6] = samples6;
    //signal[7] = samples7;

    while(1)
    {
        if(!EZDSP5502_I2CGPIO_readLine(SW0))
        {
            tablePtr = 0;
            ampPtr = 0;
            for(signalPtr = 0; signalPtr < SIGNAL_DURATION; signalPtr++)
            {
                samples = signal[signalPtr];
                for(samplePtr = 0; samplePtr < SIGNAL_SIZE; samplePtr++)
                    {

                        ( samplePtr < TABLE_MAX ) ?
                                ( tmp = samples[samplePtr] ) :
                                ( tmp = (samples[samplePtr])*_A_ + (samples[ samplePtr - TABLE_AVERAGE ])*_B_ + (samples[ samplePtr - (lookUpTable[tablePtr]) ])*_C_ );

                        tmp *= ( ( ( (float) lookUpTable[ampPtr]/TABLE_MAX ) + 1 ) / 2 );

                        (tablePtr+TABLE_STEP > TABLE_SIZE-1) ? (tablePtr -= TABLE_SIZE) : (tablePtr += TABLE_STEP);
                        (ampPtr+AMP_STEP > TABLE_SIZE-1) ? (ampPtr -= TABLE_SIZE) : (ampPtr += AMP_STEP);
                        EZDSP5502_MCBSP_write(tmp);
                        EZDSP5502_MCBSP_write(tmp);
                    }
            }
        }
        else if(!EZDSP5502_I2CGPIO_readLine(SW1))
        {
            for(signalPtr = 0; signalPtr < SIGNAL_DURATION; signalPtr++)
            {
                samples = signal[signalPtr];
                for(samplePtr = 0; samplePtr < SIGNAL_SIZE; samplePtr++)
                {
                    tmp = samples[samplePtr];
                    EZDSP5502_MCBSP_write(tmp);
                    EZDSP5502_MCBSP_write(tmp);
                }
            }
        }
        else
        {
            EZDSP5502_MCBSP_write(0);
            EZDSP5502_MCBSP_write(0);
        }
    }
    
    EZDSP5502_MCBSP_close(); // Desabilitar McBSP
}
