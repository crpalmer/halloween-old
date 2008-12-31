// DEMO.C : contains recommended program structure

#pragma chip SX28 // select device

// OTHER SAMPLE CODE:
// demo-ins.c : generating single instructions using C code
// demo-mat.c : integer math operations
// demo-fpm.c : floating point math
// demo-fxm.c : fixed point math
// demo-rom.c : const data and DW
// demo-ptr.c : indexed tables and pointers
// demo-var.c : defining RAM variables

/*
  It is RECOMMENDED to move definitions to header files (.h) and
  use #include on these. Also, because no linker is available, it
  is recommended to group related functions on separate files (.c)
  and use #include one these.
*/


// ************************************************
// ************************************************
// MACRO DEFINITIONS AND CONSTANTS

#define MY_CONSTANT1  10
#define MY_CONSTANT2  (MY_CONSTANT1 * 100L - 50)  // nested definition


// ************************************************
// ************************************************
// IO PORT DEFINITIONS

bit pinA @ RA.1;  // port A, bit 1
bit pinB @ RA.2;


// ************************************************
// ************************************************
// GLOBAL VARIABLE DEFINITIONS

#pragma rambank 0
char a0, b0;          // located in ram bank 0

#pragma rambank 2
char a1, b1;          // located in ram bank 2

#pragma rambank -
char a, bx;           // located in shared ram locations

// a bank type modifier overrides the current rambank
bank4 uns16 u16, table1[3];  // located in ram bank 4

bank2 bit bt1, bt;           // located in ram bank 2


#define DefaultRamBank 1
#pragma rambank DefaultRamBank  // change to default rambank



// ************************************************
// ************************************************
// PROTOTYPES AND CODEPAGES

/* Prototypes are needed when a function is called before it
is defined. It can also be used for fine tuning the code
layout (i.e. which codepage to use for a function). */

char function1( void);
page2 char function2( void);
#pragma location 3
void function3( void);
void function4( char aa);
void function5( int16);
#pragma location -


// ************************************************
// ************************************************
// INTERRUPT SUPPORT


#pragma origin 0   // interrupt must be located first on codepage 0

interrupt iServer(void)
{
    /* NOTE: W, STATUS and FSR are automatically saved to
       and restored from shadow registers */

    // handle the interrupt
    nop();
}



// ************************************************
// ************************************************
// INCLUDED C FILES AND MATH ROUTINES

/* Functions located in codepage 0 need to be included after the
interrupt routine. However, if the functions are not located in
codepage 0, then they can be included before the interrupt routine.
Using #pragma codepage ensures that functions not yet located (by
page modifier or #pragma location) are put on certain codepages */

/*
#pragma codepage 1
 #include "math16.h"  // 16 bit integer math routines
 #include "mm1.c"
 #include "mm2.c"
#pragma codepage 2
 #include "mm3.c"
#pragma codepage 1
 #include "mm4.c"
#pragma codepage 3
 #include "mm5.c"
*/

#pragma codepage 0



// ************************************************
// ************************************************
// C FUNCTIONS


void subr( void)
{
    bank0 char a, bx, c, d;
    bank0 uns16 a16;
    bank2 uns16 b16 = a16;

    a = bx;
    c = 10;
    a = 10 | bx | d;

    do  {
        nop();
        a --;
    } while ( --d > 0);

   #asm
    DW 0xFFF   ; any data or instruction
    DW 0x000
   #endasm
}


uns16 accumulate( void)
{
    bank0 uns8 i;
    bank0 uns16 rs = 0;

    // add all numbers from 1 to 199
    for (i = 1; i < 200; i++)
        rs += i;
    return rs;
}


void sub1( void)
{
    a0 = MODE;
    MODE = 3;
    OPTION = a;

    DDRA = 0x20;
    DDRB = 0x20;
    DDRC = 0x20;

    PLP_A = 0x20;
    PLP_B = 0x20;
    PLP_C = 0x20;

    LVL_A = 0x20;
    LVL_B = 0x20;
    LVL_C = 0x20;

    ST_B = 0x20;
    ST_C = 0x20;

    WKEN_B = 0x20;
    WKED_B = 0x20;
    WKPND_B = 0x20;

    CMP_B = a;
}


void main(void)
{
    clearRAM();  // built in function

    subr();
    sub1();

    bank2 uns16 s = accumulate();

    bt1 = 0;     // clear bit
    a.7 = 1;     // set bit
    bt = !bt;    // bit toggle

    if (a > bx)
        a &= 0xF0;  // mask bits

    // uns16 is 16 bit (unsigned long)
    bank1 uns16 aa16 = 1000;  // local variable
    bank1 uns16 bb = aa16+10000;
    aa16 |= 0x10F;  // set bits
    bb &= 0x7F;   // clear bits

    bank2 char i = 10;   // 8 bit unsigned

    aa16 -= i;

    aa16 = i * 200;

    aa16 --;     // decrement

    if (aa16 == 0  ||  !bt)
        a1 -= 33;
    if (!a.7)  // test single bit
        b0 += a1 + b1;

    nop();  // delay 1 instruction cycle

    W = 10;
}

