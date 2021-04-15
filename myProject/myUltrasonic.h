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

extern volatile uint32_t read;

extern volatile int ready;
extern volatile uint32_t start;
extern volatile uint32_t startRecord;
extern float value;

extern volatile int orange;

//extern volatile uint16_t end;
extern volatile uint32_t counter;
extern volatile uint32_t read;

void initUltrasonic(void);
void ultrasonicTrigger(void);
void readUltrasonic(void);
void TPM0_IRQHandler(void);

#endif
