/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "led_functions.h"
#define MSG_COUNT 1

typedef struct
{
	uint8_t cmd;
	uint8_t data;
}myDataPkt;

osMessageQueueId_t redMsg, greenMsg, blueMsg;

osThreadId_t redLED_Id, greenLED_Id, blueLED_Id, control_Id;
osEventFlagsId_t redLED_Id, greenLED_Id, blueLED_Id;
 
/*----------------------------------------------------------------------------
 * Application led_green_thread
 *---------------------------------------------------------------------------*/
void led_green_thread(void *argument) {
//	for (;;) {
//		//osThreadFlagsWait(0x00000001, osFlagsWaitAny, osWaitForever);
//		osEventFlagsWait(greenLED_Id, 0x0001, osFlagsWaitAny, osWaitForever);
//		turnLEDOn(green);
//		osDelay(1000);
//		turnLEDOff(green);
//		osDelay(1000);
//	}
	
	myDataPkt myRxData;
	for (;;) {
		osMessageQueueGet(greenMsg, &myRxData, NULL, osWaitForever);
		if (myRxData.cmd == 0x01 && myRxData.data == 0x01) {
			turnLEDOn(green);
			osDelay(1000);
			turnLEDOff(green);
			osDelay(1000);
		}
	}
}

/*----------------------------------------------------------------------------
 * Application led_red_thread
 *---------------------------------------------------------------------------*/
void led_red_thread(void *argument) {
//	for (;;) {
//		//osThreadFlagsWait(0x00000001, osFlagsWaitAny, osWaitForever);
//		osEventFlagsWait(redLED_Id, 0x0001, osFlagsWaitAny, osWaitForever);
//		turnLEDOn(red);
//		osDelay(1000);
//		turnLEDOff(red);
//		osDelay(1000);
//	}
	
	myDataPkt myRxData;
	for (;;) {
		osMessageQueueGet(redMsg, &myRxData, NULL, osWaitForever);
		if (myRxData.cmd == 0x01 && myRxData.data == 0x01) {
			turnLEDOn(red);
			osDelay(1000);
			turnLEDOff(red);
			osDelay(1000);
		}
	}
}

/*----------------------------------------------------------------------------
 * Application led_blue_thread
 *---------------------------------------------------------------------------*/
void led_blue_thread(void *argument) {
//	for (;;) {
//		//osThreadFlagsWait(0x00000001, osFlagsWaitAny, osWaitForever);
//		osEventFlagsWait(blueLED_Id, 0x0001, osFlagsWaitAny, osWaitForever);
//		turnLEDOn(blue);
//		osDelay(1000);
//		turnLEDOff(blue);
//		osDelay(1000);
//	}
	
	myDataPkt myRxData;
	for (;;) {
		osMessageQueueGet(blueMsg, &myRxData, NULL, osWaitForever);
		if (myRxData.cmd == 0x01 && myRxData.data == 0x01) {
			turnLEDOn(blue);
			osDelay(1000);
			turnLEDOff(blue);
			osDelay(1000);
		}
	}
}

/*----------------------------------------------------------------------------
 * Application control thread
 *---------------------------------------------------------------------------*/
void control_thread (void *argument) {
//	for (;;) {
//		//osThreadFlagsSet(redLED_Id, 0x00000001);
//		osEventFlagsSet(redLED_Id, 0x0001);
//		osDelay(1000);
//		//osThreadFlagsSet(greenLED_Id, 0x00000001);
//		osEventFlagsSet(greenLED_Id, 0x0001);
//		osDelay(1000);
//		//osThreadFlagsSet(blueLED_Id, 0x00000001);
//		osEventFlagsSet(blueLED_Id, 0x0001);
//		osDelay(1000);
//	}
	
	myDataPkt myData;
	
	myData.cmd = 0x01;
	myData.data = 0x01;
	
	for (;;) {
		osMessageQueuePut(redMsg, &myData, NULL, 0);
		osDelay(2000);
		osMessageQueuePut(greenMsg, &myData, NULL, 0);
		osDelay(2000);
		osMessageQueuePut(blueMsg, &myData, NULL, 0);
		osDelay(2000);
		
		osMessageQueuePut(redMsg, &myData, NULL, 0);
		osMessageQueuePut(greenMsg, &myData, NULL, 0);
		osMessageQueuePut(blueMsg, &myData, NULL, 0);
		osDelay(2000);
	}
}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	initGPIO();
	offLED();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	redLED_Id = osThreadNew(led_red_thread, NULL, NULL);	
  greenLED_Id = osThreadNew(led_green_thread, NULL, NULL);    // Create application main thread
	blueLED_Id = osThreadNew(led_blue_thread, NULL, NULL);
	control_Id = osThreadNew(control_thread, NULL, NULL);
	
	redMsg= osMessageQueueNew(MSG_COUNT , sizeof(myDataPkt), NULL);
	greenMsg= osMessageQueueNew(MSG_COUNT , sizeof(myDataPkt), NULL);
	blueMsg= osMessageQueueNew(MSG_COUNT , sizeof(myDataPkt), NULL);
	
//	osThreadNew(led_red_thread, NULL, NULL);	
//  osThreadNew(led_green_thread, NULL, NULL);    // Create application main thread
//	osThreadNew(led_blue_thread, NULL, NULL);
//	osThreadNew(control_thread, NULL, NULL);
//	redLED_Id = osEventFlagsNew(NULL);
//	greenLED_Id = osEventFlagsNew(NULL);
//	blueLED_Id = osEventFlagsNew(NULL);
	
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
