#include "myPWM.h"

void initPWM() {
	
	//Enable Clock Gating for PORTB
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	//Configure mode 3 for pin PWM generation
	//Left
	PORTB -> PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB -> PCR[PTB0_Pin] |= PORT_PCR_MUX(3);
	PORTB -> PCR[PTB1_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB -> PCR[PTB1_Pin] |= PORT_PCR_MUX(3);
	
	//Right
	//Configure mode 3 for pin PWM generation
	PORTB -> PCR[PTB2_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB -> PCR[PTB2_Pin] |= PORT_PCR_MUX(3);
	PORTB -> PCR[PTB3_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB -> PCR[PTB3_Pin] |= PORT_PCR_MUX(3);
	
	//Enable Clock Gating for Timer1 & Timer2
	SIM->SCGC6 = (SIM_SCGC6_TPM1_MASK | SIM_SCGC6_TPM2_MASK);
	
	//Select clock for TPM module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	//Set Modulo value 48000000 / 128 = 375000 / 7500 = 50 Hz
	TPM1->MOD = 7500;
	TPM2->MOD = 7500;
	
	/* Edge-Aligned PWM */
	// Update SnC register: CMOD = 01, PS = 111 (128) 
	TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM1->SC &= ~(TPM_SC_CPWMS_MASK);
	
	TPM2->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM2->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM2->SC &= ~(TPM_SC_CPWMS_MASK);
	
	//Enable PWM on TPM1 Channel 0 -> PTB0
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_ELSB(1)) | (TPM_CnSC_MSB(1));
	
	//Enable PWM on TPM1 Channel 1 -> PTB1
	TPM1_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C1SC |= (TPM_CnSC_ELSB(1)) | (TPM_CnSC_MSB(1));
	
	//Enable PWM on TPM2 Channel 0 -> PTB2
	TPM2_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM2_C0SC |= (TPM_CnSC_ELSB(1)) | (TPM_CnSC_MSB(1));
	
	//Enable PWM on TPM2 Channel 1 -> PTB3
	TPM2_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM2_C1SC |= (TPM_CnSC_ELSB(1)) | (TPM_CnSC_MSB(1));
}
