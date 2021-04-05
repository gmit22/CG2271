#include "MKL25Z4.h"                    // Device header
#include "myBasic.h"
#include "myMotor.h"

#define ACTIVE 0x1D4C //7500

void stop() {
	TPM1->MOD = 0;	//left stop
	TPM1_C0V = 0x0;
	TPM1_C1V = 0x0;
	
	TPM2->MOD = 0;	//right stop
	TPM2_C0V = 0;
	TPM2_C1V = 0;
}

void forward() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C1V = ACTIVE; //TPM1_CH1 left forward
	TPM2_C1V = ACTIVE; //TPM2_CH1 right forward
}

void reverse() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C0V = ACTIVE; //TPM1_CH0 left reverse
	TPM2_C0V = ACTIVE; //TPM2_CH0 right reverse
}

void turnLeft() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C0V = ACTIVE/2; //TPM1_CH0 left reverse
	TPM2_C1V = ACTIVE/2; //TPM2_CH1 right forward
}

void turnRight() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C1V = ACTIVE/2; //TPM1_CH1 left forward
	TPM2_C0V = ACTIVE/2; //TPM2_CH0 right reverse
}

void rightForward() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C1V = ACTIVE; //TPM1_CH1 left forward
	TPM2_C1V = ACTIVE/8; //TPM2_CH1 right forward slower
}

void rightReverse() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C0V = ACTIVE; //TPM1_CH0 left reverse
	TPM2_C0V = ACTIVE/8; //TPM2_CH0 right reverse
}

void leftReverse() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C0V = ACTIVE/8; //TPM1_CH0 left reverse
	TPM2_C0V = ACTIVE; //TPM2_CH0 right reverse
}

void leftForward() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C1V = ACTIVE/8; //TPM1_CH1 left forward slower
	TPM2_C1V = ACTIVE; //TPM2_CH1 right forward
}
