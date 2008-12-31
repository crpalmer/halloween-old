// DEFINING RAM VARIABLES

#pragma chip SX28 // select device


bit bt;              // a bit variable
char ga, c8;         // two 8 bit unsigned char variables

bank1 char a;        // stating the RAM bank directly

bank3 uns16 ug16;    // 16 bit unsigned integer in bank 3

uns16 table1[3];     // a table with 3 elements of 16 bit


struct {             // a structure of 2 char variables
  char b;
  char c;
} cgt;


typedef struct {     // a typedef
  int tab[2];
  struct {
    uns8 l1;
    uns8 l2;
  } vx;
} Txx;

bank2 Txx ax, bx[2];   // using the defined type


#pragma rambank 3     // defining the RAM bank for following definitions
char m1;
bank0 char uu1;
int24 m2;

#pragma rambank -     // unbanked RAM
char yy;

#pragma rambank 1
char pp;
shrBank int16 ir;     // unbanked RAM


const char *pro[3];  // a table in RAM of pointers to 'const' data


// OVERLAY DEFINITIONS
// variables overlaying another variable or table
bit  ov0 @ bt;  // full overlay
char ov1 @ c8;  // full overlay

int8 ov2 @ c8;  // full overlay, different type

bit  ov3 @ ug16.15;           // partial overlay
uns8 uu8 @ ug16.high8;        // partial overlay
uns16 uu16 @ table1[1].high8; // partial overlay
char table2[5] @ table1[0];   // partial overlay



void main(void)
{
    cgt.c = 0;

    ov0 = 0;
    ov1 = 0;
    ov2 = 0;
    ov3 = 0;

    ug16 = 0;
    uu16 = 0;

    uu16 = 0;
    table1[2] = 0;
    table1[0] = 0;
    table2[0] = 0;

    pro[0] = "Hello";
    pro[1] = "News";

    char i = *pro[0];
}


