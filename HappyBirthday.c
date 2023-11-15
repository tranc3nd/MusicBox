// This is an example program for music programming.
// You will run this program without modification. 

// The program will continuously play "Happy Birthday"
// on pin PA2.
// Authors: Min He
// Date: August 26, 2018

// Header files 
#include "tm4c123gh6pm.h"
#include "SysTick.h"

// positive logic Speaker/Headset connected to PA2
#define PA2                     (*((volatile unsigned long *)0x40004010))
#define PAUSE 255				// assume there are less than 255 tones used in any song

// 2. Declarations Section

// define music note data structure 
struct Note {
  unsigned char tone_index;
  unsigned char delay;
};
typedef const struct Note NTyp;

// Constants
#define TONE_DURATION 2 // each tone uses the same duration 

// initial values for piano major tones.
// Assume SysTick clock frequency is 16MHz.
const unsigned long tonetab[] =
// C, D, E, F, G, A, B
// 1, 2, 3, 4, 5, 6, 7
//{76445,68104,60675,57269,51021,45455,40495, // C5 Major: lower C
//	38223,34052,30337,28635,25510,22727,20248, // C6 Major: middle C
//	19111,17026,15169,14317,12755,11364,10124};// C7 Major: Upper C
{30534,27211,24242,22923,20408,18182,16194, // C4 Major notes
15289,13621,12135,11454,10204,9091,8099, // C5 Major notes
7645,6810,6067,5727,5102,4545,4050}; // C6 Major notes

// index definition for tones used in happy birthday.
//#define G6 4+7
//#define A6 5+7
//#define B6 6+7
//#define C7 0+14
//#define D7 1+14
//#define E7 2+14
//#define F7 3+14
//#define G7 4+14
#define G6 4
#define A6 5
#define B6 6
#define C7 0+7
#define D7 1+7
#define E7 2+7
#define F7 3+7
#define G7 4+7

// note table for Happy Birthday
// doe ray mi fa so la ti 
// C   D   E  F  G  A  B
NTyp happybirthday[] = 
{
// so   so   la   so   doe' ti
   G6,2,G6,2,A6,4,G6,4,C7,4,B6,4,
// pause so   so   la   so   ray' doe'
   PAUSE,4,  G6,2,G6,2,A6,4,G6,4,D7,4,C7,4,
// pause so   so   so'  mi'  doe' ti   la
   PAUSE,4, G6,2,G6,2,G7,4,E7,4,C7,4,B6,4,A6,8, 
// pause fa'  fa'   mi'  doe' ray' doe'  stop
	 PAUSE,4,  F7,2,F7,2, E7,4,C7,4,D7,4,C7,8, 0,0
};

//   Function Prototypes
void Speaker_Init(void);
void Delay(void);
extern void EnableInterrupts(void);
void play_a_song(NTyp notetab[]);

// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
int main(void){
  Speaker_Init();
  SysTick_Init();
  EnableInterrupts();  // The grader uses interrupts
  while(1){
		play_a_song(happybirthday);
  }
}

void play_a_song(NTyp notetab[])
{
	unsigned char i=0, j;

	while (notetab[i].delay) { // delay==0 indicate end of the song
		if (notetab[i].tone_index==PAUSE) // index = 255 indicate a pause: stop systick
			SysTick_stop(); // silence tone, turn off SysTick timer
		else { // play a regular note
			SysTick_Set_Current_Note(tonetab[notetab[i].tone_index]);
			SysTick_start();
		}
		
		// tempo control: 
		// play current note for specified duration
		for (j=0;j<notetab[i].delay;j++) 
			Delay();
		
		SysTick_stop();
		i++;  // move to the next note
	}
	
	// pause after each play
	for (j=0;j<15;j++) 
		Delay();
}

// Subroutine to wait 0.1 sec
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
