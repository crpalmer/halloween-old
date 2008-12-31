
#pragma chip SX28 // select device

// FIRST ADD THE INTERRUPT ROUTINE (if any)
// #include "app-int.c"

// locate library routine in code page 2 instead of code page 1
page2 float24 operator* _fmul24( float24 arg1f24, float24 arg2f24);

#pragma rambank 0
#pragma codepage 1
#include "math24f.h"
#pragma codepage 0

uns16 ad_data;
bank1 float tx, av, mg, a, vx;
bank2 float prev, kp;

void main(void)
{
    InitFpFlags();   // enable rounding as default
    vx = 3.127;
    tx += ad_data;   // automatic type cast
    ad_data = kp;    // assign integer part
    if ( tx < 0)
        tx = -tx;    // make positive
    av = tx/20.0;
    mg = av * 1.25;
    a = mg * 0.98;
    prev = vx;
    vx = a/5.0 + prev;

    kp = vx * 0.036;
    kp = vx / (1.0/0.036);
}
