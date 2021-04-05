#include "MKL25Z4.h"                    // Device header
#include "led_functions.h"

#define BAUD_RATE 9600




int main(void) {
	
	SystemCoreClockUpdate();
	initLED();
	offLEDModules();
	
	while(1) {
		solidGREEN_Stationery();
	}
}