#ifndef myBasic_H
#define myBasic_H

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

extern osSemaphoreId_t brainSem;
extern osSemaphoreId_t moveSem;
extern osSemaphoreId_t soundSem;
extern osSemaphoreId_t selfDriveSem;
extern osSemaphoreId_t triggerSem;

void delay(uint32_t);

#endif
