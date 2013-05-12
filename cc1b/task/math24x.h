
#pragma library 1
/*
fixed16_8 operator* _xmul16_8( fixed16_8 arg1, fixed16_8 arg2);
fixedU16_8 operator* _xmulU16_8( fixedU16_8 arg1, fixedU16_8 arg2);
fixed16_8 operator/ _xdiv16_8( fixed16_8 arg1, fixed16_8 arg2);
fixedU16_8 operator/ _xdivU16_8( fixedU16_8 arg1, fixedU16_8 arg2);
fixed8_16 operator* _xmul8_16( fixed8_16 arg1, fixed8_16 arg2);
fixedU8_16 operator* _xmulU8_16( fixedU8_16 arg1, fixedU8_16 arg2);
fixed8_16 operator/ _xdiv8_16( fixed8_16 arg1, fixed8_16 arg2);
fixedU8_16 operator/ _xdivU8_16( fixedU8_16 arg1, fixedU8_16 arg2);
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



fixed16_8 operator* _xmul16_8( fixed16_8 arg1, fixed16_8 arg2)
{
    fixed16_8 rval;
    char counter = sizeof(arg1)*8;
    char sign = arg1.high8 ^ arg2.high8;
    if (arg1 < 0)
        arg1 = -arg1;
    if (arg2 < 0)
        arg2 = -arg2;
    char rvalL = 0;
    do  {
        Carry = 0;
        rvalL = rl( rvalL);
        rval = rl( rval);
        arg1 = rl( arg1);
        if (Carry)  {
            rvalL += arg2.low8;
            genAdd( rval.low8, arg2.mid8);
            genAdd( rval.mid8, arg2.high8);
            if (Carry)
                rval.high8++;
        }
        counter = decsz(counter);
    } while (1);
    if (sign & 0x80)
        rval = -rval;
    return rval;
}


fixedU16_8 operator* _xmulU16_8( fixedU16_8 arg1, fixedU16_8 arg2)
{
    fixedU16_8 rval;
    char counter = sizeof(arg1)*8;
    char rvalL = 0;
    do  {
        Carry = 0;
        rvalL = rl( rvalL);
        rval = rl( rval);
        arg1 = rl( arg1);
        if (Carry)  {
            rvalL += arg2.low8;
            genAdd( rval.low8, arg2.mid8);
            genAdd( rval.mid8, arg2.high8);
            if (Carry)
                rval.high8++;
        }
        counter = decsz(counter);
    } while (1);
    return rval;
}


fixed16_8 operator/ _xdiv16_8( fixed16_8 arg1, fixed16_8 arg2)
{
    fixedU16_8 rm = 0;
    char counter = sizeof(arg1)*8+8+1;
    char c2 = sizeof(arg1)*8 + 1;
    char sign = arg1.high8 ^ arg2.high8;
    if (arg1 < 0)  {
       INVERT:
        arg1 = -arg1;
        if (c2 & 0x80)
            goto EXIT;
    }
    if (arg2 < 0)
        arg2 = -arg2;
    goto ENTRY;
    do  {
        c2 = decsz(c2);
        btsc(c2.7);
        Carry = 0;
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
   EXIT:
    return arg1;
}



fixedU16_8 operator/ _xdivU16_8( fixedU16_8 arg1, fixedU16_8 arg2)
{
    fixedU16_8 rm = 0;
    char counter = sizeof(arg1)*8+8+1;
    char c2 = sizeof(arg1)*8 + 1;
    goto ENTRY;
    do  {
        c2 = decsz(c2);
        btsc(c2.7);
        Carry = 0;
        rm = rl( rm);
        if (Carry)
            goto SUBTRACT;
        W = rm.low8 - arg2.low8;
        genSubW( rm.mid8, arg2.mid8);
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto ENTRY;
       SUBTRACT:
        rm.low8 -= arg2.low8;
        genSub( rm.mid8, arg2.mid8);
        genSub( rm.high8, arg2.high8);
        Carry = 1;
       ENTRY:
        arg1 = rl( arg1);
        counter = decsz(counter);
    } while (1);
    return arg1;
}




fixed8_16 operator* _xmul8_16( fixed8_16 arg1, fixed8_16 arg2)
{
    fixed8_16 rval;
    char counter = sizeof(arg2)*8;
    uns8 rvalH = 0;
    rval.high16 = 0;
    char sign = arg1.high8 ^ arg2.high8;
    if (arg1 < 0)
        arg1 = -arg1;
    if (arg2 < 0)
        arg2 = -arg2;
    do  {
        arg2 = rr( arg2);
        if (Carry)  {
            rval.mid8 += arg1.low8;
            genAdd( rval.high8, arg1.mid8);
            genAdd( rvalH, arg1.high8);
        }
        rvalH = rr( rvalH);
        rval = rr( rval);
        counter = decsz(counter);
    } while (1);
    if (sign & 0x80)
        rval = -rval;
    return rval;
}


fixedU8_16 operator* _xmulU8_16( fixedU8_16 arg1, fixedU8_16 arg2)
{
    fixedU8_16 rval;
    char counter = sizeof(arg2)*8;
    uns8 rvalH = 0;
    rval.high16 = 0;
    do  {
        arg2 = rr( arg2);
        if (Carry)  {
            rval.mid8 += arg1.low8;
            genAdd( rval.high8, arg1.mid8);
            genAdd( rvalH, arg1.high8);
        }
        rvalH = rr( rvalH);
        rval = rr( rval);
        counter = decsz(counter);
    } while (1);
    return rval;
}


fixed8_16 operator/ _xdiv8_16( fixed8_16 arg1, fixed8_16 arg2)
{
    fixedU8_16 rm = 0;
    char counter = sizeof(arg1)*8+16+1;
    char c2 = sizeof(arg1)*8 + 1;
    char sign = arg1.high8 ^ arg2.high8;
    if (arg1 < 0)  {
       INVERT:
        arg1 = -arg1;
        if (c2 & 0x80)
            goto EXIT;
    }
    if (arg2 < 0)
        arg2 = -arg2;
    goto ENTRY;
    do  {
        c2 = decsz(c2);
        btsc(c2.7);
        Carry = 0;
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
   EXIT:
    return arg1;
}



fixedU8_16 operator/ _xdivU8_16( fixedU8_16 arg1, fixedU8_16 arg2)
{
    fixedU8_16 rm = 0;
    char counter = sizeof(arg1)*8+16+1;
    char c2 = sizeof(arg1)*8 + 1;
    goto ENTRY;
    do  {
        c2 = decsz(c2);
        btsc(c2.7);
        Carry = 0;
        rm = rl( rm);
        if (Carry)
            goto SUBTRACT;
        W = rm.low8 - arg2.low8;
        genSubW( rm.mid8, arg2.mid8);
        genSubW( rm.high8, arg2.high8);
        if (!Carry)
            goto ENTRY;
       SUBTRACT:
        rm.low8 -= arg2.low8;
        genSub( rm.mid8, arg2.mid8);
        genSub( rm.high8, arg2.high8);
        Carry = 1;
       ENTRY:
        arg1 = rl( arg1);
        counter = decsz(counter);
    } while (1);
    return arg1;
}

#undef genAdd
#undef genSub
#undef genAddW
#undef genSubW

#pragma library 0
