// SIZE & SPEED

#pragma library 1
/*
uns16 operator* _multM8_8( uns8 arg1, char W);
uns16 operator* _multM16_16( uns16 arg1, uns16 arg2);
uns24 operator* _multUM24_8( uns24 arg1, uns8 arg2);
*/

int8 operator*( int8 arg1, int8 arg2) @

uns16 operator* _multM8_8( uns8 arg1, char W)
{
    uns16 rval = 0;
    Carry = 0;
   #define addRR(rval,arg,bit) if(arg&((uns8)1<<bit)) rval.high8+=W; rval=rr(rval);
    addRR( rval, arg1, 0);
    addRR( rval, arg1, 1);
    addRR( rval, arg1, 2);
    addRR( rval, arg1, 3);
    addRR( rval, arg1, 4);
    addRR( rval, arg1, 5);
    addRR( rval, arg1, 6);
    addRR( rval, arg1, 7);
   #undef addRR
    return rval;
}



int16 operator*( int16 arg1, int16 arg2) @

uns16 operator* _multM16_16( uns16 arg1, uns16 arg2)
{
    uns16 rval;
    rval = (uns16) arg1.low8 * arg2.low8;
    W = arg1.high8;
    if (W)
        rval.high8 += W * arg2.low8;
    W = arg2.high8;
    if (W)
        rval.high8 += W * arg1.low8;
    return rval;
}


uns24 operator*( uns8 arg1, uns24 arg2) exchangeArgs @

uns24 operator* _multUM24_8( uns24 arg1, uns8 arg2)
{
    char tmp = arg1.high8;
    arg1.high16 = (uns16) arg1.mid8 * arg2;
    W = tmp;
    if (W)
        arg1.high8 += W * arg2;
    tmp = arg1.mid8;
    arg1.low16 = (uns16) arg1.low8 * arg2;
    arg1.high16 += tmp;
    return arg1;
}

#pragma library 0
