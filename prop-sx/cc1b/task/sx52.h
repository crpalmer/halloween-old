// HEADER FILE
#pragma chip SX52, core 12 sx, code 4096, ram 10 : 0x10F

/* RAM ADDRESS    BANKED  INDIRECT
   0x000 - 0x009  no      yes    : special purpose registers
   0x00A - 0x00F  no      yes    : unbanked RAM
   0x010 - 0x0FF  yes     yes    : RAM, bank 1 - 15 (16 byte each)
   0x100 - 0x10F  yes     no     : RAM, bank 0
*/

/* PREDEFINED:

  char W;
  char INDF, RTCC, PC, STATUS, FSR, RA, RB, RC, RD, RE;
  char INDF0, TMR0, PCL, PORTA, PORTB, PORTC, PORTD, PORTE; // optional
  char OPTION;

  /* STATUS : */   bit Carry, DC, Zero_, PD, TO, PA0, PA1, PA2;

  char MODE;
  char DDRA, PLP_A, LVL_A;
  char DDRB, PLP_B, LVL_B, ST_B, WKEN_B, WKED_B, WKPND_B, CMP_B;
  char DDRC, PLP_C, LVL_C, ST_C;
  char DDRD, PLP_D, LVL_D, ST_D;
  char DDRE, PLP_E, LVL_E, ST_E;
  char TRISA, TRISB, TRISC, TRISD, TRISE; // optional

  char T1CPL, T1CPH, T1R2CML, T1R2CMH, T1R1CML, T1R1CMH;
  char T1CNTB, T1CNTA,
  char T2CPL, T2CPH, T2R2CML, T2R2CMH, T2R1CML, T2R1CMH;
  char T2CNTB, T2CNTA;
*/
