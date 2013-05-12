// sxstring48.c : test a few of the string.h and stdlib.h functions
//                    and show results via sxio.h
//                    2007/08/10  created version for SX48
//
//                    string-oriented functions tested:
//                    strcpy();
//                    strcat();
//		      strlen();
//                    itoa();
//
//                    note: the results are also shown in rambank6
//                    which can be seen via the SX-Key debugger in the
//                    'Dx' column; Y = 0x59 and N = 0x4E            
//  
//                    note the SX-Key break use in the C dobreak() function
//
//		      modified 09/02/2007 to work with CC1B 0.7A features

#pragma chip SX48 // select device

// SX-KEY specific settings:
/*<?asm
	; DEVICE	turbo
	; DEVICE	optionx
	; DEVICE	stackx
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

// a bank type modifier overrides the current rambank
// bank4 uns16 u16, table1[3];  // located in ram bank 4

// ************************************************
// ************************************************
// PROTOTYPES AND CODEPAGES
#pragma codepage 1
void doit(void);

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

// put functions in code page 0
// sxio.h must be in 0 -- it contains the interrupt handler....
#include <sxio.h>

#pragma codepage 3
#pragma rambank 2
#include <stdlib.h>

#pragma codepage 0
#pragma rambank -

// ************************************************
// ************************************************
// C FUNCTIONS

void dobreak(void) {
	/*<?asm
	 break
	 ?>*/
}

#pragma codepage 0
void main(void){
	doit();
}

#pragma codepage 1
#pragma rambank 2
void doit(void){

	int result;
	int result2;
	int i;
	int test_cnt = 0;
	
	static const char lit1[3] = "C4";
	static const char lit2[3] = "SX";
	static const char lit3[5] = "C4SX";
	
	static const msg[] = "\n\rTest results:\n\r";
	
	bank3 char buffer1[16];
	bank4 char buffer2[16];
	bank5 char buffer3[16];
	bank6 char buffer4[16];
	bank7 char buffer5[16];
	// bank8 char buffer6[8];
	
    clearRAM();  // built in function
	
	dobreak();	// chance to stop
	
	// clear test results array to spaces
	for (i = 0; i < 16; i++) {
		buffer5[i] = ' ';
	}

	// init some in-RAM strings and buffers
	buffer1[0] = 'C';  // 'C4'
	buffer1[1] = '4';
	buffer1[2] = 0x0;
	buffer2[0] = 'S';  // 'SX'
	buffer2[1] = 'X';
	buffer2[2] = 0x0;
	buffer3[0] = 0x0;
	buffer4[0] = 0x0;

	// test 1:  does lit1 = buffer1?
	if (strcmp(buffer1,lit1) == 0) {
		buffer5[test_cnt] = 'Y';
	} else {
		buffer5[test_cnt] = 'N';
	}
	test_cnt++;
	
	// test 2: does lit2 = buffer2?
	if (strcmp(buffer2, lit2) == 0) {
		buffer5[test_cnt] = 'Y';
	} else {
		buffer5[test_cnt] = 'N';
	}
	test_cnt++;
	
	// test 3: does strlen( lit1) = 2?
	if (strlen(lit2) == 2) {
		buffer5[test_cnt] = 'Y';
	} else {
		buffer5[test_cnt] = 'N';
	}
	test_cnt++;	
	
	// test 4: does strlen( buffer1) = 2?
	if (strlen(buffer1) == 2) {
		buffer5[test_cnt] = 'Y';
	} else {
		buffer5[test_cnt] = 'N';
	}
	test_cnt++;	
	
	// test5: does strcpy(buffer3,buffer1) work?
	strcpy(buffer3,buffer1);
	if (strcmp(buffer3,buffer1) == 0) {
		buffer5[test_cnt] = 'Y';
	} else {
		buffer5[test_cnt] = 'N';
	}
	test_cnt++;	
	
	// test6: does strcat(buffer3,buffer2) work?
	strcat(buffer3,buffer2);
	if (strcmp(buffer3,lit3) == 0) {
		buffer5[test_cnt] = 'Y';
	} else {
		buffer5[test_cnt] = 'N';
	}
	test_cnt++;	
	
	// test7: does strlen(buffer3)  =  4?
	if (strlen(buffer3) == 4) {
		buffer5[test_cnt] = 'Y';
	} else {
		buffer5[test_cnt] = 'N';
	}
	test_cnt++;	

	// test8: itoa() and strlen(), strcpy() and strcmp() work?
	result = strlen(lit3);
	strcpy(buffer3,"4");
	itoa(result,buffer4);
	if (strcmp(buffer3,buffer4) == 0) {
		buffer5[test_cnt] = 'Y';
	} else {
		buffer5[test_cnt] = 'N';
	}
	test_cnt++;		
	
	dobreak();
#if 1
	// show test results....
	serial_on(100);		// turn on I/O
						// don't debug/step thru here as
						// interrupts are going ....
	result = 0;
	result2 = 0;
	puts(msg);
	for (i=0; i < 15; i++) {
		if (buffer5[i] == ' ') {
			break;
		}
		putchar((char) i + '1');	// trick
		putchar(':');
		if (buffer5[i] == 'Y') {
			puts("TEST OK\n\r");
			result++;
		} else {
			puts("TEST FAILED\n\r");
			result2++;
		}
	}
	puts("\n\rPASSES: ");
	putchar((char) result + '0');	// trick
	puts("\n\rFAILS: ");
	putchar((char) result2 + '0');	// trick
	
	serial_off();
	
#endif
						// safe to run debugger again....
	dobreak();	// chance to stop

}

