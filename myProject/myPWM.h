#ifndef myPWM_H
#define myPWM_H

#include "MKL25Z4.h"

#define PTB0_Pin 0	//TPM1_CH0 left reverse
#define PTB1_Pin 1	//TPM1_CH1 left forward
#define PTB2_Pin 2	//TPM2_CH0 right reverse
#define PTB3_Pin 3	//TPM2_CH1 right forward

#define PTD0_Pin 0
#define PTD1_Pin 1

void initPWM(void);

#endif
