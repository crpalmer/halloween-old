// INTEGER MATH

#pragma chip SX28 // select device

// FIRST ADD THE INTERRUPT ROUTINE (if any)
// #include "app-int.c"

// Then include math libraries:
#pragma rambank 0
#include "math24.h"   // 8 - 24 bit math


void xmain( void)
{
    bank1 char a8, b8;      // 8 bit unsigned
    bank1 uns16 a16;        // 16 bit unsigned
    bank1 uns16 b16 = a16;  // 16 bit assignment
    bank1 int24 i24;        // 24 bit signed
    bank1 uns24 a24, b24;   // 24 bit unsigned

    a24 += b24;   // 24 bits addition

    a24 ++;       // increment any integer
    i24 -= 1;     // decrement

    a24 = b24 * 201100;  // 24 bit multiplication

    a24 /= 10345;  // division

    i24 -= b16;   // mix variable sizes

    a8 = a8 * b8;           // no warning: storing 8 bit only
    a16 = a8 * b8;          // warning: result is truncated to 8 bit
    a16 = (uns16)a8 * b8;   // type cast to get 16 bit result

    a16 = ++a8;     // increment before assignment
    a16 = a8 & 0xF;
}


void main(void)
{
    xmain();
}



