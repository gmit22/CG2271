#include "MKL25Z4.h"                    // Device header
#include <math.h>
#include "mySound.h"
#include "myBasic.h"
#include "myPWM.h"

// change this to make the song slower or faster
int tempo = 144;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!

int starWarsMelody [] = {
	NOTE_A4, 500, NOTE_A4, 500, NOTE_A4, 500, NOTE_A4, 350, NOTE_C5, 150, NOTE_A4, 500, NOTE_F4, 350, 
	NOTE_C5, 150, NOTE_A4, 650, REST, 500, NOTE_E5, 500, NOTE_E5, 500, NOTE_E5, 500, NOTE_F5, 350, 
	NOTE_C5, 150, NOTE_GS4, 500, NOTE_F4, 350, NOTE_C5, 150, NOTE_A4, 650, REST, 500, NOTE_A5, 500, 
	NOTE_A4, 300, NOTE_A4, 150, NOTE_A5, 500, NOTE_GS5, 325, NOTE_G5, 175, NOTE_FS5, 125, NOTE_F5, 125, 
	NOTE_FS5, 250, REST, 325, NOTE_AS, 250, NOTE_DS5, 500, NOTE_D5, 325, NOTE_CS5, 175, NOTE_C5, 125, 
	NOTE_AS4, 125, NOTE_C5, 250, REST, 350, NOTE_F4, 250, NOTE_GS4, 500, NOTE_F4, 350, NOTE_A4, 125, 
	NOTE_C5, 500, NOTE_A4, 375, NOTE_C5, 125, NOTE_E5, 650, REST, 500, NOTE_A5, 500, NOTE_A4, 300, 
	NOTE_A4, 150, NOTE_A5, 500, NOTE_GS5, 325, NOTE_G5, 175, NOTE_FS5, 125, NOTE_F5, 125, NOTE_FS5, 250, 
	REST, 325, NOTE_AS, 250, NOTE_DS5, 500, NOTE_D5, 325, NOTE_CS5, 175, NOTE_C5, 125, NOTE_AS4, 125, 
	NOTE_C5, 250, REST, 350, NOTE_F4, 250, NOTE_GS4, 500, NOTE_F4, 375, NOTE_C5, 125, NOTE_A4, 500, 
	NOTE_F4, 375, NOTE_C5, 125, NOTE_A4, 650, REST, 650
};

int startSong [] = {
	  //Super Mario Connected Song
  NOTE_C5,-4, NOTE_G4,-4, NOTE_E4,4, //45
  NOTE_A4,-8, NOTE_B4,-8, NOTE_A4,-8, NOTE_GS4,-8, NOTE_AS4,-8, NOTE_GS4,-8,
  NOTE_G4,8, NOTE_D4,8, NOTE_E4,-2,  
};

int endSong[] = {

  // Jigglypuff's Song
  // Score available at https://musescore.com/user/28109683/scores/5044153
  
  NOTE_D5,-4, NOTE_A5,8, NOTE_FS5,8, NOTE_D5,8,
  NOTE_E5,-4, NOTE_FS5,8, NOTE_G5,4,
  NOTE_FS5,-4, NOTE_E5,8, NOTE_FS5,4,
  NOTE_D5,-2,
  NOTE_D5,-4, NOTE_A5,8, NOTE_FS5,8, NOTE_D5,8,
  NOTE_E5,-4, NOTE_FS5,8, NOTE_G5,4,
  NOTE_FS5,-1,
  NOTE_D5,-4, NOTE_A5,8, NOTE_FS5,8, NOTE_D5,8,
  NOTE_E5,-4, NOTE_FS5,8, NOTE_G5,4,
  
  NOTE_FS5,-4, NOTE_E5,8, NOTE_FS5,4,
  NOTE_D5,-2,
  NOTE_D5,-4, NOTE_A5,8, NOTE_FS5,8, NOTE_D5,8,
  NOTE_E5,-4, NOTE_FS5,8, NOTE_G5,4,
  NOTE_FS5,-1,
  
};

//1 nop takes 1 cycle? So 1 delay is 1/48 microsec

static void delay100x(volatile uint32_t nof) {
	for(int i =0;i<100;i++) {
		delay(nof);
	}
}


void playRaceSong() {
	int notes = sizeof(starWarsMelody) / sizeof(starWarsMelody[0]);

	int noteDuration = 0, period = 0;
	
	//while(1) {
		
		for(int i = 0; i<notes; i+=2) {
			// calculates the duration of each note
			noteDuration = starWarsMelody[i + 1];
			period = TO_MOD(starWarsMelody[i]);
			TPM0->MOD = period;
			TPM0_C0V = period / 4; //12.5% duty cycle
			osDelay(noteDuration); //*25
			//TPM0->MOD = 0;
			TPM0_C0V = 0;
			osDelay(noteDuration); // *24
		}
		
	//}
}

void playConnectSong () {
	int notes = sizeof(startSong) / sizeof(startSong[0]);

	int wholenote = (60000 * 4) / tempo;

	int divider = 0, noteDuration = 0;

	uint32_t period;

	//while(1) {

		for(int i = 0; i<notes; i+=2) {
			divider = startSong[i + 1];
			if (divider > 0) {
				noteDuration = (wholenote) / divider;
			} else if (divider < 0) {
				noteDuration = (wholenote) / (int)fabs((float)divider);
				noteDuration *= 1.5;
			}
			period = TO_MOD(startSong[i]);
			TPM0->MOD = period;
			TPM0_C0V = period / 8; //12.5% duty cycle
			delay100x(2*9*noteDuration);
			TPM0->MOD = 0;
			TPM0_C0V = 0;
			delay100x(2*10*noteDuration);
		}

	//}
}


void playEndSong() {
	osSemaphoreAcquire(soundSem, osWaitForever);
	int notes = sizeof(endSong) / sizeof(endSong[0]);

	// This calculates the duration of a whole note in ms (60s/tempo)*4 beats.
	int wholenote = (60000 * 4) / tempo;

	int divider = 0, noteDuration = 0;

	uint32_t period;


		for(int i = 0; i<notes; i+=2) {
			divider = endSong[i + 1];
			if (divider > 0) {
				noteDuration = (wholenote) / divider;
			} else if (divider < 0) {
				noteDuration = (wholenote) / (int)fabs((float)divider);
				noteDuration *= 1.5;
			}
			period = TO_MOD(endSong[i]);
			TPM0->MOD = period;
			TPM0_C0V = period / 8;
			delay100x(2*9*noteDuration);
			TPM0->MOD = 0;
			TPM0_C0V = 0;
			delay100x(2*10*noteDuration);
		}
		osSemaphoreRelease(soundSem);
}
