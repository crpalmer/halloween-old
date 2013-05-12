/* pdl_rdr.h */

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

#define RDR_CARRYX_PRESENT (0x01)
#define RDR_SX28_DEFINED (0x02)
#define RDR_SX48_DEFINED (0x04)
#define RDR_SX52_DEFINED (0x08)

#define RDR_ERROR_OPEN_FAIL (-1)
#define RDR_ERROR_INVALID_FORMAT (-2)
#define RDR_ERROR_BAD_HEX_CHAR (-3)
#define RDR_ERROR_INTERNAL_FAILURE (-4)

int lstrdr(char * file_name, int array_size, unsigned short array[] );

int hexrdr(char * file_name, int array_size, unsigned short array[] );

int genrdr(char * file_name, int array_size, unsigned short array[] );

/* end of pdl_rdr.h */