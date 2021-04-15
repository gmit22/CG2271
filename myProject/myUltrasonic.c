#include "myUltrasonic.h"

void initUltrasonic(){
    //Setting up PIT module
  SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
  //writing 0 to mdis to enable clock to PIT
  PIT->MCR &= ~PIT_MCR_MDIS_MASK;
  //initialize PIT channel to count down from value, timer counts to 0 generate intrrupt and restart countdown
  PIT->CHANNEL[0].LDVAL |= 0x3FFFFF; //every 400ms, loadvalue = T * 10485760�1, interrupt every T seconds

  PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK; 
  PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK; //Let the PIT channel generate interrupt requests 
  PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK; 

  //Initialize PIT0 to count down from starting_value 
  PIT->CHANNEL[1].LDVAL =0xFFFFF; //every 100ms 
  //No chaining of timers 
  PIT->CHANNEL[1].TCTRL &= PIT_TCTRL_CHN_MASK; 
  //Let the PIT channel generate interrupt requests 
  PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK; 
  
  //set priority
  NVIC_SetPriority(PIT_IRQn, 3);
  //clear pending irq
  NVIC_ClearPendingIRQ(PIT_IRQn);
  //enable PIT interrupt in NVIC
  NVIC_EnableIRQ(PIT_IRQn);
	
	//Set PTD2 as GPIO output to trigger the sensor
	PORTD->PCR[PTD2_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[PTD2_Pin] |= PORT_PCR_MUX(1);
	PTD->PDDR |= MASK(PTD2_Pin);

	//Set PTD3 as GPIO input to receieve echo
//	PORTD->PCR[PTD3_Pin] &= ~PORT_PCR_MUX_MASK;
//	PORTD->PCR[PTD3_Pin] |= PORT_PCR_MUX(1);
//	PTD->PDDR &= ~MASK(PTD3_Pin);
//	
	PORTD->PCR[PTD3_Pin] |= (PORT_PCR_MUX(1) |
                       PORT_PCR_PS_MASK | // Pull select
                       PORT_PCR_PE_MASK | // Pull enable
                       PORT_PCR_IRQC(0x0b)); //0x0b rising and falling means look at IRQC config [1010 - interrupt on falling edge]

  // Set PORT D Switch bit to input
  PTD->PDDR &= ~(MASK(PTD3_Pin));

	NVIC_SetPriority(PORTD_IRQn, 2); // Set priority for the entire port, doesn't matter here because only single interrupt
  /* If got interrupt triggered, NVIC will capture. If interrupt is enabled and halfway got disabled and want to re-enable. 
     Any left over IRQ pending before it was disabled, you want to clear it and not carry it out when it is re-enabled. */
  NVIC_ClearPendingIRQ(PORTD_IRQn); 
  NVIC_EnableIRQ(PORTD_IRQn); // Capture the interrupts
}

//volatile int ready = 0;
//float value = 0;
//volatile uint32_t start = 0x0000;
//volatile uint32_t startRecord = 0x0000;

//volatile uint32_t counter = 0x0000;
//volatile uint32_t read = 0x0000;

//void initUltrasonic() {
//	
//	//Enable clock gating for portD
//	//SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
//	
//	//Set PTD2 as GPIO output to trigger the sensor
//	PORTD->PCR[PTD2_Pin] &= ~PORT_PCR_MUX_MASK;
//	PORTD->PCR[PTD2_Pin] |= PORT_PCR_MUX(1);
//	PTD->PDDR |= MASK(PTD2_Pin);

//	//Set PTD3 pin to TPM0_CH3 mode and pin to input
//	PORTD->PCR[PTD3_Pin] &= ~PORT_PCR_MUX_MASK;
//	PORTD->PCR[PTD3_Pin] |= (PORT_PCR_MUX(4));
//	PTD->PDDR &= ~(MASK(PTD3_Pin)); //??
//	
//	//Configure channel 3 from TPM0 as input capture for both edges and enabling interrupt
//	TPM0_C3SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK) | (TPM_CnSC_CHIE_MASK));
//	TPM0_C3SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_ELSA(1) | TPM_CnSC_CHIE(1));

//	//Enable TPM0 interrupt
//	NVIC_SetPriority(TPM0_IRQn, 2);
//	NVIC_ClearPendingIRQ(TPM0_IRQn); 
//	NVIC_EnableIRQ(TPM0_IRQn);

//	//Disable Clk to avoid interrupt triggers
//	SIM->SCGC6 &= ~SIM_SCGC6_TPM0_MASK;
//}

//void ultrasonicTrigger(){
//	
//	//starting trigger
//	PTD->PDOR |= MASK(PTD2_Pin);
//	delay(0x11); //10us orginal = 0x11
//	
//	//Off trigger after 10 seconds
//	PTD->PDOR &= ~MASK(PTD2_Pin);
//	delay(0x18e70);
//}

////void readUltrasonic(void){
////	float value = 0;
////	
////	//Enable Clk Source to TPM0 module
////	//SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
////	
////		//Configure channel 3 from TPM0 as input capture for both edges and enabling interrupt
////	TPM0_C3SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK) | (TPM_CnSC_CHIE_MASK));
////	TPM0_C3SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_ELSA(1) | TPM_CnSC_CHIE(1));
////	
////	
////	ultrasonicTrigger();
////	
////	while(!ready){}; //measuring when echo comesback
////	value = end - startRecord;
////	start = 0;
////	//value = (value * 2.6666 * 0.01715 * 1.5) - 337 ;  // determine speed of ultrasonic from frequency and speedy of sound = 34300cm/s /2 =17150;
////	
////	//Disable Clk to avoid interrupt triggers
////	//SIM->SCGC6 &= ~SIM_SCGC6_TPM0_MASK;
////		
////			//Configure channel 3 from TPM0 as input capture for both edges and enabling interrupt
////	TPM0_C3SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK) | (TPM_CnSC_CHIE_MASK));
////	//TPM0_C3SC &= ~(TPM_CnSC_ELSB(1) | TPM_CnSC_ELSA(1) | TPM_CnSC_CHIE(1));
////	
////		
////		
////	counter = 0;
////		
////	read = value;
////}


