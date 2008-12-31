// sxwatch.c :  using the SX-Key in debug, this is a visual bubble sort
//                      under the debugger, enter run, look at the data under '9x', enter run again
//                     see the sorted numbers, in descending order,  under '9x'
//
//                    note the SX-Key break in the C dobreak() function
//
//                 modified 09/02/2007 to work with CC1B 0.7A features
#pragma chip SX28 // select device

// SX-KEY specific settings:
/*<?asm
	DEVICE	turbo
	DEVICE	optionx
	DEVICE	stackx
	DEVICE	OSCHS3
	IRC_CAL	IRC_FAST
	FREQ	50_000_000
	; RESET   main			
?>*/


// ************************************************
// ************************************************
// MACRO DEFINITIONS AND CONSTANTS

// ************************************************
// ************************************************
// IO PORT DEFINITIONS

// ************************************************
// ************************************************
// GLOBAL VARIABLE DEFINITIONS

#pragma rambank 4
char array1[16];

#pragma rambank 1

#pragma rambank -
int i;
int size;
int hold;
int hold2;
bit flip;

// a bank type modifier overrides the current rambank
// bank4 uns16 u16, table1[3];  // located in ram bank 4

#define DefaultRamBank 3
#pragma rambank DefaultRamBank  // change to default rambank

// ************************************************
// ************************************************
// PROTOTYPES AND CODEPAGES

// ************************************************
// ************************************************
// INTERRUPT SUPPORT

#pragma origin 0   // interrupt must be located first on codepage 0

// ************************************************
// ************************************************
// INCLUDED C FILES AND MATH ROUTINES

/* Functions located in codepage 0 need to be included after the
interrupt routine. However, if the functions are not located in
codepage 0, then they can be included before the interrupt routine.
Using #pragma codepage ensures that functions not yet located (by
page modifier or #pragma location) are put on certain codepages */

// #include "SXIO.H"

#pragma codepage 0

// ************************************************
// ************************************************
// C FUNCTIONS

void dobreak(void) {
	/*<?asm
	 break
	 ?>*/
}

void main(void)
{

    // OPTION = 0xa8;  // ints off!

    clearRAM();  // built in function

	
	size = sizeof(array1);
	for (i=0; i < size; i++) {
		array1[i] = i;
	}	
	
	dobreak();
	
	// Q&D sort
	flip = 1;
	
	size--;
	while (flip == 1) {
		flip = 0;
		for (i=0; i < size; i++) {
			hold = array1[i];
			hold2 = array1[i+1];
			if (hold < hold2) {				
				// array1[i] = array1[i+1];  gives CC1B error....
				array1[i] = hold2;
				array1[i+1] = hold;
				flip = 1;
			}
		}
	}	
	
	dobreak();

}

