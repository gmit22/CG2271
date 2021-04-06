#ifndef myLED_H
#define myLED_H

#include "MKL25Z4.h"                    // Device header
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

// Front LED pins
#define LED_F2 2 // PortE Pin 2
#define LED_F3 3 // PortE Pin 3
#define LED_F4 4 // PortE Pin 4
#define LED_F5 5 // PortE Pin 5
#define LED_F8 8 // PortB Pin 8
#define LED_F9 9 // PortB Pin 9
#define LED_F10 10 // PortB Pin 10
#define LED_F11 11 // PortB Pin 11

// Rear LED pins
#define LED_R1 1 // PortA Pin 1

//Delay for RED rear led to blink
#define FREQ_1HZ 500 //milliseconds
#define FREQ_2HZ 250 //milliseconds

//Delay for running GREEN forward LED
#define DELAY_RUN 500 //milliseconds
#define DELAY_FLASH 160000 //milliseconds


// LED starts blinking from PTE5 in line to PTB8
//uint8_t green_led[8] = {LED_F5, LED_F4, LED_F3, LED_F2, LED_F11, LED_F10, LED_F9, LED_F8};

#define MASK(x) (1 << (x))

void initLED(void);

void offRearLED(void);

void offFrontLED(void);

void offLEDModules(void);

void flashRED_Moving(void);

void flashRED_Stationery(void);

void runningGREEN_Moving(int);

void flashGREEN_Twice(void);

void solidGREEN_Stationery(void);




#endif
