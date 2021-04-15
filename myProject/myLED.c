#include "myLED.h"
#include "myBasic.h"


/* LED GPIO Initialization Function */
void initLED(void) {
	
	//Port E clock is already enabled in the myUART module
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK;
	
 // Configure MUX settings for rear led and set them as GPIO
 PORTA->PCR[LED_R1] &= ~PORT_PCR_MUX_MASK;
 PORTA->PCR[LED_R1] |= PORT_PCR_MUX(1);
 
 // Configure MUX settings for front led and set them as GPIO
 PORTE->PCR[LED_F2] &= ~PORT_PCR_MUX_MASK;
 PORTE->PCR[LED_F2] |= PORT_PCR_MUX(1);
 PORTE->PCR[LED_F3] &= ~PORT_PCR_MUX_MASK;
 PORTE->PCR[LED_F3] |= PORT_PCR_MUX(1);
 PORTE->PCR[LED_F4] &= ~PORT_PCR_MUX_MASK;
 PORTE->PCR[LED_F4] |= PORT_PCR_MUX(1);
 PORTE->PCR[LED_F5] &= ~PORT_PCR_MUX_MASK;
 PORTE->PCR[LED_F5] |= PORT_PCR_MUX(1);
 
 PORTB->PCR[LED_F8] &= ~PORT_PCR_MUX_MASK;
 PORTB->PCR[LED_F8] |= PORT_PCR_MUX(1);
 PORTB->PCR[LED_F9] &= ~PORT_PCR_MUX_MASK;
 PORTB->PCR[LED_F9] |= PORT_PCR_MUX(1);
 PORTB->PCR[LED_F10] &= ~PORT_PCR_MUX_MASK;
 PORTB->PCR[LED_F10] |= PORT_PCR_MUX(1);
 PORTB->PCR[LED_F11] &= ~PORT_PCR_MUX_MASK;
 PORTB->PCR[LED_F11] |= PORT_PCR_MUX(1);

 // Set Data Direction Registers for PortA, PortB and PortD
 PTA->PDDR |= MASK(LED_R1);
 PTB->PDDR |= (MASK(LED_F8) | MASK(LED_F9) | MASK(LED_F10) | MASK(LED_F11));
 PTE->PDDR |= (MASK(LED_F2) | MASK(LED_F3) | MASK(LED_F4) | MASK(LED_F5));
}

// LED starts blinking from PTE5 in line to PTB8
uint8_t green_led[8] = {LED_F5, LED_F4, LED_F3, LED_F2, LED_F11, LED_F10, LED_F9, LED_F8};

//set PortA pin 1 - LOW
void offRearLED() {
	PTA->PCOR |= MASK(LED_R1);
}

void offFrontLED() {
	PTE->PCOR |= (MASK(LED_F2) | MASK(LED_F3) | MASK(LED_F4) | MASK(LED_F5));
	PTB->PCOR |= (MASK(LED_F8) | MASK(LED_F9) | MASK(LED_F10) | MASK(LED_F11));
}

void offLEDModules() {
	offFrontLED();
	offRearLED();
}

void flashRED_Moving() {
	PTA->PSOR |= MASK(LED_R1);
	osDelay(FREQ_1HZ);
	PTA->PCOR |= MASK(LED_R1);
	osDelay(FREQ_1HZ);	
}

void flashRED_Stationery() {
	PTA->PSOR |= MASK(LED_R1);
	osDelay(FREQ_2HZ);
	PTA->PCOR |= MASK(LED_R1);
	osDelay(FREQ_2HZ);	
}

void runningGREEN_Moving(int i) {
		
	if (i < 4) {
		PTE -> PSOR |= MASK(green_led[i]);
		osDelay(DELAY_RUN);
		PTE -> PCOR |= MASK(green_led[i]);
		//osDelay(DELAY_RUN);
	} else if (i >= 4) {
		PTB -> PSOR |= MASK(green_led[i]);
		osDelay(DELAY_RUN);
		PTB -> PCOR |= MASK(green_led[i]);
		//osDelay(DELAY_RUN);
	} 
}

void solidGREEN_Stationery() {
	PTE -> PSOR |= MASK(LED_F2) | MASK(LED_F3) | MASK(LED_F4) | MASK(LED_F5);
	PTB -> PSOR |= MASK(LED_F8) | MASK(LED_F9) | MASK(LED_F10) | MASK(LED_F11);
}

void flashGREEN_Twice() {
	
	for (int i=0; i<2; i++) {
		PTE -> PSOR |= MASK(LED_F2) | MASK(LED_F3) | MASK(LED_F4) | MASK(LED_F5);
		PTB -> PSOR |= MASK(LED_F8) | MASK(LED_F9) | MASK(LED_F10) | MASK(LED_F11);
		delay(0x100000);
		PTE -> PCOR |= MASK(LED_F2) | MASK(LED_F3) | MASK(LED_F4) | MASK(LED_F5);
		PTB -> PCOR |= MASK(LED_F8) | MASK(LED_F9) | MASK(LED_F10) | MASK(LED_F11);
		delay(0x100000);		
	}
	
}


