#include "MKL25Z4.h"                    // Device header
enum led_color{red, green, blue};

#define RED_LED 18
#define GREEN_LED 19
#define BLUE_LED 1
#define MASK(x) (1 << (x))

void initGPIO(void)
{
	// Enable Clock to PORTB and PORTD
	SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTD_MASK));
	
	//Configure MUX settings to make all 3 pins GPIO
	PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
	
	PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
	
	PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
	
	// Set Data Direction Registers for PortB and PortD
	PTB->PDDR |= (MASK(RED_LED) | MASK(GREEN_LED));
	PTD->PDDR |= MASK(BLUE_LED);
}

static void delay(volatile uint32_t nof) {
  while(nof!=0) {
    __asm("NOP");
    nof--;
  }
}

static void turnLEDOff() {
	PTB->PSOR = MASK(RED_LED);
	PTB->PSOR = MASK(GREEN_LED);
	PTD->PSOR = MASK(BLUE_LED);
}

static void turnLEDOn(enum led_color color) {
	if (color == red) {
		turnLEDOff();
		PTB->PCOR = MASK(RED_LED);
	}
	if (color == green) {
		turnLEDOff();
		PTB->PCOR = MASK(GREEN_LED);
	}
	if (color == blue) {
		turnLEDOff();
		PTD->PCOR = MASK(BLUE_LED);
	}
}