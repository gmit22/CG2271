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

extern volatile uint16_t read;

extern volatile int ready;
extern volatile uint16_t start;
extern volatile uint16_t startRecord;
extern volatile uint16_t endRecord;
extern volatile uint16_t valueRecord;

extern volatile uint16_t end;
extern volatile uint16_t counter;
extern volatile uint16_t num;

extern volatile uint16_t maincounter;

extern volatile uint16_t read;

void initUltrasonic(void);
void ultrasonicTrigger(void);
void readUltrasonic(void);
void TPM0_IRQHandler(void);

#endif
