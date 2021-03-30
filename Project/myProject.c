/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

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

uint8_t rx_data;
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void app_main (void *argument) {
 
  // ...
  for (;;) {}
}
 


int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(app_main, NULL, NULL);    // Create application main thread
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
