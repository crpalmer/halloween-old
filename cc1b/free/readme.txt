
       ======================================================
       README file for the CC1B C compiler, FREE/DEMO edition
       ======================================================

Welcome to the CC1B C compiler for the UBICOM SX devices. The FREE
edition can generate up to 4096 words of code with reduced
optimization. The FREE edition supports 8 and 16 bit integers, and
24 bit floating point.

The alternative DEMO edition allows a large program to be compiled
using CC1B (up to 4k instructions). However, NO hex file is
generated and the assembly file is NOT complete.

A device specific header file need to be defined by:
  1) A command line option : -pSX20
  2) Or a pragma statement : #pragma chip SX20
  3) Or an include statement : #include "sx20.h"


RESTRICTIONS
------------

  The free edition can be used to generate code for all prototype
  and non-commercial systems without restrictions. Permission is
  given to use the generated code in ONE commercial system.

  No restriction applies to source code written by the user.


COPYRIGHT NOTICE
----------------

  Copyright (c) B Knudsen Data, Trondheim, Norway, 2002 - 2008.

  www.bknd.com


  The CC1B compiler is protected by Norwegian copyright laws and
  thus by corresponding copyright laws agreed internationally by
  mutual consent. Modification of the compiler is strongly
  prohibited. All rights reserved.

  B Knudsen Data assumes no responsibility for errors or defects in
  the compiler or in the other supplied files. This also applies to
  problems caused by such errors.

  Permission is required to distribute the FREE/DEMO edition.


MAIN LIMITATIONS
----------------

  1) No specific CC1B PDF manual yet available.

  2) Access of 'const' data use currently RETLW instruction


SUPPLIED FILES
--------------

  cc1b.exe      : CC1B compiler, FREE/TEST edition

  sx.txt        : The Ubicom SX compiler support
  install.txt   : Installation guide
  int-sx.c      : Interrupt support
  math.txt      : Math library support

  demo.c        : sample syntax including interrupt
  demo-var.c    : defining RAM variables
  demo-mat.c (3): integer math operations
  demo-fpm.c    : floating point math
  demo-fxm.c (3): fixed point math
  demo-rom.c    : const data and DW
  demo-ptr.c    : indexed tables and pointers
  demo-ins.c    : generating single instructions using C code

  hexcodes.h    : direct coded instructions

  SX18.H        : header file, device SX18AC
  SX20.H        : header file, device SX20AC
  SX28.H        : header file, device SX28AC
  SX48.H        : header file, device SX48BD
  SX52.H        : header file, device SX52BD

  readme.txt    : This file

  math16.h      : 8-16 bit math library, signed and unsigned
  math16m.h     : 8-16 bit multiply, speed, signed and unsigned
  math24.h   (3): 8-24 bit math library, signed and unsigned
  math24m.h  (3): 8-24 bit multiply, speed, signed and unsigned
  math32.h   (1): 8-32 bit math library, signed and unsigned
  math32m.h  (1): 8-32 bit multiply, speed, signed and unsigned
  math16x.h  (3): 16 bit fixed point library
  math24x.h  (3): 24 bit fixed point library
  math32x.h  (1): 32 bit fixed point library
  math16f.h  (1): 16 bit floating point library
  math24f.h  (2): 24 bit floating point library
  math32f.h  (4): 32 bit floating point library
  math24lb.h (2): 24 bit floating point math functions (log,sqrt,cos,..)
  math32lb.h (4): 32 bit floating point math functions (log,sqrt,cos,..)

  (1) : Not available on the STANDARD, DEMO and FREE editions
  (2) : Not available on the 250 WORD DEMO edition
  (3) : Not available on the FREE edition
  (4) : Not available on the DEMO and FREE editions


SPECIAL NOTES
-------------

  Note that CC1B requires that libraries are included to make
  library calls. Please refer to the following 'Programming
  recommendations that enables compact code' for details.


PROGRAMMING RECOMMENDATIONS THAT ENABLES COMPACT CODE
-----------------------------------------------------

  1. If you are going to use multiplication and division, take a
     look at 'math.txt'.

  2. CC1B will update the bank bits automatically, but variables
     have to be placed on the different RAM banks by using pragma
     statements. It therefore recommended to locate all global
     variables belonging to the same RAM bank together (in header
     files).

        #pragma rambank 0
         char a;  // variables to be located in bank 0
        #pragma rambank 1
         // variables to be located in bank 1
         ..

     Some notes when locating variables:
     a) Locate variables which are close related to each other in
        the same bank.
     b) Locate all variables accessed in the same function in the
        same bank if possible.
     c) Use as few banks as possible. Fill bank 0 first, then bank 1,
        etc.


  3. It is most efficient to put all local variables in mapped RAM.
     This is easiest done by moving all global variables to the
     beginning of the program (or to header files). Then change the
     rambank to mapped RAM before compiling the functions:

         // all global variables
         #pragma rambank -    // mapped RAM
         // all functions


  4. It is recommended to define all functions before they are
     called. That is, make sure that the function code is compiled
     before the first function call is made. This allows better
     optimization of the bank selection bits.


  5. The functions have to be located on the different code pages by
     pragma instructions. Calling a function located in another
     codepage requires extra page instructions. CC1B will do this
     automatically. However, moving a function to another codepage
     may reduce the number of instructions required. Some of the
     PICmicro devices have 4 code pages.

     It is recommended to divide the program into modules. Use a
     small main file and include all header and program modules in
     the beginning of the main file. Pragma instructions will locate
     the functions. The generated function call structure file
     (*.fcs) contains information that will be helpful in finding
     optimal combinations. Please refer to the manual or the *.txt
     files for details.

        #pragma codepage 2  // locate following functions
        #pragma location 2  // locate functions prototypes also


  6. Computed goto's is very efficient for implementing a multi-
     selection (instead of using the switch or multiple if
     statements). Many examples of how to do this is found in
     'Application Notes' in the User's Manual.


  7. The smallest possible variables should be used. Variables of 1
     and 8 bit are efficient. Also, some operations on signed
     variables requires more code compared to using unsigned
     variables.



WRITING CODE THAT CAN BE COMPILED BY CC1B
-----------------------------------------

  CC1B is not a full C compiler. There are restrictions to be aware
  of. The main problem is that CC1B has limited ability to allocate
  temporary variables. Because of this, complex statements often
  have to be rewritten using simpler statements. This is most
  notisable when using variables of 16 bit or larger. There is an
  advantage of such rewriting. Often the generated hex code gets
  more compact.


  MODIFIED SAMPLE:

    uns16 array[5], temp, temp2;
    char j, k;

    //if(array[j] > array[j+1])
    //{
    //    temp = array[j];
    //    array[j] = array[j+1];
    //    array[j+1] = temp;
    //}

    temp = array[j];
    k = j+1;
    temp2 = array[k];
    if(temp > temp2)
    {
        array[j] = temp2;
        array[k] = temp;
    }


  MODIFIED SAMPLE:
    //if (x++ < 20) { /* .. */ }
    if (x < 20) { x++;  /* .. */ }
    if (++x < 20) { /* .. */ }  // ok


  MODIFIED SAMPLE:

    unsigned long checksum;
    unsigned char c;
    unsigned long calc(unsigned long c);
    //checksum = calc(c^checksum) ^ (checksum/256);
    unsigned long tmp = calc(c^checksum);
    checksum /= 256;
    checksum ^= tmp;


  MODIFIED SAMPLE:

    //rx_str[ptr2] = rx_str[ptr1];
    char tmp = rx_str[ptr1];
    rx_str[ptr2] = tmp;


  MODIFIED SAMPLE:

    // if (c = TestOptions(0x00c6)) { /* .. */ }
    c = TestOptions(0x00c6);
    if (c) { /* .. */ }


  MODIFIED SAMPLE:

    //if (!((1<<(rx_str[ptr1]-1))&option)) { /* .. */ }
    unsigned char xtmp = rx_str[ptr1] - 1;
    unsigned long tmp = (unsigned long)1 << xtmp;
    tmp &= option;
    if (!tmp) { /* .. */ }


  MODIFIED SAMPLE:

    unsigned long packet;
    //packet = rx_str[2]*256 + rx_str[3];
    packet.high8 = rx_str[2];
    packet.low8 = rx_str[3];


  MODIFIED SAMPLE:

    unsigned long length;
    //length = 0x45 + 0x88 + 0x40 + 127 + addr1 + addr3 + str[1] + str[3];
    length = (unsigned long) 0x45 + 0x88 + 0x40 + 127 + addr1;
    length += addr3;
    length += str[1];
    length += str[3];

    //length = (length & 255) + packet / 256;
    length &= 255;
    length += packet.high8;

