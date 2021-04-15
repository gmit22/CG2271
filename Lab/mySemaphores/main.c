/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include CMSIS_device_header
#include "cmsis_os2.h"
#include "led_functions.h"

#define SW_POS 6
#define BAUD_RATE 9600
#define UART_TX_PORTE22 22
#define UART_RX_PORTE23 23
#define UART2_INT_PRIO 128
#define PTB0_Pin 0
#define PTB1_Pin 1
 
 osMutexId_t myMutex;
 osSemaphoreId_t mySem;
 
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
	
	//Interrupt UART
	NVIC_SetPriority(UART2_IRQn, UART2_INT_PRIO);
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
	
	UART2->C2 |= UART_C2_RIE_MASK;
	
	//Enable TX and RX
	UART2->C2 |= ((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
}

void UART2_IRQHandler(void) {
	NVIC_ClearPendingIRQ(UART2_IRQn);
	if (UART2->S1 & UART_S1_RDRF_MASK) {
		osSemaphoreRelease(mySem);
	}
}
 
void PORTD_IRQHandler() {
	
	//Clear Pending IRQ
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	
	delay(0x80000);	//helps with debouncing
	osSemaphoreRelease(mySem);
	
	//Clear INT Flag
	PORTD->ISFR |= MASK(SW_POS);
}
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void app_main (void *argument) {
 
  // ...
  for (;;) {}
}

/*----------------------------------------------------------------------------
 * Application led_green_thread
 *---------------------------------------------------------------------------*/
void led_green_thread(void *argument) {
	for (;;) {
		//osMutexAcquire(myMutex, osWaitForever);
		osSemaphoreAcquire(mySem, osWaitForever);
		
		turnLEDOn(green);
		osDelay(1000);
		//delay(0x80000);
		turnLEDOff(green);
		osDelay(1000);
		//delay(0x80000);
		
		//osSemaphoreRelease(mySem);
		//osMutexRelease(myMutex);
	}
}

/*----------------------------------------------------------------------------
 * Application led_red_thread
 *---------------------------------------------------------------------------*/
void led_red_thread(void *argument) {
	for (;;) {
		//osMutexAcquire(myMutex, osWaitForever);
		osSemaphoreAcquire(mySem, osWaitForever);
		
		turnLEDOn(red);
		osDelay(1000);
		//delay(0x80000);
		turnLEDOff(red);
		osDelay(1000);
		//delay(0x80000);
		
		//osSemaphoreRelease(mySem);
		//osMutexRelease(myMutex);
	}
}

const osThreadAttr_t thread_attr = {
	.priority = osPriorityNormal1
};
 
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	initGPIO();
	initSwitch();
	offLED();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	//myMutex = osMutexNew(NULL);
	mySem = osSemaphoreNew(1, 0, NULL);
	osThreadNew(led_red_thread, NULL, NULL);
	osThreadNew(led_green_thread, NULL, NULL);    // Create application main thread
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
