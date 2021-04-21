#include "MKL25Z4.h"                    // Device header
#include "myBasic.h"
#include "myMotor.h"

#define ACTIVE 0x1D4C //7500
#define LESS_ACTIVE 0x1388 //5000
#define LESS_LESS_ACTIVE 0xFA0 //4000 

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
	
	TPM1_C1V = LESS_ACTIVE; //TPM1_CH1 left forward
	TPM2_C1V = LESS_ACTIVE; //TPM2_CH1 right forward
}

void reverse() {
	
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C1V = 0; 
	TPM2_C1V = 0;
	
	TPM1_C0V = LESS_ACTIVE; //TPM1_CH0 left reverse
	TPM2_C0V = LESS_ACTIVE; //TPM2_CH0 right reverse
}

//Restore to /2 later?
void turnLeft() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM2_C0V = 0;
	TPM1_C1V = 0;
	
	TPM2_C1V = LESS_LESS_ACTIVE; //TPM2_CH0 right reverse
	TPM1_C0V = LESS_LESS_ACTIVE; //TPM1_CH1 left forward
}

void turnRight() {
	
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM2_C1V = 0;
	TPM1_C0V = 0;
	
	TPM1_C1V = LESS_LESS_ACTIVE; //TPM2_CH1 right forward
	TPM2_C0V = LESS_LESS_ACTIVE; //TPM1_CH0 left reverse
	
	
}


// Remvove for TPM1 and /8 for TPM2
void rightForward() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM2_C0V = 0;	
	TPM1_C0V = 0; //TPM1_CH1 left forward
	
	TPM1_C1V = ACTIVE;
	TPM2_C1V = ACTIVE/6; //TPM2_CH1 right forward slower
}
// Remvove for TPM1 and /8 for TPM2
void rightReverse() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C1V = 0;
	TPM2_C1V = 0;
	
	TPM1_C0V = ACTIVE; //TPM1_CH0 left reverse
	TPM2_C0V = ACTIVE/6; //TPM2_CH0 right reverse
}
// TPM1 /8 TPM2 no divide
void leftReverse() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C1V = 0;
	TPM2_C1V = 0;
	
	TPM1_C0V = ACTIVE/6; //TPM1_CH0 left reverse
	TPM2_C0V = ACTIVE; //TPM2_CH0 right reverse
}
// TPM1 /8 TPM2 no divide
void leftForward() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C0V = 0;
	TPM2_C0V = 0;
	
	TPM1_C1V = ACTIVE/6; //TPM1_CH1 left forward slower
	TPM2_C1V = ACTIVE; //TPM2_CH1 right forward
}

void selfLeft() {
	turnLeft();
	delay(0xB1008);
	stop();
	delay(0xFFFFF);
}

void selfRight() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM2_C0V = 0;	
	TPM1_C0V = 0; //TPM1_CH1 left forward
	
	TPM1_C1V = ACTIVE;
	TPM2_C1V = 1250; //TPM2_CH1 right forward slower
}

void selfForward() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C0V = 0; 
	TPM2_C0V = 0;
	
	TPM1_C1V = LESS_ACTIVE; //TPM1_CH1 left forward
	TPM2_C1V = LESS_ACTIVE; //TPM2_CH1 right forward
}

void shortForward() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C0V = 0; 
	TPM2_C0V = 0;
	
	TPM1_C1V = 2700;//LESS_LESS_ACTIVE/2; //TPM1_CH1 left forward
	TPM2_C1V = 2700;//LESS_LESS_ACTIVE/2; //TPM2_CH1 right forward
	
}
void comingBack() {
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	TPM1_C0V = 0; 
	TPM2_C0V = 0;
	
	TPM1_C1V = LESS_LESS_ACTIVE/2; //TPM1_CH1 left forward
	TPM2_C1V = LESS_LESS_ACTIVE/2; //TPM2_CH1 right forward
	
}

void longForward() {
	forward();
	delay(0x166660);
	stop();
	delay(0xFFFFF);
}

void uturn() {
	
	stop();
	osDelay(700);
	turnLeft();
	osDelay(250);
	stop();
	osDelay(2000);
	forward();
	osDelay(400); //300 cmi
	stop();
	osDelay(2000);
	selfRight();
	osDelay(700);
	stop();
	osDelay(2000);
	selfRight();
	osDelay(700);
	stop();
	osDelay(2000);
	selfRight();
	osDelay(700);
	stop();
	osDelay(2000);
	selfRight(); //may need change
	osDelay(700);
	stop();
	osDelay(1000);
	selfForward(); 
	osDelay(450); // change from 650
  //stop();
	//osDelay(2000);
	turnLeft();
	osDelay(400); //was 400 need more left //lab 500 90degree
	stop();
	

//	selfLeft();
//	shortForward();
//	selfRight();
//	shortForward();
//	selfRight();
//	longForward();
//	selfRight();
//	shortForward();
//	selfRight();
//	shortForward();
//	selfLeft();
}
