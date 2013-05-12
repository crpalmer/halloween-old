//pdl_code.h tag: 1073823776
#define PDL_CODE  1073823776
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
int getR_IND() {
return(base.IND);
}
void setR_IND(int a1) {
base.IND = a1;
}
int getR_RTCC() {
return(base.RTCC);
}
void setR_RTCC(int a1) {
base.RTCC = a1;
}
int getR_WREG() {
return(base.WREG);
}
void setR_WREG(int a1) {
base.WREG = a1;
}
void incR_PC() {
base.PC++;
}
int getR_PC() {
return(base.PC);
}
void setR_PC(int a1) {
base.PC = a1;
}
int getR_STATUS() {
return(base.STATUS);
}
void setR_STATUS(int a1) {
base.STATUS = a1;
}
int getR_PA2() {
return (( base.STATUS & 128) ? 1 : 0);
}
void setR_PA2(int a1) {
(a1) ? base.STATUS |= 128 : base.STATUS &=~128;
}
int getR_PA1() {
return (( base.STATUS & 64) ? 1 : 0);
}
void setR_PA1(int a1) {
(a1) ? base.STATUS |= 64 : base.STATUS &=~64;
}
int getR_PA0() {
return (( base.STATUS & 32) ? 1 : 0);
}
void setR_PA0(int a1) {
(a1) ? base.STATUS |= 32 : base.STATUS &=~32;
}
int getR_TO() {
return (( base.STATUS & 16) ? 1 : 0);
}
void setR_TO(int a1) {
(a1) ? base.STATUS |= 16 : base.STATUS &=~16;
}
int getR_PD() {
return (( base.STATUS & 8) ? 1 : 0);
}
void setR_PD(int a1) {
(a1) ? base.STATUS |= 8 : base.STATUS &=~8;
}
int getR_Z() {
return (( base.STATUS & 4) ? 1 : 0);
}
void setR_Z(int a1) {
(a1) ? base.STATUS |= 4 : base.STATUS &=~4;
}
int getR_DC() {
return (( base.STATUS & 2) ? 1 : 0);
}
void setR_DC(int a1) {
(a1) ? base.STATUS |= 2 : base.STATUS &=~2;
}
int getR_C() {
return (( base.STATUS & 1) ? 1 : 0);
}
void setR_C(int a1) {
(a1) ? base.STATUS |= 1 : base.STATUS &=~1;
}
int getR_FSR() {
return(base.FSR);
}
void setR_FSR(int a1) {
base.FSR = a1;
}
int getR_RA() {
PDLM_TRIGGER_READ_RA
return(base.RA);
}
void setR_RA(int a1) {
base.RA = a1;
PDLM_TRIGGER_WRITE_RA
}
int getR_RB() {
PDLM_TRIGGER_READ_RB
return(base.RB);
}
void setR_RB(int a1) {
base.RB = a1;
PDLM_TRIGGER_WRITE_RB
}
int getR_RC() {
PDLM_TRIGGER_READ_RC
return(base.RC);
}
void setR_RC(int a1) {
base.RC = a1;
PDLM_TRIGGER_WRITE_RC
}
int getR_TRIS_RA() {
return(base.TRIS_RA);
}
void setR_TRIS_RA(int a1) {
base.TRIS_RA = a1;
}
int getR_TRIS_RB() {
return(base.TRIS_RB);
}
void setR_TRIS_RB(int a1) {
base.TRIS_RB = a1;
}
int getR_TRIS_RC() {
return(base.TRIS_RC);
}
void setR_TRIS_RC(int a1) {
base.TRIS_RC = a1;
}
int getR_PLP_RA() {
return(base.PLP_RA);
}
void setR_PLP_RA(int a1) {
base.PLP_RA = a1;
}
int getR_PLP_RB() {
return(base.PLP_RB);
}
void setR_PLP_RB(int a1) {
base.PLP_RB = a1;
}
int getR_PLP_RC() {
return(base.PLP_RC);
}
void setR_PLP_RC(int a1) {
base.PLP_RC = a1;
}
int getR_LVL_RA() {
return(base.LVL_RA);
}
void setR_LVL_RA(int a1) {
base.LVL_RA = a1;
}
int getR_LVL_RB() {
return(base.LVL_RB);
}
void setR_LVL_RB(int a1) {
base.LVL_RB = a1;
}
int getR_LVL_RC() {
return(base.LVL_RC);
}
void setR_LVL_RC(int a1) {
base.LVL_RC = a1;
}
int getR_ST_RB() {
return(base.ST_RB);
}
void setR_ST_RB(int a1) {
base.ST_RB = a1;
}
int getR_ST_RC() {
return(base.ST_RC);
}
void setR_ST_RC(int a1) {
base.ST_RC = a1;
}
int getR_WKEN_RB() {
return(base.WKEN_RB);
}
void setR_WKEN_RB(int a1) {
base.WKEN_RB = a1;
}
int getR_WKED_RB() {
return(base.WKED_RB);
}
void setR_WKED_RB(int a1) {
base.WKED_RB = a1;
}
int getR_WKPND_RB() {
return(base.WKPND_RB);
}
void setR_WKPND_RB(int a1) {
base.WKPND_RB = a1;
}
int getR_CMP_RB() {
return(base.CMP_RB);
}
void setR_CMP_RB(int a1) {
base.CMP_RB = a1;
}
int getR_MREG() {
return(base.MREG);
}
void setR_MREG(int a1) {
base.MREG = a1;
}
int getR_OPTION() {
return(base.OPTION);
}
void setR_OPTION(int a1) {
base.OPTION = a1;
}
int getR_RTW() {
return (( base.OPTION & 128) ? 1 : 0);
}
void setR_RTW(int a1) {
(a1) ? base.OPTION |= 128 : base.OPTION &=~128;
}
int getR_RTI() {
return (( base.OPTION & 64) ? 1 : 0);
}
void setR_RTI(int a1) {
(a1) ? base.OPTION |= 64 : base.OPTION &=~64;
}
int getR_RTS() {
return (( base.OPTION & 32) ? 1 : 0);
}
void setR_RTS(int a1) {
(a1) ? base.OPTION |= 32 : base.OPTION &=~32;
}
int getR_RTE() {
return (( base.OPTION & 16) ? 1 : 0);
}
void setR_RTE(int a1) {
(a1) ? base.OPTION |= 16 : base.OPTION &=~16;
}
int getR_PSA() {
return (( base.OPTION & 8) ? 1 : 0);
}
void setR_PSA(int a1) {
(a1) ? base.OPTION |= 8 : base.OPTION &=~8;
}
int getR_PS2() {
return (( base.OPTION & 4) ? 1 : 0);
}
void setR_PS2(int a1) {
(a1) ? base.OPTION |= 4 : base.OPTION &=~4;
}
int getR_PS1() {
return (( base.OPTION & 2) ? 1 : 0);
}
void setR_PS1(int a1) {
(a1) ? base.OPTION |= 2 : base.OPTION &=~2;
}
int getR_PS0() {
return (( base.OPTION & 1) ? 1 : 0);
}
void setR_PS0(int a1) {
(a1) ? base.OPTION |= 1 : base.OPTION &=~1;
}
void andfrw ( int a1) { incR_PC(); PDLM_ANDFRW }
void andwfr ( int a1) { incR_PC(); PDLM_ANDWFR }
void andwlit ( int a1) { incR_PC(); PDLM_ANDWLIT }
void notfr ( int a1) { incR_PC(); PDLM_NOTFR }
void orfrw ( int a1) { incR_PC(); PDLM_ORFRW }
void orwfr ( int a1) { incR_PC(); PDLM_ORWFR }
void orwlit ( int a1) { incR_PC(); PDLM_ORWLIT }
void xorfrw ( int a1) { incR_PC(); PDLM_XORFRW }
void xorwfr ( int a1) { incR_PC(); PDLM_XORWFR }
void xorwlit ( int a1) { incR_PC(); PDLM_XORWLIT }
void addfrw ( int a1) { incR_PC(); PDLM_ADDFRW }
void addwfr ( int a1) { incR_PC(); PDLM_ADDWFR }
void clrfr ( int a1) { incR_PC(); PDLM_CLRFR }
void clrw () { incR_PC(); PDLM_CLRW }
void clrwdt () { incR_PC(); PDLM_CLRWDT }
void decfr ( int a1) { incR_PC(); PDLM_DECFR }
void decszfr ( int a1) { incR_PC(); PDLM_DECSZFR }
void incfr ( int a1) { incR_PC(); PDLM_INCFR }
void incszfr ( int a1) { incR_PC(); PDLM_INCSZFR }
void subfrw ( int a1) { incR_PC(); PDLM_SUBFRW }
void swapfr ( int a1) { incR_PC(); PDLM_SWAPFR }
void clrbfrbit ( int a1,  int a2) { incR_PC(); PDLM_CLRBFRBIT }
void sbfrbit ( int a1,  int a2) { incR_PC(); PDLM_SBFRBIT }
void setbfrbit ( int a1,  int a2) { incR_PC(); PDLM_SETBFRBIT }
void snbfrbit ( int a1,  int a2) { incR_PC(); PDLM_SNBFRBIT }
void movwfr ( int a1) { incR_PC(); PDLM_MOVWFR }
void movwfrmw ( int a1) { incR_PC(); PDLM_MOVWFRMW }
void movwlit ( int a1) { incR_PC(); PDLM_MOVWLIT }
void movwcfr ( int a1) { incR_PC(); PDLM_MOVWCFR }
void movwmmfr ( int a1) { incR_PC(); PDLM_MOVWMMFR }
void movwppfr ( int a1) { incR_PC(); PDLM_MOVWPPFR }
void movwswfr ( int a1) { incR_PC(); PDLM_MOVWSWFR }
void movwm () { incR_PC(); PDLM_MOVWM }
void movszwmmfr ( int a1) { incR_PC(); PDLM_MOVSZWMMFR }
void movszwppfr ( int a1) { incR_PC(); PDLM_MOVSZWPPFR }
void movmw () { incR_PC(); PDLM_MOVMW }
void movmlit ( int a1) { incR_PC(); PDLM_MOVMLIT }
void moveraw () { incR_PC(); PDLM_MOVERAW }
void moverbw () { incR_PC(); PDLM_MOVERBW }
void movercw () { incR_PC(); PDLM_MOVERCW }
void moveoptionw () { incR_PC(); PDLM_MOVEOPTIONW }
void testfr ( int a1) { incR_PC(); PDLM_TESTFR }
void nop () { incR_PC(); }
void bankaddr8 ( int a1) { incR_PC(); PDLM_BANKADDR8 }
void pageaddr12 ( int a1) { incR_PC(); PDLM_PAGEADDR12 }
