
#pragma chip SX28 // select device

// FIRST ADD THE INTERRUPT ROUTINE (if any)
// #include "app-int.c"

#pragma rambank 0
#pragma codepage 1
#include "math24x.h"
#pragma codepage 0

uns16 ad_data;
bank1 fixed16_8  tx, av, mg, a, vx;
bank2 fixed16_8  prev, kp;

void main(void)
{
    vx = 3.127;
    tx += ad_data;    // automatic type cast
    ad_data = kp;     // assign integer part
    if ( tx < 0)
        tx = -tx;     // make positive
    av = tx/20.0;
    //mg = av * 1.25;
    //a = mg * 0.98;    // 0.980469: error on constant: 0.000478
    prev = vx;
    vx = a/5.0 + prev;

    // kp = vx * 0.036;       // 0.03515626: error on constant: 0.024
    kp = vx / (1.0/0.036); // 27.7773437 error on constant: 0.0000156
}


