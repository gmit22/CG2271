#ifndef myUltrasonic_H
#define myUltrasonic_H

#include "MKL25Z4.h"                    // Device header
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include <math.h>
#include "myBasic.h"

#define PTD2_Pin 2  // output Trigger
#define PTD3_Pin 3 // input Echo 
#define PTD4_Pin 4 // PWM pin

#define MASK(x) (1 << (x))

void initUltrasonic(void);

#endif
