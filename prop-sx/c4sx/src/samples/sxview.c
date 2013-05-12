// sxview.c :  demos using the SX-KEY in debug,  mostly this is a toy showing C coding
//                   run this code under debug
//                   click run once, look at the memory/register matrix, should be full of 0x00
//                   click run again, look for 'c4sx' in the memory/register  matrix
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

#define MARKER	0xff

// ************************************************
// ************************************************
// IO PORT DEFINITIONS

// ************************************************
// ************************************************
// GLOBAL VARIABLE DEFINITIONS

bank0 char array0[16];
bank1 char array1[16];
bank2 char array2[16];
bank3 char array3[16];
bank4 char array4[16];
bank5 char array5[16];
bank6 char array6[16];
bank7 char array7[16];

#pragma rambank 1

#pragma rambank -
int i;
int j;

// a bank type modifier overrides the current rambank
// bank4 uns16 u16, table1[3];  // located in ram bank 4

#define DefaultRamBank -
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

void setByte(int x, int y) {

	switch (x) {
		case 0:
			array0[y] = MARKER;
			break;
		case 1:
			array1[y] = MARKER;
			break;
		case 2:
			array2[y] = MARKER;
			break;
		case 3:
			array3[y] = MARKER;
			break;
		case 4:
			array4[y] = MARKER;
			break;
		case 5:
			array5[y] = MARKER;
			break;
		case 6:
			array6[y] = MARKER;
			break;
		case 7:
			array7[y] = MARKER;
			break;
			
	}
	
}

void hLine(int x, int y, int len) {
	
	for (i=0; i < len; i++) {
		setByte(x + i, y);
	}

}

void vLine(int x, int y, int len) {
	
	for (i=0; i < len; i++) {
		setByte(x, y + i);
	}

}

void main(void)
{

    clearRAM();  // built in function

	// stop before!
	dobreak();
	
	j = 0;
	vLine(j,0,3);
	hLine(j,0,3);
	hLine(j,2,3);
	
	j++;
	vLine(j,4,2);
	hLine(j,5,3);
	vLine((j+2),4,3);
	
	j++;
	j++;
	hLine(j,8,2);
	vLine(j,8,3);
	hLine(j,10,2);
	vLine((j+1),10,3);
	hLine(j,12,2);
	
	j = 13;
	for (i=5; i <= 7; i++) {
		setByte(i,j);
		j++;
	}
	setByte(7,13);
	setByte(5,15);
	
	// stop after!!
	dobreak();

}

