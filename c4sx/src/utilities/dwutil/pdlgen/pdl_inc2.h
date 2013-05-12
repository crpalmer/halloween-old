//pdl_inc2.h tag: 1073823776
#define PDL_INC2  1073823776
/* copyright.txt

ESX28 is copyright 2005-2006 Bob Senser

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the 
"Software"), to deal in the Software without restrictions, including
without limitation the rights to use, copy, modify, publish,
sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be 
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL BOB SENSER BE LIABLE FOR ANY CLAIM, DAMAGES, OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE
ARISING FROM, OUT OF OR IN CONNECTION WITH THIS THE SOFTWARE OR THE
USE OR OTHER DEALINGS OF THE SOFTWARE.

*/ 
struct cxt {
unsigned char IND;
unsigned char RTCC;
unsigned char WREG;
unsigned int PC;
unsigned char STATUS;
unsigned char FSR;
unsigned char RA;
unsigned char RB;
unsigned char RC;
unsigned char TRIS_RA;
unsigned char TRIS_RB;
unsigned char TRIS_RC;
unsigned char PLP_RA;
unsigned char PLP_RB;
unsigned char PLP_RC;
unsigned char LVL_RA;
unsigned char LVL_RB;
unsigned char LVL_RC;
unsigned char ST_RB;
unsigned char ST_RC;
unsigned char WKEN_RB;
unsigned char WKED_RB;
unsigned char WKPND_RB;
unsigned char CMP_RB;
unsigned char MREG;
unsigned char OPTION;
};

static const int pgmmemory = 2024;
static const int pgmmemsize = 12;
static const int datamemory = 136;
static const int datamemsize = 8;

PDL_TEMP(full, 12, 0, 0);
PDL_TEMP(fr, 7, 5, 0);
PDL_TEMP(frbit, 4, 3, 5);
PDL_TEMP(lit, 4, 8, 0);
PDL_TEMP(lit4, 8, 4, 0);
PDL_TEMP(addr3, 9, 3, 0);
PDL_TEMP(addr8, 4, 8, 0);
PDL_TEMP(addr9, 3, 9, 0);

struct pdl_inst instructions[] = {
PDL_INST(fr, 0x160, andfrw, 0x1, 0x1, "AND $,W"),
PDL_INST(fr, 0x140, andwfr, 0x1, 0x1, "AND W,$"),
PDL_INST(lit, 0xe00, andwlit, 0x1, 0x1, "AND W,#$"),
PDL_INST(fr, 0x260, notfr, 0x1, 0x1, "NOT $"),
PDL_INST(fr, 0x120, orfrw, 0x1, 0x1, "OR $,W"),
PDL_INST(fr, 0x100, orwfr, 0x1, 0x1, "OR W,$"),
PDL_INST(lit, 0xd00, orwlit, 0x1, 0x1, "OR W,#$"),
PDL_INST(fr, 0x1a0, xorfrw, 0x1, 0x1, "XOR $,W"),
PDL_INST(fr, 0x180, xorwfr, 0x1, 0x1, "XOR W,$"),
PDL_INST(lit, 0xf00, xorwlit, 0x1, 0x1, "XOR W,#$"),
PDL_INST(fr, 0x1e0, addfrw, 0x1, 0x1, "ADD $,W"),
PDL_INST(fr, 0x1c0, addwfr, 0x1, 0x1, "ADD W,$"),
PDL_INST(fr, 0x60, clrfr, 0x1, 0x1, "CLR $"),
PDL_INST(full, 0x40, clrw, 0x1, 0x1, "CLR W"),
PDL_INST(full, 0x4, clrwdt, 0x1, 0x1, "CLR !WDT"),
PDL_INST(fr, 0xe0, decfr, 0x1, 0x1, "DEC $"),
PDL_INST(fr, 0x2e0, decszfr, 0x1, 0x2, "DECSZ $"),
PDL_INST(fr, 0x2a0, incfr, 0x1, 0x1, "INC $"),
PDL_INST(fr, 0x3e0, incszfr, 0x1, 0x2, "INCSZ $"),
PDL_INST(fr, 0x360, rlfr, 0x1, 0x1, "RL $"),
PDL_INST(fr, 0x320, rrfr, 0x1, 0x1, "RR $"),
PDL_INST(fr, 0xa0, subfrw, 0x1, 0x1, "SUB $,W"),
PDL_INST(fr, 0x3a0, swapfr, 0x1, 0x1, "SWAP $"),
PDL_INST(frbit, 0x400, clrbfrbit, 0x1, 0x1, "CLRB $.$"),
PDL_INST(frbit, 0x700, sbfrbit, 0x1, 0x2, "SB $.$"),
PDL_INST(frbit, 0x500, setbfrbit, 0x1, 0x1, "SETB $.$"),
PDL_INST(frbit, 0x600, snbfrbit, 0x1, 0x2, "SNB $.$"),
PDL_INST(fr, 0x20, movfrw, 0x1, 0x1, "MOV $,W"),
PDL_INST(fr, 0x200, movwfr, 0x1, 0x1, "MOV W,$"),
PDL_INST(fr, 0x80, movwfrmw, 0x1, 0x1, "MOV W,$-W"),
PDL_INST(lit, 0xc00, movwlit, 0x1, 0x1, "MOV W,#$"),
PDL_INST(fr, 0x240, movwcfr, 0x1, 0x1, "MOV W,/$"),
PDL_INST(fr, 0xc0, movwmmfr, 0x1, 0x1, "MOV W,--$"),
PDL_INST(fr, 0x280, movwppfr, 0x1, 0x1, "MOV W,++$"),
PDL_INST(fr, 0x340, movwllfr, 0x1, 0x1, "MOV W,<<$"),
PDL_INST(fr, 0x300, movwlrfr, 0x1, 0x1, "MOV W,>>$"),
PDL_INST(fr, 0x380, movwswfr, 0x1, 0x1, "MOV W,<>$"),
PDL_INST(full, 0x42, movwm, 0x1, 0x1, "MOV W,M"),
PDL_INST(fr, 0x2c0, movszwmmfr, 0x1, 0x2, "MOVSZ W,--$"),
PDL_INST(fr, 0x3c0, movszwppfr, 0x1, 0x2, "MOVSZ W,++$"),
PDL_INST(full, 0x43, movmw, 0x1, 0x1, "MOV M,W"),
PDL_INST(lit4, 0x50, movmlit, 0x1, 0x1, "MOV M,#$"),
PDL_INST(full, 0x5, moveraw, 0x1, 0x1, "MOV !RA,W"),
PDL_INST(full, 0x6, moverbw, 0x1, 0x1, "MOV !RB,W"),
PDL_INST(full, 0x7, movercw, 0x1, 0x1, "MOV !RC,W"),
PDL_INST(full, 0x2, moveoptionw, 0x1, 0x1, "MOV !OPTION,W"),
PDL_INST(fr, 0x220, testfr, 0x1, 0x1, "TEST $"),
PDL_INST(addr8, 0x900, calladdr8, 0x3, 0x3, "CALL $"),
PDL_INST(addr9, 0xa00, jmpaddr9, 0x3, 0x3, "JMP $"),
PDL_INST(full, 0x0, nop, 0x1, 0x1, "NOP"),
PDL_INST(full, 0xc, ret, 0x3, 0x3, "RET"),
PDL_INST(full, 0xd, retp, 0x3, 0x3, "RETP"),
PDL_INST(full, 0xe, reti, 0x3, 0x3, "RETI"),
PDL_INST(full, 0xf, retiw, 0x3, 0x3, "RETIW"),
PDL_INST(lit, 0x800, retwlit, 0x3, 0x3, "RETW $"),
PDL_INST(addr3, 0x18, bankaddr8, 0x1, 0x1, "BANK $"),
PDL_INST(full, 0x41, iread, 0x4, 0x4, "IREAD"),
PDL_INST(addr3, 0x10, pageaddr12, 0x1, 0x1, "PAGE $"),
PDL_INST(full, 0x3, sleep, 0x1, 0x1, "SLEEP"),
 { NULL, 0, } };

