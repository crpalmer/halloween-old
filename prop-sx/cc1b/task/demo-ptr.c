// POINTERS AND INDEXED TABLES

#pragma chip SX28 // select device


bank0 char tab1[10];
bank1 char tab2[10];

bank2 uns16 tu2[4];
bank3 uns16 tu3[4];
uns16 *px;


void pointers( void)
{
    // p1 is 8 bit wide
    char *p1 = tab1;
    char s = 0;
    do
       s += *p1 & 3;
    while (++p1 < &tab1[10]);

    px = tu2;
    px = &tu3[9];
    uns16 e = *px;


    // low level pointer access
    FSR = &tab1[1];
    char a = 10-1;
    do  {
        INDF = 0;
    } while (--a > 0);

    char *px, i;
    i = *px;

}


typedef struct {
  uns16 ab;
  struct {
    uns8 l1;
    uns8 l2;
  } vx;
} Txx;

bank4 Txx ax, bx[2], *pxx;

void sub( void)
{
    pxx = &ax;
    ax.ab = 1000;
    pxx->vx.l1 = 3;

    pxx = &bx[0];
    pxx->ab = 0;
    pxx ++;
    pxx->vx.l2 = 0;
}


bank0 uns16 *lp;

uns16 sub2( void)
{
    bank1 uns16 dx;

    lp ++;
    return dx;
}

uns16 sub3( void)
{
    return *lp;
}



void main(void)
{
    pointers();
    sub();

    bank2 uns16 z = sub2();
    z += sub3();
}


