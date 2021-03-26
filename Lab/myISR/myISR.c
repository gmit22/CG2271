#include "MKL25Z4.h"                    // Device header
#include "led_functions.h"							// LED header

#define SW_POS 6

volatile int led_control = 0;

void PORTD_IRQHandler(void) {
	
	delay(0x80000); //does this debounce?
	
	//Clear Pending IRQ
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	
	//Update variable
	if (led_control == 0) {
		led_control = 1;
	} else if (led_control == 1) {
		led_control = 2;
	} else {
		led_control = 0;
	}
	
	//Clear INT Flag
	PORTD -> ISFR |= MASK(SW_POS);
}

void initSwitch(void) {
	
	//enable clock for PortD
	SIM -> SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	/*Select GPIO and enable pull-up resistors and interrupts on falling
	edges of pin connected to switch*/
	PORTD -> PCR[SW_POS] |= (PORT_PCR_MUX(1) |
													PORT_PCR_PE_MASK |
													PORT_PCR_PS_MASK |
													PORT_PCR_IRQC(0x0a));
	
	//Set PORT D switch bit to input
	PTD -> PDDR &= ~MASK(SW_POS);
	
	//Enable Interrupts
	NVIC_SetPriority(PORTD_IRQn,2);
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	NVIC_EnableIRQ(PORTD_IRQn);
}

void InitGPIO(void)
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

int main(void) {
	SystemCoreClockUpdate();
	initSwitch();
	InitGPIO();
	
	while(1) {
		if (led_control == 0) {
			turnLEDOff();
			turnLEDOn(red);
		} else if (led_control == 1) {
			turnLEDOff();
			turnLEDOn(green);
		} else {
			turnLEDOff();
			turnLEDOn(blue);
		}
	}
}
