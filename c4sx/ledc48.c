// ledc48.c:  blink led on RB pin 0 -- simple SX C program for CC1B compiler
//                 C program similar to led28.src (assembler for SX)
//                 modified 07/30/2007 to include SX-Key directives
//                 modified 08/10/2007 to work with SX48 and
//                                                   bank0 on hold changed to bank1
//                 modified 09/02/2007 to use new CC1B 0.7A features

#pragma chip SX48 // select device

#pragma asmDir:	DEVICE OSCHS3
#pragma asmDir:	IRC_CAL IRC_FAST
#pragma asmDir:	FREQ	50_000_000
#pragma asmDir:	ID	'project'

// ************************************************
// ************************************************
// MACRO DEFINITIONS AND CONSTANTS

#define MAX_COUNT1 10
#define MAX_COUNT2 255
#define MAX_COUNT3 255
 
// ************************************************
// ************************************************
// IO PORT DEFINITIONS

bit pinB0 @ RB.0;

// ************************************************
// ************************************************
// GLOBAL VARIABLE DEFINITIONS

#pragma rambank 0
// unsigned char Count1, Count2;          // located in ram bank 0

#pragma rambank 2


// ************************************************
// ************************************************
// PROTOTYPES AND CODEPAGES

/* Prototypes are needed when a function is called before it
is defined. It can also be used for fine tuning the code
layout (i.e. which codepage to use for a function). */

void delay( void);
page2 char function2( void);



// ************************************************
// ************************************************
// INTERRUPT SUPPORT

#pragma origin 0   // interrupt must be located first on codepage 0

interrupt iServer(void)
{
    /* NOTE: W, STATUS and FSR are automatically saved to
       and restored from shadow registers */

    nop();
}



// ************************************************
// ************************************************
// INCLUDED C FILES AND MATH ROUTINES

// none in this program

#pragma codepage 0

// ************************************************
// ************************************************
// C FUNCTIONS


void delay( void)
{
	unsigned char Count1, Count2, Count3;  
	for (Count1 = MAX_COUNT1; Count1 != 0; Count1--) {
		for (Count2 = MAX_COUNT2; Count2 != 0; Count2--) {
			for (Count3 = MAX_COUNT3; Count3 != 0; Count3--) {
			// do nothing
			}
		}
	}
	
	return;

}


void main(void)
{

	// OPTION = 0xa8;  // ints off!
	
    clearRAM();  // built in function
#if 0
	
	    // configure 8-bit RB, all outputs
	TRISB = 0x00;   // all pins output
	LVL_B = 0xff;   // all TTL voltages
    ST_B = 0xff;    // schmitt triggers off
    PLP_B = 0xff;   // pull-ups off
    WKEN_B = 0xff;  // Wake up disabled
    WKED_B = 0xff;  // Wake up falling edge

    // configure 8-bit RC, all outputs
	TRISC = 0x00;   // all pins output
	LVL_C = 0xff;   // all TTL voltages
    ST_C = 0xff;    // schmitt triggers off
    PLP_C = 0xff;   // pull-ups off

    RB = 0xff;      // all LEDs off!
	
#endif
	
	// with SX48, using bank0 compilers OK but fails at
	// execution, LED does not blink.....
	bank1 unsigned char hold;
	bit holdB0 @ hold.0;	

	TRISB = 0xfe;   // %1111 1110
	RB = 0;
	while (1) {
	  delay();
	  holdB0 = pinB0;
	  hold++;
	  pinB0 = holdB0;
	}

}

