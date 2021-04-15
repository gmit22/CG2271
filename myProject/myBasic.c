#include "MKL25Z4.h"
#include "myBasic.h"

//mySemaphores
osSemaphoreId_t brainSem, moveSem, soundSem, selfDriveSem, triggerSem;
osMutexId_t myMutex;

/* Delay Function */
void delay(volatile uint32_t nof) {
  while(nof!=0) {
    __asm("NOP");
    nof--;
  }
}
