#define AIC3204_I2C_ADDR 0x18

#include "ezdsp5502.h"
#include "ezdsp5502_i2c.h"
#include "ezdsp5502_i2cgpio.h"
#include "ezdsp5502_mcbsp.h"
#include "csl_mcbsp.h"
#include "csl_gpio.h"

#define AZUL        LED0
#define AMARELO     LED1
#define VERMELHO    LED2
#define VERDE       LED3

#define ON          LOW
#define OFF         HIGH

#pragma DATA_SECTION(samples_0, ".samp_0")
#pragma DATA_SECTION(samples_1, ".samp_1")
#pragma DATA_SECTION(samples_2, ".samp_2")
#pragma DATA_SECTION(samples_3, ".samp_3")
#pragma DATA_SECTION(samples_4, ".samp_4")
#pragma DATA_SECTION(samples_5, ".samp_5")
#pragma DATA_SECTION(samples_6, ".samp_6")
#pragma DATA_SECTION(samples_7, ".samp_7")
#pragma DATA_SECTION(samples_8, ".samp_8")
#pragma DATA_SECTION(samples_9, ".samp_9")
#pragma DATA_SECTION(samples_10, ".samp_10")
#pragma DATA_SECTION(samples_11, ".samp_11")

#include "samples.h"

Int16 AIC3204_rget(  Uint16 regnum, Uint16* regval )
{
    Int16  retcode = 0;
    Uint16 cmd[2];

    cmd[0] = regnum & 0x007F;       // 7-bit Device Register
    cmd[1] = 0;

    /* Send AIC3204 register name */
    retcode |= EZDSP5502_I2C_write( AIC3204_I2C_ADDR, cmd, 1 );

    /* Return AIC3204 register value */
    retcode |= EZDSP5502_I2C_read( AIC3204_I2C_ADDR, cmd, 1 );
    *regval = cmd[0];
    EZDSP5502_waitusec( 50 );

    return retcode;
}

/*
 *
 *  AIC3204_rset( regnum, regval )
 *
 *      Set codec register regnum to value regval
 *
 */
Int16 AIC3204_rset( Uint16 regnum, Uint16 regval )
{
    Uint16 cmd[2];
    cmd[0] = regnum & 0x007F;       // 7-bit Device Register
    cmd[1] = regval;                // 8-bit Register Data

    EZDSP5502_waitusec( 100 );

    /* Write to AIC3204 Register */
    return EZDSP5502_I2C_write( AIC3204_I2C_ADDR, cmd, 2 );
}

void offleds() {
    //desligar todos os leds
    EZDSP5502_I2CGPIO_writeLine(   LED0, HIGH );
    EZDSP5502_I2CGPIO_writeLine(   LED1, HIGH );
    EZDSP5502_I2CGPIO_writeLine(   LED2, HIGH );
    EZDSP5502_I2CGPIO_writeLine(   LED3, HIGH );
}

typedef Int16 (*tiltro_t)(const Int16 *, Int16);

Int16 filtro0(const Int16 *input, Int16 sample) { //sem filrto. som original
    return input[sample];
}

Int16 filtro1(const Int16 *input, Int16 sample) {
    float alpha = 10; //distorcao
    float mix = 0.15; //mistura dos sinais
    Int16 q = input[sample] / abs( input[sample] );
    q = q * ( 1 - exp( alpha * ( q * input[sample] ) ) );
    return mix * q + (1 - mix) * input[sample];
}

Int16 filtro2(const Int16 *input, Int16 sample) {
    float delay = 0.5; //em segundos
    float alpha = 0.65; //forca do eco
    Int16 D = delay * FREQUENCY_SAMPLE;
    if(sample < D)
        D = sample;
    return input[sample] + alpha * input[sample - D];
}

Int16 filtro3(const Int16 *input, Int16 sample) {
    float delay = 0.1; // em segundos (valor bem pequeno por causa do feedback)
    float alpha = 0.3; // volume
    float beta = 0.0001; // ganho do feedback
    Int16 D = delay * FREQUENCY_SAMPLE;
    float k = 0;
    if (sample > D) {
        k = filtro3(input, sample-D);
    }
    return alpha * input[sample] - (beta * D) * k;
}

tiltro_t filtro_ativo = &filtro0;

tiltro_t filtros[4] = {&filtro0, &filtro1, &filtro2, &filtro3};
char idx_filtro = 3;


void next_filter() {
    idx_filtro++;
    if(idx_filtro > 3)
        idx_filtro = 0;
    filtro_ativo = filtros[idx_filtro];
    offleds();
    EZDSP5502_I2CGPIO_writeLine(LED3+idx_filtro, ON);
}

int main(){
    Int16 sec, sample, level, n;
    char sw_pause, sw_select;

    EZDSP5502_init( );

    EZDSP5502_I2CGPIO_configLine(  SW0, IN );
    EZDSP5502_I2CGPIO_configLine(  SW1, IN );

    EZDSP5502_I2CGPIO_configLine(  LED0, OUT );
    EZDSP5502_I2CGPIO_configLine(  LED1, OUT );
    EZDSP5502_I2CGPIO_configLine(  LED2, OUT );
    EZDSP5502_I2CGPIO_configLine(  LED3, OUT );

    offleds();

    /* Set to McBSP1 mode */
    EZDSP5502_I2CGPIO_configLine( BSP_SEL1, OUT );
    EZDSP5502_I2CGPIO_writeLine(  BSP_SEL1, LOW );

    /* Enable McBSP1 */
    EZDSP5502_I2CGPIO_configLine( BSP_SEL1_ENn, OUT );
    EZDSP5502_I2CGPIO_writeLine(  BSP_SEL1_ENn, LOW );

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
    AIC3204_rset( 11, 0x82 );  // Power up NDAC and set NDAC value to 2
    AIC3204_rset( 12, 0x87 );  // Power up MDAC and set MDAC value to 7
    AIC3204_rset( 18, 0x87 );  // Power up NADC and set NADC value to 7
    AIC3204_rset( 19, 0x82 );  // Power up MADC and set MADC value to 2

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

    /* Initialize McBSP */
    EZDSP5502_MCBSP_init( );

    level = HIGH;

    next_filter(); //reset nos filtros

    for(;;) {
        do {
            sw_pause = EZDSP5502_I2CGPIO_readLine(SW0);
            sw_select = EZDSP5502_I2CGPIO_readLine(SW1);
            if(!sw_select) {
                next_filter();
                for(sample = 0 ; sample < 500; sample++){
                    EZDSP5502_waitusec( 1000 );
                }
            }
        } while(sw_pause);

        for ( sec = 0 ; sec < NN_SAMPLES ; sec++ ) {
            for ( sample = 0 ; sample < N_SAMPLES - 1 ; sample++ ) {
                n = filtro_ativo(samples[sec], sample);
                EZDSP5502_MCBSP_write(n);      // TX left channel first (FS Low)
                EZDSP5502_MCBSP_write(n);      // TX right channel next (FS High)
            }
        }
    }

    EZDSP5502_MCBSP_close(); // Disable McBSP
    AIC3204_rset( 1, 1 );    // Reset codec

}
