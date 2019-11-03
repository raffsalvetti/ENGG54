/*
* flangerTest.c
*
*  Created on: Oct 27, 2012
*      Author: BLEE
*
*  Description: This is the test program for floating-point flangering effect experiment
*
*  For the book "Real Time Digital Signal Processing:
*                Fundamentals, Implementation and Application, 3rd Ed"
*                By Sen M. Kuo, Bob H. Lee, and Wenshun Tian
*                Publisher: John Wiley and Sons, Ltd
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tistdtypes.h"


#define MAX_BUF_SIZE 1024
#define PI 3.14159265


Int16  d_buffer[MAX_BUF_SIZE];
Int8   temp[2*MAX_BUF_SIZE];
Int8   senoid[MAX_BUF_SIZE];
Uint8  waveHeader[44];

void wHeader(Uint8 *w, float f1, float f2, Uint32 bytes);

int Fc = 440;
int Fs = 8000;



void main()
{
    FILE   *fpIn,*fpOut;
    float yOut;
    float carrier = 0 ;
    int i,j,c;
    c = 2;
    long int count = 0;
    //printf("Lendo wave File\n");

    fpIn = fopen("..\\data\\Soxphone8kHz.wav", "rb");
    fpOut = fopen("..\\data\\audioOut.wav", "wb");

    if (fpIn == NULL)
    {
        printf( "Problema ao reproduzio o audio\n" );
        exit(0);
    }
    //Lê os heades do wave
    if (c == 2)
    {
      fread(waveHeader, sizeof(Int8), 44, fpIn);        // Advance input to speech data
      fwrite(waveHeader, sizeof(Int8), 44, fpOut);      // Write header for output file
    }
    //Faz a modulação
    //carrier =  sin(2*pi*indice*(Fc/Fs));
    while ((fread(&temp, sizeof(Int8),2*MAX_BUF_SIZE, fpIn)) == 2*MAX_BUF_SIZE )
    {
        //calcula o carrier
        for(j = 0;j< MAX_BUF_SIZE;j++)
        {
            carrier = 0.2*sin(2*PI*count*(Fc/((float)Fs)));
            temp[j] = temp[j] * carrier;
            count++;


        }

        fwrite(&temp, sizeof(Int8), 2*MAX_BUF_SIZE, fpOut);
        //printf("%ld data samples processed\n", count);
    }


    printf("Ring efecect\n");
    //gera a seinoide:
    //for(i=0; i< MAX_BUF_SIZE,)


     if (c == 2)        // based on input WAVE file to generate output
    {
      wHeader(waveHeader, 8000, 8000, count<<1);
      rewind(fpOut);
      fwrite(waveHeader, sizeof(Int8), 44, fpOut);
    }

    fclose(fpOut);
    fclose(fpIn);
    printf("Exp --- completed\n");


}





// Create the wave file header
void wHeader(Uint8 *w, float f1, float f2, Uint32 bytes)
{
    Int32 t;

    t = bytes;
    w[40] = (t>>0)&0xff;    // Set up output file size
    w[41] = (t>>8)&0xff;
    w[42] = (t>>16)&0xff;
    w[43] = (t>>24)&0xff;
    t += 36;
    w[4] = (t>>0)&0xff;
    w[5] = (t>>8)&0xff;
    w[6] = (t>>16)&0xff;
    w[7] = (t>>24)&0xff;
    t = w[24]|(w[25]<<8)|((Int32)w[26]<<16)|((Int32)w[27]<<24);
    t = (Int32)((float)t*f2/f1);
    w[24] = (t>>0)&0xff;    // Set up output file frequency
    w[25] = (t>>8)&0xff;
    w[26] = (t>>16)&0xff;
    w[27] = (t>>24)&0xff;
    t = w[28]|(w[29]<<8)|((Int32)w[30]<<16)|((Int32)w[31]<<24);
    t = (Int32)((float)t*f2/f1);
    w[28] = (t>>0)&0xff;    // Set up output file byte rate
    w[29] = (t>>8)&0xff;
    w[30] = (t>>16)&0xff;
    w[31] = (t>>24)&0xff;
    return;
}



