#include "MKL25Z4.h"                    // Device header
#include "myBasic.h"
#include "myMotor.h"

#define ACTIVE 0x1D4C //7500
#define LESS_ACTIVE 0x1388 //5000

void stop() {
	TPM1->MOD = 0;	//left stop
	TPM1_C0V = 0x0;
	TPM1_C1V = 0x0;
	
	TPM2->MOD = 0;	//right stop
	TPM2_C0V = 0;
	TPM2_C1V = 0;
}

// TPM2 -> controls the right side of the motors
// TPM1 -> controls the left side of the motors

// C0V To move both side of motors to move reverse (-ve)
// C1V To move both side of motors to move forward (+ve)

//forward() reevrse() divided by 4 for lowering speed for testing
void forward() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C0V = 0; 
	TPM2_C0V = 0;
	
	TPM1_C1V = ACTIVE/2; //TPM1_CH1 left forward
	TPM2_C1V = ACTIVE/2; //TPM2_CH1 right forward
}

void reverse() {
	
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C1V = 0; 
	TPM2_C1V = 0;
	
	TPM1_C0V = ACTIVE; //TPM1_CH0 left reverse
	TPM2_C0V = ACTIVE; //TPM2_CH0 right reverse
}

//Restore to /2 later?
void turnRight() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM2_C0V = 0;
	TPM1_C1V = 0;
	
	TPM2_C1V = LESS_ACTIVE; //TPM2_CH0 right reverse
	TPM1_C0V = LESS_ACTIVE; //TPM1_CH1 left forward
}

void turnLeft() {
	
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM2_C1V = 0;
	TPM1_C0V = 0;
	
	TPM1_C1V = LESS_ACTIVE; //TPM2_CH1 right forward
	TPM2_C0V = LESS_ACTIVE; //TPM1_CH0 left reverse
	
	
}


// Remvove for TPM1 and /8 for TPM2
void rightForward() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM2_C0V = 0;	
	TPM1_C0V = 0; //TPM1_CH1 left forward
	
	TPM1_C1V = ACTIVE;
	TPM2_C1V = ACTIVE/10; //TPM2_CH1 right forward slower
}
// Remvove for TPM1 and /8 for TPM2
void rightReverse() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C1V = 0;
	TPM2_C1V = 0;
	
	TPM1_C0V = ACTIVE; //TPM1_CH0 left reverse
	TPM2_C0V = ACTIVE/10; //TPM2_CH0 right reverse
}
// TPM1 /8 TPM2 no divide
void leftReverse() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C1V = 0;
	TPM2_C1V = 0;
	
	TPM1_C0V = ACTIVE/10; //TPM1_CH0 left reverse
	TPM2_C0V = ACTIVE; //TPM2_CH0 right reverse
}
// TPM1 /8 TPM2 no divide
void leftForward() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C0V = 0;
	TPM2_C0V = 0;
	
	TPM1_C1V = ACTIVE/10; //TPM1_CH1 left forward slower
	TPM2_C1V = ACTIVE; //TPM2_CH1 right forward
}
