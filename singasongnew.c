// This is an example program for music programming.
// You will run this program without modification.
// The program will repeatedly play a specified song.

// The program will continuously play one of the song  
// defined in mysong on a speaker connected to pin PA2.
// Authors: Min He
// Date: August 26, 2018

// 1. Header files 
#include "tm4c123gh6pm.h"
#include "SysTick.h"

// 2. Declarations Section

// define music data structure 
struct Note {
  unsigned char tone_index;
  unsigned char delay;
};
typedef const struct Note NTyp;

// initial values for piano major tones.
// Assume SysTick clock frequency is 16MHz.
const unsigned long Tone_Tab[] =
// Notes:  C, D, E, F, G, A, B
// Offset: 0, 1, 2, 3, 4, 5, 6
{30534,27211,24242,22923,20408,18182,16194, // C4 Major notes
15289,13621,12135,11454,10204,9091,8099, // C5 Major notes
7645,6810,6067,5727,5102,4545,4050}; // C6 Major notes

#define PAUSE 255				// assume there are less than 255 tones used in any song

// indexes for notes used in music scores
#define C5 0+7
#define D5 1+7
#define E5 2+7
#define F5 3+7
#define G5 4+7
#define A5 5+7
#define B5 6+7
#define C6 0+2*7
#define D6 1+2*7
#define E6 2+2*7
#define F6 3+2*7
#define G6 4+2*7
#define A6 5+2*7
#define B6 6+2*7

// doe ray mi fa so la ti 
// C   D   E  F  G  A  B
NTyp mysong[][50] =
 
// score table for Happy Birthday
{{//so   so   la   so   doe' ti
   G5,2,G5,2,A5,4,G5,4,C6,4,B5,4,
// pause so   so   la   so   ray' doe'
   PAUSE,4,  G5,2,G5,2,A5,4,G5,4,D6,4,C6,4,
// pause so   so   so'  mi'  doe' ti   la
   PAUSE,4,  G5,2,G5,2,G6,4,E6,4,C6,4,B5,4,A5,8, 
// pause fa'  fa'   mi'  doe' ray' doe' stop
	 PAUSE,4,  F6,2,F6,2, E6,4,C6,4,D6,4,C6,8,0,0},

 score table for Mary Had A Little Lamb
{E6, 4, D6, 4, C6, 4, D6, 4, E6, 4, E6, 4, E6, 8, 
 D6, 4, D6, 4, D6, 8, E6, 4, G6, 4, G6, 8,
 E6, 4, D6, 4, C6, 4, D6, 4, E6, 4, E6, 4, E6, 8, 
 D6, 4, D6, 4, E6, 4, D6, 4, C6, 8, 0, 0 },

// score table for Twinkle Twinkle Little Stars
{C6,4,C6,4,G6,4,G6,4,A6,4,A6,4,G6,8,F6,4,F6,4,E6,4,E6,4,D6,4,D6,4,C6,8, 
 G6,4,G6,4,F6,4,F6,4,E6,4,E6,4,D6,8,G6,4,G6,4,F6,4,F6,4,E6,4,E6,4,D6,8, 
 C6,4,C6,4,G6,4,G6,4,A6,4,A6,4,G6,8,F6,4,F6,4,E6,4,E6,4,D6,4,D6,4,C6,8,0,0}};

	// Function Prototypes
void Speaker_Init(void);
void Delay(void);
extern void EnableInterrupts(void);
void play_a_song(NTyp scoretab[]);

// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){
  Speaker_Init();
  SysTick_Init();
  EnableInterrupts();  // SysTick uses interrupts
  while(1){
		play_a_song(mysong);
  }
}

void play_a_song(NTyp scoretab[0])
{
	unsigned char i=0, j;

	while (scoretab[i].delay) {
		if (scoretab[i].tone_index==PAUSE) // index = 255 indicate a pause: stop systick
			SysTick_stop(); // silence tone, turn off SysTick timer
		else {
			SysTick_Set_Current_Note(Tone_Tab[scoretab[i].tone_index]);
			SysTick_start();
		}
		
		// tempo control: 
		// play current note for duration 
		// specified in the music score table
		for (j=0;j<scoretab[i].delay;j++) 
			Delay();
		
		SysTick_stop();
		i++;  // move to the next note
	}
	
	// pause after each play
	for (j=0;j<15;j++) 
		Delay();
}

// Subroutine to wait 0.1 sec for 16MHz system clock
// Inputs: None
// Outputs: None
// Notes: ...
void Delay(void){
	unsigned long volatile time;
  time = 727240*20/91;  // 0.1sec
  while(time){
		time--;
  }
}
// Make PA2 an output, enable digital I/O, ensure alt. functions off
void Speaker_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x01;           // 1) activate clock for Port A
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
                                    // 2) no need to unlock PA2
  GPIO_PORTA_PCTL_R &= ~0x00000F00; // 3) regular GPIO
  GPIO_PORTA_AMSEL_R &= ~0x04;      // 4) disable analog function on PA2
  GPIO_PORTA_DIR_R |= 0x04;         // 5) set direction to output
  GPIO_PORTA_AFSEL_R &= ~0x04;      // 6) regular port function
  GPIO_PORTA_DEN_R |= 0x04;         // 7) enable digital port
  GPIO_PORTA_DR8R_R |= 0x04;        // 8) optional: enable 8 mA drive on PA2 to increase the voice volumn
}
