// sxlimits.c :  show some CC1B limits, using the SX-Key in debuger
//
//                    note the SX-Key break in the C dobreak() function
//
//                 modified 09/04/2007 to work with CC1B 0.7A features
//
#pragma chip SX28 // select device

// SX-KEY specific settings:
/*<?asm
	DEVICE	turbo
	DEVICE	optionx
	DEVICE	stackx
	DEVICE	OSCHS3
	IRC_CAL	IRC_FAST
	FREQ	50_000_000
;	RESET   main
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

#pragma rambank 1
int rb1_i;
int rb1_j;
int rb1_k;
int *rb1_ptr;

#pragma rambank 4
char rb4_array[16];

#pragma rambank 7
int rb7_i;
int rb7_j;
int rb7_k;
int *rb7_ptr;

#pragma rambank -
int i;
int j;
int *ptr;
int size;
int temp1;
bit b;

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

#pragma codepage 0

// ************************************************
// ************************************************
// C FUNCTIONS

void dobreak(void) {
	/*<?asm
	 break
	 ?>*/
}

void dummy(int a1, int a2) {
	// note that sum is pushed to bank3
	bank3 int sum;
	
	sum = a1 + a2;
}
#pragma rambank -
void main(void)
{
    clearRAM();  // built in function
	
//
// example of needing a simpler statement...
//
#if 0
	for (i=0; i < sizeof(rb4_array); i++) {
		rb4_array[i] = i;
	}
	/*
        for (i=0; i < sizeof(rb4_array); i++) {
								^------
	Error sxlimits.c 95: Unable to generate code
	(The C syntax is correct. However, CC1B is unable to generate code.
	The workaround is often to split the code into simpler statements,
	using an extra variable to store temporary results. Sometimes it is
	enough to change the sequence of operations)
	*/
#else	
	size = sizeof(rb4_array);
	for (i=0; i < size; i++) {
		rb4_array[i] = i;
	}

#endif	
	dobreak();
//
// example of one index limit 
//
#if 0
	i = 1;
	j = 2;
	rb4_array[i] = rb4_array[j];
	/*
	  rb4_array[i] = rb4_array[j];
                                                    ^------
	Error sxlimits.c 120: Only one index or indirect addressing is allowed in an expression
	(CC1B can not handle multiple indexing in the same expression. The workaround
	is to use temporary variables and split the code into several simpler
	statements, with only one index or indirect addressing in each expression)
	*/	
#else
	i = 1;
	j = 2;
	temp1 = rb4_array[j];
	rb4_array[i] = temp1; 
#endif
	dobreak();
//
// example of argument passing, with increment, limits 
// note:  dummy(++i, ++j) works OK!
//
#if 0
	i = 1;
	j = 2;
	dummy(i++, j++);
	/*
	dummy(i++, j++);
				  ^------
	Error sxlimits.c 151: Syntax error
	(Probably unbalanced or missing paranthesis. The marker ^------ shows
	the approximate position where CC1B gives up)
	*/	
#else
	i = 1;
	j = 2;
	dummy(i, j); 
	i++;
	j++;
#endif	
	dobreak();
//
// example of pointer limits with ram pages 
// solution is generally to put some used variables in page 0
//
#if 0
	rb1_i = 1;
	rb1_ptr = &rb7_i;
	*rb1_ptr = rb1_i;
	/*
        *rb1_ptr = rb1_i;
                                 ^------
	Error sxlimits.c 176: Unable to update the bank selection bits
	(CC1B can not update the bank selection bits. This can happen if a
	skip instruction test a variable in one bank, and the next conditional
	instruction requires a different updating of the bank selection bits.
	The cure can be to rewrite the code or move a variable to another bank)
	*/	
#else
	i = 1;
	rb1_ptr = &rb7_i;
	*rb1_ptr = i;
#endif	
	dobreak();	
//
// no support for '?:' conditional expression 
//
#if 0
	temp1 = (i > j) ? i : j;
	/*
        temp1 = (i > j) ? i : j;
                                ^------
	Error sxlimits.c 197: The '?..:..' syntax is not supported
	(The statement must be rewritten using 'if')
	*/	
#else
	if (i > j) {
		temp1 = i;
	} else {
		temp1 = j;
	}
#endif	
	dobreak();	
//
// problems inverting a bit
//
#if 0
	i = 0;
	b = 0;
	i = ~i;		// inverting a byte is alright
	b = ~b;		// inverting a bit is not  .....
	/*
        b = ~b;         // inverting a bit is not  .....
									^------
	Error sxlimits.c 221: Unable to generate code
	(The C syntax is correct. However, CC1B is unable to generate code.
	The workaround is often to split the code into simpler statements,
	using an extra variable to store temporary results. Sometimes it is
	enough to change the sequence of operations)
	*/	
#else
	i = 0;
	b = 0;
	i = ~i;		// inverting a byte is alright
	if (b == 0) b = 1; else b = 0;
#endif	
	dobreak();			
}
// end of sxlimits.c
