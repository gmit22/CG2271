/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "myPWM.h"
#include "myLED.h"
#include "myUART.h"
#include "myBasic.h"
#include "myMotor.h"
#include "mySound.h"

#define NORTH 1
#define SOUTH 2
#define EAST 3
#define WEST 4

#define NORTH_EAST 5
#define NORTH_WEST 6
#define SOUTH_EAST 7
#define SOUTH_WEST 8

#define STOP 9
#define END 10
#define CONNECTION 11
#define SELF_DRIVE 12

//uint8_t rx_data;
char isMove = 0;

const osThreadAttr_t maxPriority = {
	.priority = osPriorityRealtime
};


const osThreadAttr_t aboveNormPriority = {
	.priority = osPriorityAboveNormal
};

const osThreadAttr_t highPriority = {
	.priority = osPriorityHigh
};



void bluetoothConnected() {
	flashGREEN_Twice();
	//flashRED_Moving();
	//delay(1000);
	userSignal = STOP;
	//Code to play a sound when bluetooth is connected to the Freedom Board.
	playConnectSong(); 
}

//Check and update flags if the device is in moving state
char isMoving() {
	
	if (userSignal == STOP || userSignal == END) {
		isMove = 0;
	} else if (userSignal == NORTH || userSignal == SOUTH || userSignal == EAST || userSignal == WEST 
					|| userSignal == NORTH_EAST || userSignal == NORTH_WEST || userSignal == SOUTH_EAST || userSignal == SOUTH_WEST) {
					isMove = 1;
	}
	return isMove;	
}

/*----------------------------------------------------------------------------
 * Application tAudio 
 *---------------------------------------------------------------------------*/
void tAudio(void *arguement) {
	
	for (;;) {
		playRaceSong();
	}	
}

/*----------------------------------------------------------------------------
 * Application rear RED LEDs
 *---------------------------------------------------------------------------*/
void tRearLED(void *arguement) {
	
	for (;;) {
		if (isMoving()) {
			flashRED_Moving();
		} else {
			flashRED_Stationery();
		}
	}	
}

/*----------------------------------------------------------------------------
 * Application front GREEN LEDs
 *---------------------------------------------------------------------------*/
void tFrontLED(void *arguement) {
	
	int ledIndex = 0;
	
	for (;;) {
		if (isMoving()) {
			ledIndex = (ledIndex + 1)%8;
			runningGREEN_Moving(ledIndex);
		} else {
			solidGREEN_Stationery();
		}
	}
}

/*----------------------------------------------------------------------------
 * Application tMotorThread
 *---------------------------------------------------------------------------*/
void tMotorThread (void *argument) {
	for(;;) {
		osSemaphoreAcquire(moveSem, osWaitForever);
		switch(userSignal) {
			case NORTH:
				forward();
				//state = FORWARD;
				break;
			case SOUTH:
				reverse();
				//state = REVERSE;
				break;
			case EAST:
				turnLeft();
				//state = RIGHT;
				break;
			case WEST:
				turnRight();
				//state = LEFT;
				break;
			case NORTH_EAST:
				rightForward();
				//state = RIGHTFW;
				break;
			case SOUTH_EAST:
				rightReverse();
				//state = RIGHTRV;
				break;
			case SOUTH_WEST:
				leftReverse();
				//state = LEFTRV;
				break;
			case NORTH_WEST:
				leftForward();
				//state = LEFTFW;
				break;
			default:
				stop();
				userSignal = STOP;
				//state = HALT;
		}
	}
}

/*----------------------------------------------------------------------------
 * Application tBrainThread
 *---------------------------------------------------------------------------*/
void tBrainThread (void *argument) {
	for (;;) {
		osSemaphoreAcquire(brainSem, osWaitForever);
		
		switch(userSignal) {
			case END:
				playEndSong();
			break;
			
			case NORTH:
			case SOUTH:
			case EAST:
			case WEST:
			case NORTH_EAST:
			case SOUTH_EAST:
			case SOUTH_WEST:
			case NORTH_WEST:
			case STOP:
				osSemaphoreRelease(moveSem);
				break;

			default:
				//stop();
				break;
		}
	}
}


int main (void) {
 
  //System Initialization 
	

	SystemCoreClockUpdate();
	setupUART2(BAUD_RATE);
	initLED();
	initPWM();  //Can please check if this PWM is for the motors or the sound?
	offLEDModules();
	//forward();
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	
	//Initialize Semaphores
	brainSem = osSemaphoreNew(1, 0, NULL);
	moveSem = osSemaphoreNew(1, 0, NULL);
	soundSem = osSemaphoreNew(1, 1, NULL);
	
	//Wair for bluetooth connectivity 
	while (userSignal != CONNECTION);
	bluetoothConnected();
	
	osThreadNew(tBrainThread, NULL, &maxPriority);
	osThreadNew(tAudio, NULL, NULL);
  osThreadNew(tRearLED, NULL, NULL);
	osThreadNew(tFrontLED, NULL, NULL);
	osThreadNew(tMotorThread, NULL, &aboveNormPriority);
	
	
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
