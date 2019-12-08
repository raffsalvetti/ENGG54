//////////////////////////////////////////////////////////////////////////////
// CÁLCULO DE THD PARA UMA SENÓIDE DE REFERÊNCIA (100 Hz de harmônica
// fundamental e amplitude determinada pelo sinal-exemplar)
// A resolução adotada foi a de 1 hertz/amostra (Fsampling = Length = 2048 (valor-limite para FFT nesta placa))
//////////////////////////////////////////////////////////////////////////////

#define L 512  // Comprimento do sinal
#define L2 L/2 //Metade do comprimento do sinal
#define integ_resol 3 //Numero de itens para integração do sinal
#define considerados 3 // Quantidade de harmônicos considerados

#define fs L //frequencia de amostragem em hertz
#define fg 100  //harmonica fundamental em hertz


#include "ezdsp5502.h"
#include "ezdsp5502_i2c.h"
#include "ezdsp5502_i2cgpio.h"
#include <tms320.h>
#include <Dsplib.h>
#include <math.h>
#include <MISC.H>

#pragma DATA_SECTION(ab_val,".infoD")



//float dados[2048]={0};







 void rfft_SCALE(DATA* x, ushort nx){
     cfft_SCALE(x,nx/2);
     EZDSP5502_I2CGPIO_writeLine(  BSP_SEL1, LOW );
     cbrev(x,x,nx/2);
     unpack(x,nx);


 }
 void rfft_NOSCALE(DATA* x, ushort nx){
     cfft_SCALE(x,nx/2);
     cbrev(x,x,nx/2);
     unpack(x,nx);


 }
         //Pegar módulo do número complexo em notação Q.15
 void absol(DATA* input, DATA* output, ushort size){ //size = tamanho do vetor original (incluindo partes reais e imaginarias);
         ushort index;
         LDATA mult1, mult2;

         for(index = 0; index < size; index=index+2){
             mult1 = input[index] ;
             mult2 = input[index+1];


             mult1 = mult1*mult1; //Faz primeiro numero ao quadrado
             mult2 = mult2 * mult2; //Faz primeiro numero ao quadrado

             output[index/2] = sqrt(mult1 + mult2); //tira a raiz de mult1² + mult2²

         }

 }

 void ajusteValores(float* input, ushort size, float ganho){ //Multiplica cada um dos valores da FFT por 2*ganho ( ajuste de amplitude)
     ushort index;

     for(index = 0; index < size; index++){
         input[index] = 2*ganho*input[index];
     }

 }

Int16 thdCalculo( )  //MUDAR NOME!
{




    ushort index= 0; //Variável de índice
    ushort j = 0; //Variável de índice

    //********************************** Valores de entrada do MATLAB//
    double ganho = 1; //Será usado em "ajusteValores"
                     //Amostras normalizadas em [-1, 0.9999]; Limitação do DSP



    //estes dados abaixo deram 0.4% (correto)
    // SINAL: 0.03*cos(2 * pi * fg * Ts  *n); -> A DISTORÇÃO DEVE SER BEM PRÓXIMA DE 0%
    float dados[512]={ 0.03 , 0.01 , -0.02 , -0.03 , 0.01 , 0.03 , 0.01 , -0.02 , -0.03 , 0.00 , 0.03 , 0.02 , -0.02 , -0.03 , -0.00 , 0.03 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.03 , -0.00 , -0.03 , -0.02 , 0.02 , 0.03 , -0.00 , -0.03 , -0.02 , 0.02 , 0.03 , 0.00 , -0.03 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.03 , 0.00 , 0.03 , 0.02 , -0.02 , -0.03 , -0.00 , 0.03 , 0.02 , -0.01 , -0.03 , -0.01 , 0.03 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.03 , -0.01 , -0.03 , -0.01 , 0.02 , 0.03 , -0.00 , -0.03 , -0.02 , 0.02 , 0.03 , 0.00 , -0.03 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.03 , 0.00 , 0.03 , 0.02 , -0.02 , -0.03 , -0.00 , 0.03 , 0.02 , -0.02 , -0.03 , -0.00 , 0.03 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.03 , -0.00 , -0.03 , -0.02 , 0.02 , 0.03 , 0.00 , -0.03 , -0.02 , 0.01 , 0.03 , 0.01 , -0.03 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.03 , 0.01 , 0.03 , 0.01 , -0.02 , -0.03 , 0.00 , 0.03 , 0.02 , -0.02 , -0.03 , -0.00 , 0.03 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.03 , -0.00 , -0.03 , -0.02 , 0.02 , 0.03 , -0.00 , -0.03 , -0.02 , 0.02 , 0.03 , 0.00 , -0.03 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.03 , 0.00 , 0.03 , 0.02 , -0.02 , -0.03 , -0.00 , 0.03 , 0.02 , -0.01 , -0.03 , -0.01 , 0.03 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.03 , -0.01 , -0.03
                       , -0.01 , 0.02 , 0.03 , -0.00 , -0.03 , -0.02 , 0.02 , 0.03 , 0.00 , -0.03 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.03 , 0.00 , 0.03 , 0.02 , -0.02 , -0.03 , 0.00 , 0.03 , 0.02 , -0.02 , -0.03 , -0.00 ,
                       0.03 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.02 , -0.01 , -0.03 , -0.01
                       , 0.02 , 0.03 , -0.00 , -0.03 , -0.02 , 0.02 , 0.03 , 0.00 , -0.03 , -0.02 , 0.01 , 0.03 , 0.01 , -0.03 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.03 , 0.01 , 0.03 , 0.01 ,
                       -0.02 , -0.03 , 0.00 , 0.03 , 0.02 , -0.02 , -0.03 , -0.00 , 0.03 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.03 , -0.00 , -0.03 , -0.02
                       , 0.02 , 0.03 , 0.00 , -0.03 , -0.02 , 0.02 , 0.03 , 0.00 , -0.03 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.03 , 0.00 , 0.03 , 0.02 , -0.02 , -0.03 , -0.00 , 0.03 , 0.02 , -0.01 , -0.03 , -0.01 , 0.03 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.03 , -0.01 , -0.03 , -0.01 , 0.02 , 0.03 , -0.00 , -0.03 , -0.02 , 0.02 , 0.03 , 0.00 , -0.03 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.03 , 0.00 , 0.03 , 0.02 , -0.02 , -0.03 , -0.00 , 0.03 , 0.02 , -0.02 , -0.03 , -0.00 , 0.03 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.03 , -0.00 , -0.03 , -0.02 , 0.02 , 0.03 , 0.00 , -0.03 , -0.02 , 0.01 , 0.03 , 0.01 , -0.03 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.03 , 0.01 , 0.03 , 0.01 , -0.02 , -0.03 , 0.00 , 0.03 , 0.02 , -0.02 , -0.03 , -0.00 , 0.03 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.03 , -0.00 , -0.03 , -0.02 , 0.02 , 0.03 , -0.00 , -0.03 , -0.02 , 0.02 , 0.03 , 0.00 , -0.03 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.03 , 0.00 , 0.03 , 0.02 , -0.02 , -0.03 , -0.00 , 0.03 , 0.02 , -0.01 , -0.03 , -0.01 , 0.03 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.03 , -0.01 , -0.03 , -0.01 , 0.02 , 0.03 , -0.00 , -0.03 , -0.02 , 0.02 , 0.03 , 0.00 , -0.03 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.02 , 0.01 , 0.03 , 0.01 , -0.02 , -0.03 , 0.00 , 0.03 , 0.02 , -0.02 , -0.03 , -0.00 , 0.03 , 0.02 , -0.02 , -0.03 , -0.00 , 0.03 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.02 , -0.01 , -0.03 , -0.01 , 0.02 , 0.03 , -0.00 , -0.03 , -0.02 , 0.02 , 0.03 , 0.00 , -0.03 , -0.02 , 0.01 , 0.03 , 0.01 , -0.03 , -0.02 , 0.01};

    //SINAL: 0.03*cos(2 * pi * fg * Ts  *n)+ 0.03*cos(2*pi * 2 * fg * Ts  *n); -> A DISTORÇÃO DEVE SER BEM PRÓXIMA DE 100%
   // float dados[512]={ 0.06 , -0.01 , -0.02 , -0.01 , -0.02 , 0.06 , -0.00 , -0.02 , -0.01 , -0.03 , 0.05 , 0.01 , -0.03 , -0.00 , -0.03 , 0.05 , 0.02 , -0.03 , -0.00 , -0.03 , 0.04 , 0.03 , -0.03 , -0.00 , -0.03 , 0.03 , 0.04 , -0.03 , -0.00 , -0.03 , 0.01 , 0.05 , -0.03 , -0.01 , -0.02 , 0.00 , 0.06 , -0.02 , -0.01 , -0.02 , -0.01 , 0.06 , -0.02 , -0.02 , -0.01 , -0.02 , 0.06 , -0.01 , -0.02 , -0.01 , -0.03 , 0.06 , 0.01 , -0.03 , -0.00 , -0.03 , 0.05 , 0.02 , -0.03 , -0.00 , -0.03 , 0.04 , 0.03 , -0.03 , 0.00 , -0.03 , 0.03 , 0.04 , -0.03 , -0.00 , -0.03 , 0.02 , 0.05 , -0.03 , -0.00 , -0.03 , 0.01 , 0.06 , -0.03 , -0.01 , -0.02 , -0.01 , 0.06 , -0.02 , -0.01 , -0.02 , -0.02 , 0.06 , -0.01 , -0.02 , -0.01 , -0.02 , 0.06 , 0.00 , -0.02 , -0.01 , -0.03 , 0.05 , 0.01 , -0.03 , -0.00 , -0.03 , 0.04 , 0.03 , -0.03 , -0.00 , -0.03 , 0.03 , 0.04 , -0.03 , -0.00 , -0.03 , 0.02 , 0.05 , -0.03 , -0.00 , -0.03 , 0.01 , 0.05 , -0.03 , -0.01 , -0.02 , -0.00 , 0.06 , -0.02 , -0.01 , -0.02 , -0.01 , 0.06 , -0.01 , -0.02 , -0.01 , -0.02 , 0.06 , -0.00 , -0.02 , -0.01 , -0.03 , 0.05 , 0.01 , -0.03 , -0.00 , -0.03 , 0.05 , 0.02 , -0.03 , -0.00 , -0.03 , 0.04 , 0.03 , -0.03 , -0.00 , -0.03 , 0.03 , 0.04 , -0.03 , -0.00 , -0.03 , 0.01 , 0.05 , -0.03 , -0.01 , -0.02 , 0.00 , 0.06 , -0.02 , -0.01 , -0.02 , -0.01 , 0.06 , -0.02 , -0.02 , -0.01 , -0.02 , 0.06 , -0.01 , -0.02 , -0.01 , -0.03 , 0.06 , 0.01 , -0.03 , -0.00 , -0.03 , 0.05 , 0.02 , -0.03 , -0.00 , -0.03 , 0.04 , 0.03 , -0.03 , 0.00 , -0.03 , 0.03 , 0.04 , -0.03 , -0.00 , -0.03 , 0.02 , 0.05 , -0.03 , -0.00 , -0.03 , 0.01 , 0.06 , -0.03 , -0.01 , -0.02 , -0.01 , 0.06 , -0.02 , -0.01 , -0.02 , -0.02 , 0.06 , -0.01 , -0.02 , -0.01 , -0.02 , 0.06 , 0.00 , -0.02 , -0.01 , -0.03 , 0.05 , 0.01 , -0.03 , -0.00 , -0.03 , 0.04 , 0.03 , -0.03 , -0.00 , -0.03 , 0.03 , 0.04 , -0.03 , -0.00 , -0.03 , 0.02 , 0.05 , -0.03 , -0.00 , -0.03 , 0.01 , 0.05 , -0.03 , -0.01 , -0.02 , -0.00 , 0.06 , -0.02 , -0.01 , -0.02 , -0.01 , 0.06 , -0.01 , -0.02 , -0.01 , -0.02 , 0.06 , -0.00 , -0.02 , -0.01 , -0.03 , 0.05 , 0.01 , -0.03 , -0.00 , -0.03 , 0.05 , 0.02 , -0.03 , -0.00 , -0.03 , 0.04 , 0.03 , -0.03 , -0.00 , -0.03 , 0.03 , 0.04 , -0.03 , -0.00 , -0.03 , 0.01 , 0.05 , -0.03 , -0.01 , -0.02 , 0.00 , 0.06 , -0.02 , -0.01 , -0.02 , -0.01 , 0.06 , -0.02 , -0.02 , -0.01 , -0.02 , 0.06 , -0.01 , -0.02 , -0.01 , -0.03 , 0.06 , 0.01 , -0.03 , -0.00 , -0.03 , 0.05 , 0.02 , -0.03 , -0.00 , -0.03 , 0.04 , 0.03 , -0.03 , 0.00 , -0.03 , 0.03 , 0.04 , -0.03 , -0.00 , -0.03 , 0.02 , 0.05 , -0.03 , -0.00 , -0.03 , 0.01 , 0.06 , -0.03 , -0.01 , -0.02 , -0.01 , 0.06 , -0.02 , -0.01 , -0.02 , -0.02 , 0.06 , -0.01 , -0.02 , -0.01 , -0.02 , 0.06 , 0.00 , -0.02 , -0.01 , -0.03 , 0.05 , 0.01 , -0.03 , -0.00 , -0.03 , 0.04 , 0.03 , -0.03 , -0.00 , -0.03 , 0.03 , 0.04 , -0.03 , -0.00 , -0.03 , 0.02 , 0.05 , -0.03 , -0.00 , -0.03 , 0.01 , 0.05 , -0.03 , -0.01 , -0.02 , -0.00 , 0.06 , -0.02 , -0.01 , -0.02 , -0.01 , 0.06 , -0.01 , -0.02 , -0.01 , -0.02 , 0.06 , -0.00 , -0.02 , -0.01 , -0.03 , 0.05 , 0.01 , -0.03 , -0.00 , -0.03 , 0.05 , 0.02 , -0.03 , -0.00 , -0.03 , 0.04 , 0.03 , -0.03 , -0.00 , -0.03 , 0.03 , 0.04 , -0.03 , -0.00 , -0.03 , 0.01 , 0.05 , -0.03 , -0.01 , -0.02 , 0.00 , 0.06 , -0.02 , -0.01 , -0.02 , -0.01 , 0.06 , -0.02 , -0.02 , -0.01 , -0.02 , 0.06 , -0.01 , -0.02 , -0.01 , -0.03 , 0.06 , 0.01 , -0.03 , -0.00 , -0.03 , 0.05 , 0.02 , -0.03 , -0.00 , -0.03 , 0.04 , 0.03 , -0.03 , 0.00 , -0.03 , 0.03 , 0.04 , -0.03 , -0.00 , -0.03 , 0.02 , 0.05 , -0.03 , -0.00 , -0.03 , 0.01 , 0.06 , -0.03 , -0.01 , -0.02 , -0.01 , 0.06 , -0.02 , -0.01 , -0.02 , -0.02 , 0.06 , -0.01 , -0.02 , -0.01 , -0.02 , 0.06 , 0.00 , -0.02 , -0.01 , -0.03 , 0.05 , 0.01 , -0.03 , -0.00 , -0.03 , 0.04 , 0.03 , -0.03 , -0.00 , -0.03 , 0.03 , 0.04 , -0.03 , -0.00 , -0.03 , 0.02
     //                  , 0.05 , -0.03 , -0.00 , -0.03 , 0.01 , 0.05 , -0.03 , -0.01 , -0.02 , -0.00 , 0.06 , -0.02 , -0.01 , -0.02 , -0.01};



















    DATA amostras_int[L]={0}; // Vetor para converter amostras de float para Q.15
                // L/2
    DATA ab_val[L/2]={0};  //Vetor de valores absolutos (Real + imaginário da FFT); tem metade do tamanho do vetor-resultante
                        //da FFT


    float abs_floatFFT [L/2] = {0}; //Armazenará os valores em float após calculo da FFT


    fltoq15(dados, amostras_int, L); //Converte float em Q.15


    rfft(amostras_int,L,SCALE); //calcula FFT real
    absol(amostras_int, ab_val , L); // Converte os valores complexos da FFT em reais absolutos
    q15tofl(ab_val, abs_floatFFT, L/2 );  //Transforma de Q.15 para float
    ajusteValores(abs_floatFFT, L/2, ganho); //Multiplica os índices absolutos da FFT por 2*ganho (ajuste)

    //***************************** Agora podemos calcular o THD ************************//


    float bin_freq = fs/L; //Resolução da DFT (Hertz/Amostra)


    ushort numero_harmonicas = fs/(2*fg);  //Divide-se o espectro por 2 e pela frequencia-base;



    float freq_harmonicas[fs/(2*fg)];    //Armazena todas as harmonicas existentes, mas só serão usadas 'consideradas' harmônicas de fato (ver valor em #DEFINE consideradas)

    for (index = 0; index < numero_harmonicas; index++){ //vetor com as frequencias harmonicas
        freq_harmonicas[index] = (index + 1) * fg;
    }

    //Calculo dos bin_harmonicos

    double bin_harmonicos[considerados] = {0}; //usar double para arredondamento posterior
    double calculo=0;


    for(index=0; index < numero_harmonicas; index++){
            //bin_harmonicos[index] = freq_harmonicas/bin_freq;
        calculo = freq_harmonicas[index]/bin_freq;
        modf(calculo , &bin_harmonicos[index]); //calcula arredondamento de freq_harmonicas/bin_freq
    }

    //ushort integ_resol = 2 ; //Resolução de integração
    //ushort considerados = 3; //Deve-se alterar este valor para um termo menor (5, 6 por exemplo)

    // considerados ; considerados
    double soma_harmonicos[considerados] = {0};       //DEVE SER UM VETOR (ARMAZENA AS SOMAS DE CADA HARMONICO)
    double square_harmonicos[considerados] = {0};     //DEVE SER UM VETOR (ARMAZENA AS RAIZES DE CADA HARMONICO)
    double numerador = 0;                             //Armazena a soma dos quadrados das intensidades (V2² + V3² + ...)

    //Integrar todos os harmônicos (somar pontos ao redor do valor teórico)
    for(index = 0; index < considerados; index++){

        //Soma todos os itens de um dado harmônico -> soma de todos = V
        for(j=(bin_harmonicos[index]-integ_resol); j < (bin_harmonicos[index]+integ_resol); j++){
            soma_harmonicos[index] = soma_harmonicos[index] + abs_floatFFT[j];
        }
        //Calcula V²
        square_harmonicos[index] = pow(soma_harmonicos[index],2);

        numerador = numerador + square_harmonicos[index];
    }
    numerador = numerador - square_harmonicos[0]; //Remove a contribuição do primeiro harmônico


    numerador = sqrt(numerador); //Armazena a raiz quadrada de numerador nele mesmo

    double distorcao = numerador / soma_harmonicos[0];  //A distorção é dada por "(sqrt (V2²+V3²+...) )/V1"
    double dist_percent = distorcao * 100;

    return 0;
}
