#ifndef myUART_H
#define myUART_H


#include "MKL25Z4.h"                    // Device header
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#define BAUD_RATE 9600
#define UART_RX_PORTE23 23
#define UART2_INT_PRIO 128

#define MASK(x) (1 << (x))
#define Q_SIZE 32

extern volatile uint8_t userSignal;

typedef struct {
	unsigned char Data[Q_SIZE];
	unsigned int Head;
	unsigned int Tail;
	unsigned int Size;
} Q_T;



// Init UART2
void setupUART2(uint32_t);

void UART2_IRQHandler(void);

#endif