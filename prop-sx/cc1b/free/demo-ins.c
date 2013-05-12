// GENERATING SINGLE INSTRUCTIONS USING C SYNTAX

#pragma chip SX28 // select device

char a, b, c, i;
bit flag, semi;

interrupt hx( void)
{
    if (semi)      // SNB   12.semi
        retiw();   // RETIW
}                  // RETI 

void s1( void)
{
    if (!flag)     // SB    12.flag
        retp();    // RETP 
}                  // RET  

page3 char s2( void)
{
    return 9;      // RETW  #9
}


bit b1, b2;

char funk( void)
{
    i = i & W;     // AND   i,W
    W = i & W;     // AND   W,i
    W = W & 53;    // AND   W,#53
    i = i ^ 255;   // NOT   i
    i = i | W;     // OR    i,W
    W = i | W;     // OR    W,i
    W = W | 23;    // OR    W,#23
    i = i ^ W;     // XOR   i,W
    W = i ^ W;     // XOR   W,i
    W = W ^ 12;    // XOR   W,#12
    i = i + W;     // ADD   i,W
    W = i + W;     // ADD   W,i
    i = 0;         // CLR   i
    W = 0;         // CLR   W
    clrwdt();      // CLR   !wdt
    i = i - 1;     // DEC   i
    i = decsz(i);  // DECSZ i
    i = i + 1;     // INC   i
    i = incsz(i);  // INCSZ i
    i = rl( i);    // RL    i
    i = rr( i);    // RR    i
    i = i - W;     // SUB   i,W
    i = swap( i);  // SWAP  i
    flag = 0;      // CLRB  12.flag
    btss(semi);    // SB    12.semi
    semi = 1;      // SETB  12.semi
    btsc(flag);    // SNB   12.flag
    i = W;         // MOV   i,W
    W = i;         // MOV   W,i
    W = i - W;     // MOV   W,i-W
    W = 45;        // MOV   W,#45
    W = i ^ 255;   // MOV   W,/i
    W = i - 1;     // MOV   W,--i
    W = i + 1;     // MOV   W,++i
    W = rl( i);    // MOV   W,<<i
    W = rr( i);    // MOV   W,>>i
    W = swap( i);  // MOV   W,<>i
    W = MODE;      // MOV   W,M
    W = decsz(i);  // MOVSZ W,--i
    W = incsz(i);  // MOVSZ W,++i
    MODE = W;      // MOV   M,W
    MODE = 6;      // MOV   M,#6
    controlA(W);   // MOV   !RA,W
    OPTION = W;    // MOV   !option,W
    i = i;         // TEST  i
    s1();          // CALL  s1

    s2();          // CALL  s2   + automatic page updating

    goto X;        // JMP   m001
    nop();         // NOP  

   #pragma optimize 0

    return W;      // RET  
    nop();         // NOP  
    retp();        // RETP 
    nop();         // NOP  
    retiw();       // RETIW
    nop();         // NOP  
    return 5;      // RETW  5
    bank(5);       // BANK  160  // manual bank updating is not required
    iread();       // IREAD
    sleep();       // SLEEP

   #pragma optimize 1

X:
    return 0;
}


void main(void)
{
    funk();

    MODE = 0x3;
    W = 0x10;
    iread();    // IREAD

   #pragma optimize 0
   #pragma updateBank 0

    // manual bank updating is not required
    bank(0);    // BANK  0
    bank(1);    // BANK  32
    bank(2);    // BANK  64
    bank(3);    // BANK  96
    bank(4);    // BANK  128
    bank(5);    // BANK  160
    bank(6);    // BANK  192
    bank(7);    // BANK  224

    // manual page updating is not required
    page(0);    // PAGE  0
    page(1);    // PAGE  512
    page(2);    // PAGE  1024
    page(3);    // PAGE  1536
    page(4);    // PAGE  2048
    page(5);    // PAGE  2560
    page(6);    // PAGE  3072
    page(7);    // PAGE  3584

   #pragma optimize 1
   #pragma updateBank 1

    nop();
}
