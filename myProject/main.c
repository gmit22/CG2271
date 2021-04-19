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
volatile uint32_t end = 0x0000;
uint32_t distance = 0x00;
volatile uint32_t gettingPITdistance = 0x00;
volatile uint32_t pitCounter = 0x00;
uint32_t pitValue = 0x00;
float pitDistance = 0;
int selfDriveFlag = 1;
int selfReturnFlag = 1;

int selfDriveFlagLED = 0;

volatile int ready;
volatile uint32_t start;
volatile uint32_t startRecord;



osMessageQueueId_t ultrasonicMessage;
osMessageQueueId_t ultrasonicDistance;


//osMutexId_t myMutex;

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

void PORTD_IRQHandler() {
		// Clear Pending IRQ
		NVIC_ClearPendingIRQ(PORTD_IRQn); // Not necessary, but for legacy reasons
		
		// Updating some variable / flag
	//if (PTD -> PDIR & MASK (PTD3_Pin)){
		if(!start){
				/*Clear the TPM0 counter register to start with ~0 */
				startRecord = PIT_CVAL0; 
				start = 1;
			} else {
				end = PIT_CVAL0;// + counter * 3750; //tpm0_c3V is counter value
				//counter = 0;
				ready = 1;
				if ((startRecord -end) > 0) {
					pitValue = startRecord-end + 0x3FFFFF * pitCounter;
				} else if ((end-startRecord) > 0)  {              
						pitValue = end + (0x3FFFFF- startRecord)+ 0x3FFFFF * pitCounter;
				}
				
				pitCounter =0;
				osMessageQueuePut(ultrasonicMessage, &pitValue, NULL, 0);

			}
		//}

							
				//value = (end * 2.6666 * 0.01715 * 1.5) - 337 ;  // determine speed of ultrasonic from frequency and speedy of sound = 34300cm/s /2 =17150;
				//osMessageQueuePut(ultrasonicMessage, &end, NULL, 0);
				//osDelay(2000);
				//SIM->SCGC6 &= ~SIM_SCGC6_TPM0_MASK;
		
		//Clear INT Flag
		PORTD->ISFR |= MASK(PTD3_Pin); // ISFR = Interrupt Status Flag Register. Important to clear it (1 to clear the flag to stop sending interrupt requests)
	}
void PIT_IRQHandler(){
  if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
    pitCounter++; 
    //Clear interrupt request flag for channel 
    PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK; 
  }
}  

//Check and update flags if the device is in moving state
char isMoving() {
	
	if ((userSignal == STOP && selfDriveFlagLED == 0) || userSignal == END) {
		isMove = 0;
	} else if (userSignal == NORTH || userSignal == SOUTH || userSignal == EAST || userSignal == WEST 
					|| userSignal == NORTH_EAST || userSignal == NORTH_WEST || userSignal == SOUTH_EAST || userSignal == SOUTH_WEST ||selfDriveFlagLED == 1) {
					isMove = 1;
	}
					
	return isMove;	
}

/*----------------------------------------------------------------------------
 * Application tAudio 
 *---------------------------------------------------------------------------*/
void tAudio(void *arguement) {
	
	for (;;) {
		if (endRaceSong)playRaceSong();
		else playEndSong();

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
			ledIndex = 0;
			solidGREEN_Stationery();
		}
	}
}

/*----------------------------------------------------------------------------
 * Application tUltrasonicThread
 *---------------------------------------------------------------------------*/
void tUltrasonicThread(void *argument) {
	for(;;) {
		osSemaphoreRelease(triggerSem);
					
					osMessageQueueGet(ultrasonicMessage, &distance, NULL, osWaitForever);
					while(!ready) {}
					gettingPITdistance = distance;
					start = 0;

	}
}

/*----------------------------------------------------------------------------
 * Application tSelfDriveThread
 *---------------------------------------------------------------------------*/
void tSelfDriveThread(void *argument) {
		
		for (;;) {
			osSemaphoreAcquire(selfDriveSem, osWaitForever);
			
				while(selfDriveFlag){
					pitDistance = (gettingPITdistance * 0.028333 * 0.01715)  + 4;
					forward();
					osDelay(200);
					while (pitDistance > 3 ) {
						pitDistance = (gettingPITdistance * 0.028333 * 0.01715)  + 4;
						shortForward();
						osDelay(50);
						if ((pitDistance < 45) && (pitDistance > 16) ) {
							stop();
							break;
						}
					}
					//shortForward();
					selfDriveFlag = 0;
					stop();
					osDelay(1500);
				
				uturn();
			  osDelay(1000);
				selfDriveFlagLED = 0;

			
				while(selfReturnFlag){
					selfDriveFlagLED = 1;
					pitDistance = (gettingPITdistance * 0.028333 * 0.01715)  + 4;

					comingBack();
					osDelay(1000);
					stop();
					osDelay(300);
					if ((pitDistance < 55) && (pitDistance > 5) ) {
						stop();
						break;
					}
				}
				stop();
				selfDriveFlagLED = 0;
			}
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
				break;
			case SOUTH:
				reverse();
				break;
			case EAST:
				turnRight();
				break;
			case WEST:
				turnLeft();
				break;
			case NORTH_EAST:
				rightForward();
				break;
			case SOUTH_EAST:
				rightReverse();
				break;
			case SOUTH_WEST:
				leftReverse();
				break;
			case NORTH_WEST:
				leftForward();
				break;
			default:
				stop();
				userSignal = STOP;
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
				//osMutexRelease(myMutex);
				endRaceSong = 0; 
				//playEndSong();
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
				selfDriveFlagLED = 1;
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
	initUltrasonic();
	offLEDModules();
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	
	//Initialize Semaphores
	brainSem = osSemaphoreNew(1, 0, NULL);
	moveSem = osSemaphoreNew(1, 0, NULL);
	soundSem = osSemaphoreNew(1, 1, NULL);
	selfDriveSem = osSemaphoreNew(1, 0, NULL);
	triggerSem = osSemaphoreNew(1, 0, NULL);
	myMutex = osMutexNew(NULL);
	
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
	osThreadNew(tUltrasonicThread, NULL, NULL);
	
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
