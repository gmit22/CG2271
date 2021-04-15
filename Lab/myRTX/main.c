/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "led_functions.h"
 
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
		turnLEDOn(green);
		//osDelay(1000);
		delay(0x80000);
		turnLEDOff(green);
		//osDelay(1000);
		delay(0x80000);
	}
}

/*----------------------------------------------------------------------------
 * Application led_red_thread
 *---------------------------------------------------------------------------*/
void led_red_thread(void *argument) {
	for (;;) {
		turnLEDOn(red);
		//osDelay(1000);
		delay(0x80000);
		turnLEDOff(red);
		//osDelay(1000);
		delay(0x80000);
	}
}
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	initGPIO();
	offLED();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(led_green_thread, NULL, NULL);    // Create application main thread
	osThreadNew(led_red_thread, NULL, NULL);
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
