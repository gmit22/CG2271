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
#include "myUltrasonic.h"

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
uint32_t end = 0x0000;
uint32_t distance = 0x00;

osMessageQueueId_t ultrasonicMessage;

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
	//playConnectSong(); 
}

void TPM0_IRQHandler(void) // what is the interrupt logic
{
	NVIC_ClearPendingIRQ(TPM0_IRQn); //clear queue before begin;
		
	if(TPM0_C3SC & TPM_CnSC_CHF_MASK){  // If CHF = channel is 1 when an event occur- cleared by writing 1 
		TPM0_C3SC |= TPM_CnSC_CHF_MASK;
		if(!start){
			/*Clear the TPM0 counter register to start with ~0 */
			TPM0_CNT = 0;
			startRecord = TPM0_CNT; 
			start = 1;
		} else {
			end = TPM0_C3V + counter * 3750; //tpm0_c3V is counter value
			//counter = 0;
			ready = 1;
			//value = (end * 2.6666 * 0.01715 * 1.5) - 337 ;  // determine speed of ultrasonic from frequency and speedy of sound = 34300cm/s /2 =17150;
			//osMessageQueuePut(ultrasonicMessage, &end, NULL, 0);
			//osDelay(2000);
			//SIM->SCGC6 &= ~SIM_SCGC6_TPM0_MASK;
		}
	}
	
	if(TPM0_SC & TPM_SC_TOF_MASK) { //checking for overflow so that it resets when hit and counter = 1 cycle.
		TPM0_SC |= TPM_SC_TOF_MASK;
		counter++; 
	}
 //osDelay (100);
	
	//PORTD->ISFR |= 0xFFFFF;
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
 * Application tSelfDriveThread
 *---------------------------------------------------------------------------*/
void tSelfDriveThread(void *argument) {
	initUltrasonic();
	for (;;) {
		//initUltrasonic();
			SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
		osSemaphoreAcquire(selfDriveSem, osWaitForever); 
			while(1){
				SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

				//value =0;
				//counter =0;
				//distance =0;
			osSemaphoreRelease(triggerSem);
			//osMessageQueueGet(ultrasonicMessage, &distance, NULL, osWaitForever);
					while(!ready) {}
//						if ((end - startRecord) > 0) {
//							value = end - startRecord;
//						} else if ((startRecord - end) > 0)  {
//							value = end + 7500 - startRecord;
//						}
					distance = end - startRecord;
					start = 0;
					value = (distance* 2.6666 * 0.01715);// * 1.5) - 337 ;  // determine speed of ultrasonic from frequency and speedy of sound = 34300cm/s /2 =17150;
					counter = 0x0000;
						ready = 0;
					SIM->SCGC6 &= ~SIM_SCGC6_TPM0_MASK;
						//end=0;
						//value=0;
					//osDelay(100);
						//forward();
						//delay(0xFFFFF);
						//stop();
			}
			
		//osDelay(1000);
		//osSemaphoreRelease(selfDriveSem);
		//osSemaphoreRelease(triggerSem);
	}
}

/*----------------------------------------------------------------------------
 * Application tTriggerThread
 *---------------------------------------------------------------------------*/
void tTriggerThread(void *argument) {
	for (;;) {
		osSemaphoreAcquire(triggerSem, osWaitForever);
		PTD->PDOR |= MASK(PTD2_Pin);
		osDelay(10); //10us orginal = 0x11
	
		//Off trigger after 10 seconds
		PTD->PDOR &= ~MASK(PTD2_Pin);
		//delay(0x18e70);
		osDelay(10);
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
				//uturn();
				//state = FORWARD;
				break;
			case SOUTH:
				reverse();
				//state = REVERSE;
				break;
			case EAST:
				turnRight();
				//state = RIGHT;
				break;
			case WEST:
				turnLeft();
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
				uturn();
				//leftForward();
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
			case SELF_DRIVE:
				osSemaphoreRelease(selfDriveSem);
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
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	
	//Initialize Semaphores
	brainSem = osSemaphoreNew(1, 0, NULL);
	moveSem = osSemaphoreNew(1, 0, NULL);
	soundSem = osSemaphoreNew(1, 1, NULL);
	selfDriveSem = osSemaphoreNew(1, 0, NULL);
	triggerSem = osSemaphoreNew(1, 0, NULL);
	
	ultrasonicMessage = osMessageQueueNew(1, sizeof(uint32_t), NULL);
	
	//Wair for bluetooth connectivity 
	while (userSignal != CONNECTION);
	bluetoothConnected();
	
	osThreadNew(tBrainThread, NULL, NULL);
	osThreadNew(tSelfDriveThread, NULL, NULL);
	osThreadNew(tTriggerThread, NULL, NULL);
	osThreadNew(tAudio, NULL, NULL);
  osThreadNew(tRearLED, NULL, NULL);
	osThreadNew(tFrontLED, NULL, NULL);
	osThreadNew(tMotorThread, NULL, NULL);

	
	
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
