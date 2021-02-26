#include "MKL25Z4.h"                    // Device header
#include "led_functions.h"

#define BAUD_RATE 9600
#define UART_TX_PORTE22 22
#define UART_RX_PORTE23 23
#define UART2_INT_PRIO 128
#define PTB0_Pin 0
#define PTB1_Pin 1

volatile uint8_t rx_data = 0x69;

void initUART2(uint32_t);
void UART2_Transmit_Poll(uint8_t); 
void delay(volatile uint32_t);
uint8_t UART2_Receive_Poll(void);


void initUART2(uint32_t baud_rate) {
	uint32_t divisor, bus_clock;
	
	//Enable clock to UART2 and PORTE
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	//Connect UART pins for PTE22, PTE23
	PORTE->PCR[UART_TX_PORTE22] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[UART_TX_PORTE22] |= PORT_PCR_MUX(4);
	
	PORTE->PCR[UART_RX_PORTE23] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[UART_RX_PORTE23] |= PORT_PCR_MUX(4);
	
	//Ensure TX and RX are disabled before configuration
	UART2->C2 &= ~((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
	
	//Set baud rate to desired value
	bus_clock = (DEFAULT_SYSTEM_CLOCK)/2;
	divisor = bus_clock / (baud_rate * 16);
	UART2->BDH = UART_BDH_SBR(divisor >> 8);
	UART2->BDL = UART_BDL_SBR(divisor);
	
	//No parity, 8 bits
	UART2->C1 = 0;
	UART2->S2 = 0;
	UART2->C3 = 0;
	
	/* Interrupt UART
	NVIC_SetPriority(UART2_IRQn, UART2_INT_PRIO);
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
	
	UART2->C2 |= UART_C2_RIE_MASK;
	*/
	
	//Enable TX and RX
	UART2->C2 |= ((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
}

void initPWM(void) {
	
	//Enable Clock Gating for PORTB
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	//Configure mode 3 for pin PWM generation
	PORTB -> PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB -> PCR[PTB0_Pin] |= PORT_PCR_MUX(3);
	
	PORTB -> PCR[PTB1_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB -> PCR[PTB1_Pin] |= PORT_PCR_MUX(3);
	
	//Enable Clock Gating for timer1
	SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
	
	//Select clock for TPM module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	//Set Modulo value 20971520 / 128 = 163840 / 3276 = 50 Hz
	//TPM1->MOD = 3276;
	
	//Set Modulo value 48000000 / 128 = 375000 / 7500 = 50 Hz
	TPM1->MOD = 7500;
	
	/* Edge-Aligned PWM */
	// Update SnC register: CMOD = 01, PS = 111 (128) 
	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);
	
	//Enable PWM on TPM1 Channel 0 -> PTB0
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1)) | (TPM_CnSC_MSB(1));
	
	//Enable PWM on TPM1 Channel 1 -> PTB1
	TPM1_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C1SC |= (TPM_CnSC_ELSB(1)) | (TPM_CnSC_MSB(1));
}
/* UART2 Transmit Interrupt Handler */
/* void UART2_IRQHandler(void) {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	if (UART2->S1 & UART_S1_RDRF_MASK) {
		global_rx = UART2->D;
	}
} */

/* UART2 Transmit Poll */
void UART2_Transmit_Poll(uint8_t data) {
	while (!(UART2->S1 & UART_S1_TDRE_MASK));
	UART2->D = data;
}

/* UART2 Receive Poll */
uint8_t UART2_Receive_Poll(void) {
	while (!(UART2->S1 & UART_S1_RDRF_MASK));
	return (UART2->D);
}

int main(void) {
	
	SystemCoreClockUpdate();
	initGPIO();
	initUART2(BAUD_RATE);
	initPWM();
	turnLEDOff();
	
	TPM1_C0V = 0x0EA6;
	
	while(1) {
		/* Rx and Tx */
		rx_data = UART2_Receive_Poll();
		
		if (rx_data == 0) {
			turnLEDOn(red);
		} else if (rx_data == 1) {
			turnLEDOff();
		} else if(rx_data == 2) {
			TPM1_C0V = 0x0EA6;	//50% duty cycle
		} else if (rx_data == 3) {
			TPM1_C0V = 0x0;		//0% duty cycle
		}
	}
}



