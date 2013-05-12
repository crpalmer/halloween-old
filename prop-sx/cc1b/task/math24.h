// SIZE

#pragma library 1
/*
uns16 operator* _mult8x8( uns8 arg1, uns8 arg2);
int16 operator* _multS8x8( int8 arg1, int8 arg2);
uns16 operator* _mult16x16( uns16 arg1, uns16 arg2);
uns24 operator* _multU16x8( uns16 arg1, uns8 arg2);
uns24 operator* _multU24x8( uns24 arg1, uns8 arg2);
uns24 operator* _multU24x16( uns24 arg1, uns16 arg2);
int24 operator* _multS16x16( int16 arg1, int16 arg2);
uns24 operator* _mult24x24( uns24 arg1, uns24 arg2);
uns16 operator/ _divU16_8( uns16 arg1, uns8 arg2);
uns24 operator/ _divU24_8( uns24 arg1, uns8 arg2);
uns16 operator/ _divU16_16( uns16 arg1, uns16 arg2);
uns24 operator/ _divU24_16( uns24 arg1, uns16 arg2);
uns24 operator/ _divU24_24( uns24 arg1, uns24 arg2);
int16 operator/ _divS16_8( int16 arg1, int8 arg2);
int24 operator/ _divS24_8( int24 arg1, int8 arg2);
int16 operator/ _divS16_16( int16 arg1, int16 arg2);
int24 operator/ _divS24_16( int24 arg1, int16 arg2);
int24 operator/ _divS24_24( int24 arg1, int24 arg2);
uns8 operator% _remU16_8( uns16 arg1, uns8 arg2);
uns8 operator% _remU24_8( uns24 arg1, uns8 arg2);
uns16 operator% _remU16_16( uns16 arg1, uns16 arg2);
uns16 operator% _remU24_16( uns24 arg1, uns16 arg2);
uns24 operator% _remU24_24( uns24 arg1, uns24 arg2);
int8 operator% _remS16_8( int16 arg1, int8 arg2);
int8 operator% _remS24_8( int24 arg1, int8 arg2);
int16 operator% _remS16_16( int16 arg1, int16 arg2);
int16 operator% _remS24_16( int24 arg1, int16 arg2);
int24 operator% _remS24_24( int24 arg1, int24 arg2);
*/

#if __CoreSet__ < 1600
 #define genAdd(r,a) W=a; btsc(Carry); W=incsz(a); r+=W
 #define genSub(r,a) W=a; btss(Carry); W=incsz(a); r-=W
 #define genAddW(r,a) W=a; btsc(Carry); W=incsz(a); W=r+W
 #define genSubW(r,a) W=a; btss(Carry); W=incsz(a); W=r-W
#else
 #define genAdd(r,a) W=a; r=addWFC(r)
 #define genSub(r,a) W=a; r=subWFB(r)
 #define genAddW(r,a) W=a; W=addWFC(r)
 #define genSubW(r,a) W=a; W=subWFB(r)
#endif


int8 operator*( int8 arg1, int8 arg2)  @

uns16 operator* _mult8x8( uns8 arg1, uns8 arg2)
{
    uns16 rval;
    char counter = sizeof(arg2)*8;
    rval.high8 = 0;
    W = arg1;
    do  {
        arg2 = rr( arg2);
        if (Carry)
            rval.high8 += W;
        rval = rr( rval);
        counter = decsz(counter);
    } while (1);
    return rval;
}


int16 operator* _multS8x8( int8 arg1, int8 arg2)
{
    uns16 rval;
    char counter = sizeof(arg2)*8;
    int8 tmpArg2 = arg2;
    rval.high8 = 0;
    W = arg1;
    do  {
        tmpArg2 = rr( tmpArg2);
        if (Carry)
            rval.high8 += W;
        rval = rr( rval);
        counter = decsz(counter);
    } while (1);
    W = arg2;
    if (arg1 < 0)
        rval.high8 -= W;
    W = arg1;
    if (arg2 < 0)
        rval.high8 -= W;
    return rval;
}


int16 operator*( int16 arg1, int16 arg2) @

uns16 operator* _mult16x16( uns16 arg1, uns16 arg2)
{
    uns16 rval;
    char counter = sizeof(arg1)*8;
    do  {
        Carry = 0;
        rval = rl( rval);
        arg1 = rl( arg1);
        if (Carry)
            rval += arg2;
        counter = decsz(counter);
    } while (1);
    return rval;
}



uns24 operator*( uns8 arg1, uns16 arg2) exchangeArgs @

uns24 operator* _multU16x8( uns16 arg1, uns8 arg2)
{
    uns24 rval;
    rval.high8 = 0;
    char counter = sizeof(arg1)*8;
    W = arg2;
    do  {
        arg1 = rr( arg1);
        if (Carry)
            rval.high8 += W;
        rval = rr(rval);
        counter = decsz(counter);
    } while (1);
    return rval;
}


uns24 operator*( uns8 arg1, uns24 arg2) exchangeArgs @

uns24 operator* _multU24x8( uns24 arg1, uns8 arg2)
{
    uns8 rvalH = 0;
    char counter = 1+sizeof(arg1)*8;
    W = arg2;
    goto ENTRY;
    do  {
        if (Carry)
            rvalH += W;
        rvalH = rr( rvalH);
      ENTRY:
        arg1 = rr( arg1);
        counter = decsz(counter);
    } while (1);
    return arg1;
}



int24 operator* _multS16x16( int16 arg1, int16 arg2)
{
    int24 rval;

    char counter = sizeof(arg1)*8;
    int16 tmpArg1 = arg1;
    rval.low8 = 0;
    do  {
        Carry = 0;
        rval = rl( rval);
        tmpArg1 = rl( tmpArg1);
        if (Carry)  {
            rval.low8 += arg2.low8;
            genAdd( rval.mid8, arg2.high8);
            if (Carry)
                rval.high8 += 1;
        }
        counter = decsz(counter);
    } while (1);

    if (arg1 < 0)
        rval.high8 -= arg2.low8;

    if (arg2 < 0)
        rval.high8 -= arg1.low8;

    return rval;
}


uns24 operator*( uns16 arg1, uns24 arg2) exchangeArgs @

uns24 operator* _multU24x16( uns24 arg1, uns16 arg2)
{
    uns24 rval;
    rval.low16 = 0;
    char counter = sizeof(arg2)*8;
    do  {
        Carry = 0;
        rval = rl( rval);
        arg2 = rl( arg2);
        if (Carry)  {
            rval.low8 += arg1.low8;
            genAdd( rval.mid8, arg1.mid8);
            genAdd( rval.high8, arg1.high8);
        }
        counter = decsz(counter);
    } while (1);
    return rval;
}




int24 operator*( int24 arg1, int24 arg2) @

uns24 operator* _mult24x24( uns24 arg1, uns24 arg2)
{
    uns24 rval;
    char counter = sizeof(arg1)*8;
    do  {
        Carry = 0;
        rval = rl( rval);
        arg1 = rl( arg1);
        if (Carry)  {
            rval.low8 += arg2.low8;
            genAdd( rval.mid8, arg2.mid8);
            genAdd( rval.high8, arg2.high8);
        }
        counter = decsz(counter);
    } while (1);
    return rval;
}





uns16 operator/ _divU16_8( uns16 arg1, uns8 arg2)
{
    uns8 rm = 0;
    char counter = sizeof(arg1)*8+1;
    goto ENTRY;
    do  {
        rm = rl( rm);
        uns8 tmp = rl( tmp);
        W = rm - arg2;
        if (tmp&1)
            Carry = 1;
        if (Carry)
            rm = W;
       ENTRY:
        arg1 = rl( arg1);
        counter = decsz(counter);
    } while (1);
    return arg1;
}


uns24 operator/ _divU24_8( uns24 arg1, uns8 arg2)
{
    uns8 rm = 0;
    char counter = sizeof(arg1)*8+1;
    goto ENTRY;
    do  {
        rm = rl( rm);
        uns8 tmp = rl( tmp);
        W = rm - arg2;
        if (tmp&1)
            Carry = 1;
        if (Carry)
            rm = W;
       ENTRY:
        arg1 = rl( arg1);
        counter = decsz(counter);
    } while (1);
    return arg1;
}


uns16 operator/ _divU16_16( uns16 arg1, uns16 arg2)
{
    uns16 rm = 0;
    char counter = sizeof(arg1)*8+1;
    goto ENTRY;
    do  {
        rm = rl( rm);
        W = rm.low8 - arg2.low8;
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto ENTRY;
        rm.high8 = W;
        rm.low8 -= arg2.low8;
        Carry = 1;
       ENTRY:
        arg1 = rl( arg1);
        counter = decsz(counter);
    } while (1);
    return arg1;
}


uns24 operator/ _divU24_16( uns24 arg1, uns16 arg2)
{
    uns16 rm = 0;
    char counter = sizeof(arg1)*8+1;
    goto ENTRY;
    do  {
        rm = rl( rm);
        if (Carry)
            goto SUBTRACT;
        W = rm.low8 - arg2.low8;
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto ENTRY;
       SUBTRACT:
        rm.low8 -= arg2.low8;
        genSub( rm.high8, arg2.high8);
        Carry = 1;
       ENTRY:
        arg1 = rl( arg1);
        counter = decsz(counter);
    } while (1);
    return arg1;
}


uns24 operator/ _divU24_24( uns24 arg1, uns24 arg2)
{
    uns24 rm = 0;
    char counter = sizeof(arg1)*8+1;
    goto ENTRY;
    do  {
        rm = rl( rm);
        W = rm.low8 - arg2.low8;
        genSubW( rm.mid8, arg2.mid8);
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto ENTRY;
        rm.high8 = W;
        rm.low8 -= arg2.low8;
        genSub( rm.mid8, arg2.mid8);
        Carry = 1;
       ENTRY:
        arg1 = rl( arg1);
        counter = decsz(counter);
    } while (1);
    return arg1;
}


int8  operator/ (int8 arg1, int8 arg2) @

int16 operator/ _divS16_8( int16 arg1, int8 arg2)
{
    uns8 rm = 0;
    char counter = 16+1;
    char sign = arg1.high8 ^ arg2.high8;
    if (arg1 < 0)  {
       INVERT:
        arg1 = -arg1;
        if (!counter)
            return arg1;
    }
    if (arg2 < 0)
        arg2 = -arg2;
    goto ENTRY;
    do  {
        rm = rl( rm);
        W = rm - arg2;
        if (Carry)
            rm = W;
       ENTRY:
        arg1 = rl( arg1);
        counter = decsz(counter);
    } while (1);
    if (sign & 0x80)
        goto INVERT;
    return arg1;
}


int24 operator/ _divS24_8( int24 arg1, int8 arg2)
{
    uns8 rm = 0;
    char counter = sizeof(arg1)*8+1;
    char sign = arg1.high8 ^ arg2.high8;
    if (arg1 < 0)  {
       INVERT:
        arg1 = -arg1;
        if (!counter)
            return arg1;
    }
    if (arg2 < 0)
        arg2 = -arg2;
    goto ENTRY;
    do  {
        rm = rl( rm);
        W = rm - arg2;
        if (Carry)
            rm = W;
       ENTRY:
        arg1 = rl( arg1);
        counter = decsz(counter);
    } while (1);
    if (sign & 0x80)
        goto INVERT;
    return arg1;
}


int16 operator/ _divS16_16( int16 arg1, int16 arg2)
{
    uns16 rm = 0;
    char counter = sizeof(arg1)*8+1;
    char sign = arg1.high8 ^ arg2.high8;
    if (arg1 < 0)  {
       INVERT:
        arg1 = -arg1;
        if (!counter)
            return arg1;
    }
    if (arg2 < 0)
        arg2 = -arg2;
    goto ENTRY;
    do  {
        rm = rl( rm);
        W = rm.low8 - arg2.low8;
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto ENTRY;
        rm.high8 = W;
        rm.low8 -= arg2.low8;
        Carry = 1;
       ENTRY:
        arg1 = rl( arg1);
        counter = decsz(counter);
    } while (1);
    if (sign & 0x80)
        goto INVERT;
    return arg1;
}


int24 operator/ _divS24_16( int24 arg1, int16 arg2)
{
    uns16 rm = 0;
    char counter = sizeof(arg1)*8+1;
    char sign = arg1.high8 ^ arg2.high8;
    if (arg1 < 0)  {
       INVERT:
        arg1 = -arg1;
        if (!counter)
            return arg1;
    }
    if (arg2 < 0)
        arg2 = -arg2;
    goto ENTRY;
    do  {
        rm = rl( rm);
        W = rm.low8 - arg2.low8;
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto ENTRY;
        rm.high8 = W;
        rm.low8 -= arg2.low8;
        Carry = 1;
       ENTRY:
        arg1 = rl( arg1);
        counter = decsz(counter);
    } while (1);
    if (sign & 0x80)
        goto INVERT;
    return arg1;
}


int24 operator/ _divS24_24( int24 arg1, int24 arg2)
{
    uns24 rm = 0;
    char counter = sizeof(arg1)*8+1;
    char sign = arg1.high8 ^ arg2.high8;
    if (arg1 < 0)  {
       INVERT:
        arg1 = -arg1;
        if (!counter)
            return arg1;
    }
    if (arg2 < 0)
        arg2 = -arg2;
    goto ENTRY;
    do  {
        rm = rl( rm);
        W = rm.low8 - arg2.low8;
        genSubW( rm.mid8, arg2.mid8);
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto ENTRY;
        rm.high8 = W;
        rm.low8 -= arg2.low8;
        genSub( rm.mid8, arg2.mid8);
        Carry = 1;
       ENTRY:
        arg1 = rl( arg1);
        counter = decsz(counter);
    } while (1);
    if (sign & 0x80)
        goto INVERT;
    return arg1;
}



uns8 operator% _remU16_8( uns16 arg1, uns8 arg2)
{
    uns8 rm = 0;
    char counter = sizeof(arg1)*8;
    do  {
        arg1 = rl( arg1);
        rm = rl( rm);
        uns8 tmp = rl( tmp);
        W = rm - arg2;
        if (tmp&1)
            Carry = 1;
        if (Carry)
            rm = W;
        counter = decsz(counter);
    } while (1);
    return rm;
}


uns8 operator% _remU24_8( uns24 arg1, uns8 arg2)
{
    uns8 rm = 0;
    char counter = sizeof(arg1)*8;
    do  {
        arg1 = rl( arg1);
        rm = rl( rm);
        uns8 tmp = rl( tmp);
        W = rm - arg2;
        if (tmp&1)
            Carry = 1;
        if (Carry)
            rm = W;
        counter = decsz(counter);
    } while (1);
    return rm;
}


uns16 operator% _remU16_16( uns16 arg1, uns16 arg2)
{
    uns16 rm = 0;
    char counter = sizeof(arg1)*8;
    do  {
        arg1 = rl( arg1);
        rm = rl( rm);
        W = rm.low8 - arg2.low8;
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto NOSUB;
        rm.high8 = W;
        rm.low8 -= arg2.low8;
      NOSUB:
        counter = decsz(counter);
    } while (1);
    return rm;
}


uns16 operator% _remU24_16( uns24 arg1, uns16 arg2)
{
    uns16 rm = 0;
    char counter = sizeof(arg1)*8;
    do  {
        arg1 = rl( arg1);
        rm = rl( rm);
        if (Carry)
            goto SUBTRACT;
        W = rm.low8 - arg2.low8;
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto NOSUB;
      SUBTRACT:
        rm.low8 -= arg2.low8;
        genSub( rm.high8, arg2.high8);
      NOSUB:
        counter = decsz(counter);
    } while (1);
    return rm;
}


uns24 operator% _remU24_24( uns24 arg1, uns24 arg2)
{
    uns24 rm = 0;
    char counter = sizeof(arg1)*8;
    do  {
        arg1 = rl( arg1);
        rm = rl( rm);
        W = rm.low8 - arg2.low8;
        genSubW( rm.mid8, arg2.mid8);
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto NOSUB;
        rm.high8 = W;
        rm.low8 -= arg2.low8;
        genSub( rm.mid8, arg2.mid8);
      NOSUB:
        counter = decsz(counter);
    } while (1);
    return rm;
}


int8 operator% (int8 arg1, int8 arg2) @

int8 operator% _remS16_8( int16 arg1, int8 arg2)
{
    int8 rm = 0;
    char counter = 16;
    char sign = arg1.high8;
    if (arg1 < 0)
        arg1 = -arg1;
    if (arg2 < 0)
        arg2 = -arg2;
    do  {
        arg1 = rl( arg1);
        rm = rl( rm);
        W = rm - arg2;
        if (Carry)
            rm = W;
        counter = decsz(counter);
    } while (1);
    if (sign & 0x80)
        rm = -rm;
    return rm;
}


int8 operator% _remS24_8( int24 arg1, int8 arg2)
{
    int8 rm = 0;
    char counter = sizeof(arg1)*8;
    char sign = arg1.high8;
    if (arg1 < 0)
        arg1 = -arg1;
    if (arg2 < 0)
        arg2 = -arg2;
    do  {
        arg1 = rl( arg1);
        rm = rl( rm);
        W = rm - arg2;
        if (Carry)
            rm = W;
        counter = decsz(counter);
    } while (1);
    if (sign & 0x80)
        rm = -rm;
    return rm;
}


int16 operator% _remS16_16( int16 arg1, int16 arg2)
{
    int16 rm = 0;
    char counter = sizeof(arg1)*8;
    char sign = arg1.high8;
    if (arg1 < 0)
        arg1 = -arg1;
    if (arg2 < 0)
        arg2 = -arg2;
    do  {
        arg1 = rl( arg1);
        rm = rl( rm);
        W = rm.low8 - arg2.low8;
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto NOSUB;
        rm.high8 = W;
        rm.low8 -= arg2.low8;
      NOSUB:
        counter = decsz(counter);
    } while (1);
    if (sign & 0x80)
        rm = -rm;
    return rm;
}


int16 operator% _remS24_16( int24 arg1, int16 arg2)
{
    int16 rm = 0;
    char counter = sizeof(arg1)*8;
    char sign = arg1.high8;
    if (arg1 < 0)
        arg1 = -arg1;
    if (arg2 < 0)
        arg2 = -arg2;
    do  {
        arg1 = rl( arg1);
        rm = rl( rm);
        W = rm.low8 - arg2.low8;
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto NOSUB;
        rm.high8 = W;
        rm.low8 -= arg2.low8;
      NOSUB:
        counter = decsz(counter);
    } while (1);
    if (sign & 0x80)
        rm = -rm;
    return rm;
}


int24 operator% _remS24_24( int24 arg1, int24 arg2)
{
    int24 rm = 0;
    char counter = sizeof(arg1)*8;
    char sign = arg1.high8;
    if (arg1 < 0)
        arg1 = -arg1;
    if (arg2 < 0)
        arg2 = -arg2;
    do  {
        arg1 = rl( arg1);
        rm = rl( rm);
        W = rm.low8 - arg2.low8;
        genSubW( rm.mid8, arg2.mid8);
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto NOSUB;
        rm.high8 = W;
        rm.low8 -= arg2.low8;
        genSub( rm.mid8, arg2.mid8);
      NOSUB:
        counter = decsz(counter);
    } while (1);
    if (sign & 0x80)
        rm = -rm;
    return rm;
}

#undef genAdd
#undef genSub
#undef genAddW
#undef genSubW

#pragma library 0

