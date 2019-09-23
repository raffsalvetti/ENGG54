# Codec de áudio

A placa do EZDSP5502 possui dois plugues para conector de áudio, sendo um de entrada e um de saída. Ambos estão conectados ao chip TLV320AIC3204 que é um codec de áudio estéreo com baixo consumo de potência.
Para configurar esse codec, é utilizado o protocolo I2C, fazendo envio de mensagens para ele. Já para enviar os dados a serem reproduzidos é necessário usar a MCBSP (Multi Channel Buffered Serial Port), que é basicamente uma porta serial multiplexada, para selecionar o codec nesse multiplexador é utilizado o expansor de entrada e saída, que por sua vez é controlado por I2C. Segue detalhamento abaixo.

## Configuração no código
Headers importantes:
```c
//Inicialização da placa e esperas
#include "ezdsp5502.h"
//Comunicar I2C com o codec para configurar
#include "ezdsp5502_i2c.h"
//Selecionar o canal do codec na MCBSP
#include "ezdsp5502_i2cgpio.h"
//Enviar e receber dados do codec
#include "ezdsp5502_mcbsp.h"
#include "csl_mcbsp.h"
```
BSP_SEL1 deve ser configurado como saída e estar em nível baixo para atuar como seletor nessa multiplexação. BSP_SEL1_ENn também deve estar como saída e nível baixo para habilitar o canal do codec.
```c
//Selecionar canal do codec na MCBSP
EZDSP5502_I2CGPIO_configLine( BSP_SEL1, OUT );
EZDSP5502_I2CGPIO_writeLine(  BSP_SEL1, LOW );
//Habilitar canal do codec na MCBSP
EZDSP5502_I2CGPIO_configLine( BSP_SEL1_ENn, OUT );
EZDSP5502_I2CGPIO_writeLine(  BSP_SEL1_ENn, LOW );
```
Para configurar o codec, é necessário enviar para ele mensagens pelo protocolo I2C com o número do registrador e valor que deseja alterá-lo para.
```c
//Endereço I2C do codec
#define AIC3204_I2C_ADDR 0x18
```
```c
//Exemplo de função para definir configuração
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
```
```c
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
```
```c
//Exemplo de linhas de configuração
AIC3204_rset( 0, 0 );      // Seleciona página 0
AIC3204_rset( 1, 1 );      // Reseta o codec
//Continua...
```
Para reproduzir ou capturar áudio nos plugues, primeiro é necessário inicializar a MCBSP e então fazer leituras ou escritas na mesma.
```c
//Como inicializar MCBSP
EZDSP5502_MCBSP_init( );
//Escrever para a saída
EZDSP5502_MCBSP_write( Int16 data );
//Ler da entrada
EZDSP5502_MCBSP_read( Int16* data );
//Como desabilitar MCBSP
EZDSP5502_MCBSP_close();
```
Os dados para reproduzir no codec podem ser, por exemplo, tabelados em look-up tables pré-geradas, como os exemplos abaixo que estão em ponto fixo 16 bits com sinal:
```c
//Sinal senoidal
Int16 sinetable[48] = {
    0x0000, 0x10b4, 0x2120, 0x30fb, 0x3fff, 0x4dea, 0x5a81, 0x658b,
    0x6ed8, 0x763f, 0x7ba1, 0x7ee5, 0x7ffd, 0x7ee5, 0x7ba1, 0x76ef,
    0x6ed8, 0x658b, 0x5a81, 0x4dea, 0x3fff, 0x30fb, 0x2120, 0x10b4,
    0x0000, 0xef4c, 0xdee0, 0xcf06, 0xc002, 0xb216, 0xa57f, 0x9a75,
    0x9128, 0x89c1, 0x845f, 0x811b, 0x8002, 0x811b, 0x845f, 0x89c1,
    0x9128, 0x9a76, 0xa57f, 0xb216, 0xc002, 0xcf06, 0xdee0, 0xef4c
};
//Sinal dente de serra
Int16 sawtable[48] = {
    0x0000, 0x02ab, 0x0556, 0x0801, 0x0aac, 0x0d57, 0x1002, 0x12ad, 
    0x1558, 0x1803, 0x1aae, 0x1d59, 0x2004, 0x22af, 0x255a, 0x2805, 
    0x2ab0, 0x2d5b, 0x3006, 0x32b1, 0x355c, 0x3807, 0x3ab2, 0x3d5d, 
    0x4008, 0x42b3, 0x455e, 0x4809, 0x4ab4, 0x4d5f, 0x500a, 0x52b5, 
    0x5560, 0x580b, 0x5ab6, 0x5d61, 0x600c, 0x62b7, 0x6562, 0x680d, 
    0x6ab8, 0x6d63, 0x700e, 0x72b9, 0x7564, 0x780f, 0x7aba, 0x7d65
};
//Sinal triangular
Int16 triangletable[48] = {
    0x0000, 0x10b4, 0x2120, 0x30fb, 0x3fff, 0x4dea, 0x5a81, 0x658b,
    0x6ed8, 0x763f, 0x7ba1, 0x7ee5, 0x7ffd, 0x7ee5, 0x7ba1, 0x76ef,
    0x6ed8, 0x658b, 0x5a81, 0x4dea, 0x3fff, 0x30fb, 0x2120, 0x10b4,
    0x0000, 0xef4c, 0xdee0, 0xcf06, 0xc002, 0xb216, 0xa57f, 0x9a75,
    0x9128, 0x89c1, 0x845f, 0x811b, 0x8002, 0x811b, 0x845f, 0x89c1,
    0x9128, 0x9a76, 0xa57f, 0xb216, 0xc002, 0xcf06, 0xdee0, 0xef4c
};
```
Ao escrever nos dois canais, a primeira escrita será o canal esquerdo, logo em seguida será o direito, por exemplo:
```c
EZDSP5502_MCBSP_write( sinetable[0] );      // Canal esquerdo primeiro
EZDSP5502_MCBSP_write( sinetable[0] );      // Canal direito depois
```
## Registradores do AIC3204
Para consultar o mapa de registradores do codec e suas funções, veja o [datasheet](http://www.ti.com/product/TLV320AIC3204 "Datasheet AIC3204") do mesmo. Este é um passo importante para verificar se está com as configurações adequadas ao seu projeto. São esses valores que são utilizados nas funções AIC3204_rset e AIC3204_rget acima descritas.

## Programa exemplo
O programa exemplo consiste num loop infinito em que caso seja pressionado um botão, uma música é tocada usando ondas quadradas e caso seja pressionado o outro botão, começa a soar um alarme criado a partir de look up table pré gerada de onda senoidal.  

[Clique aqui](https://imgur.com/a/TsjAXtQ "Demonstração do Codec AIC3204 do EZDSP5502") para assistir a um vídeo curto de demonstração do programa executando e produzindo som.

```c
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

//Endereço I2C do codec
#define AIC3204_I2C_ADDR 0x18
#define TABLESIZE 1600

//Frequencias das notas
#define C 131
#define D 147
#define E 165
#define F 175
#define G 196
#define A 220
#define B 247

//Notas musicais na ordem
Int16 song[] = {F, A, B, F, A, B, E*2, D*2, B, C*2, B, G, E, D, E, G, E, F, A, B, F, A, B, E*2, D*2, B, C*2, E*2, G, E, D, E, G, E, D, E, F, G, A, B, D, E, F, C*2, D*2, E*2, D, E, F, G, A, B, B, A, C, B, D, C, E, F, E, E};

//Look-up table de seno
Int16 sinetable[1600] = {...}; //Omitida por simplicidade, ver código fonte

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
}

void main( void )
{
	Int16 songPtr = 0;
	Int16 freq10 = song[0];
	Int16 i = 0;
	Int16 sample = 0;
	Int16 freq = 80;
	Int16 sample2 = 0;
	
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
    
    while(1)
    {
    	//Esse botao toca música
    	if(!EZDSP5502_I2CGPIO_readLine(SW0))
    	{
    		songPtr = 0;
    		while(songPtr < 60)
		    {
		    	freq10 = song[songPtr];
		    	for(i = 0; i < freq10*4; i++)
		    	{
			    	EZDSP5502_MCBSP_write(32767);
				    EZDSP5502_MCBSP_write(32767);
				    EZDSP5502_waitusec(50000/freq10);
				    EZDSP5502_MCBSP_write(-32767);
				    EZDSP5502_MCBSP_write(-32767);
				    EZDSP5502_waitusec(50000/freq10);
		    	}
		    	songPtr++;
		    }
    	}
    	//Esse botao inicia alarme
	    else if(!EZDSP5502_I2CGPIO_readLine(SW1))
	    {
		    freq = 80;
		    for(i = 0; i < 200; i++)
		    {
		    	if(freq > 280) freq = 80;
		    	for(sample = 0; sample < 5000; sample++)
		    	{
			    	EZDSP5502_MCBSP_write(sinetable[sample2] );      // TX left channel first (FS Low)
			        EZDSP5502_MCBSP_write(sinetable[sample2] );      // TX right channel next (FS High)
			        sample2 += freq;
			        if(sample2 > TABLESIZE-1) sample2 = sample2 - TABLESIZE;
			    }
			    freq += 10;
		    }
	    }
	    
    }
    
    EZDSP5502_MCBSP_close(); // Desabilitar McBSP
}
```
