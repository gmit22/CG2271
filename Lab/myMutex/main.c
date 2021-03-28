/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include CMSIS_device_header
#include "cmsis_os2.h"
#include "led_functions.h"
 
 osMutexId_t myMutex;
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void app_main (void *argument) {
 
  // ...
  for (;;) {}
}

/*----------------------------------------------------------------------------
 * Application led_green_thread
 *---------------------------------------------------------------------------*/
void led_green_thread(void *argument) {
	for (;;) {
		osMutexAcquire(myMutex, osWaitForever);
		
		turnLEDOn(green);
		osDelay(1000);
		//delay(0x80000);
		turnLEDOff(green);
		osDelay(1000);
		//delay(0x80000);
		
		osMutexRelease(myMutex);
	}
}

/*----------------------------------------------------------------------------
 * Application led_red_thread
 *---------------------------------------------------------------------------*/
void led_red_thread(void *argument) {
	for (;;) {
		osMutexAcquire(myMutex, osWaitForever);
		
		turnLEDOn(red);
		osDelay(1000);
		//delay(0x80000);
		turnLEDOff(red);
		osDelay(1000);
		//delay(0x80000);
		
		osMutexRelease(myMutex);
	}
}

const osThreadAttr_t thread_attr = {
	.priority = osPriorityNormal1
};
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	initGPIO();
	offLED();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	myMutex = osMutexNew(NULL);
	osThreadNew(led_red_thread, NULL, NULL);
	osThreadNew(led_green_thread, NULL, NULL);    // Create application main thread
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
