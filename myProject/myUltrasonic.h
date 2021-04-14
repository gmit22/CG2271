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

volatile int ready;
volatile uint16_t start;
volatile uint16_t startRecord;
volatile uint16_t endRecord;
volatile uint16_t valueRecord;

volatile uint16_t end;
volatile uint16_t counter;
volatile uint16_t num;

volatile uint16_t maincounter;

void initUltrasonic(void);
void ultrasonicTrigger(void);
void readUltrasonic(void);
void TPM0_IRQHandler(void);

#endif
