/*
* flanger.h
*
*  Created on: Oct 27, 2012
*      Author: BLEE
*
*  Description: This C header file for flanger experiment
*
*  For the book "Real Time Digital Signal Processing:
*                Fundamentals, Implementation and Application, 3rd Ed"
*                By Sen M. Kuo, Bob H. Lee, and Wenshun Tian
*                Publisher: John Wiley and Sons, Ltd
*
*/

#ifndef __FLANGER_H__
#define __FLANGER_H__
 
#define MAX_BUF_SIZE 1024
#define LFO_F 0.5

typedef struct {
	float A_maxSwing;
	float G_depth;
	float var_delay;
	Int16 delay;
	Int32 n;
} flanger;

void  flangerInit(float depth, flanger *t);
Int16 Flanger_process(Int16 xinp, flanger *t);
void  Flanger_sweep(flanger *t);

extern Int16 d_buffer[MAX_BUF_SIZE];
extern float var_delay;
#endif
