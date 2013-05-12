// # SX28 pdl v2 "include" file: sx28pdlv2.h

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

// used with pdl_inc1.h, pdl_inc2.h and pdl_code.h


struct pdl_temp {
	int op_bits;
	int a1_bits;
	int a2_bits;
};

struct pdl_inst {
	struct pdl_temp *ptr;
	unsigned int opcode;
	void *c_code_ptr;
	int cycles1;		// normal clock cycles used
	int cycles2;		// altername (skip/jum) cycles used
	unsigned char opcode_text[20];
};

#define PDL_TEMP(name,a1,a2,a3) struct pdl_temp name = { a1, a2, a3 }
//
// mugged for dwutil use -- remove pointer to instruction C code...
//
#if 0
#define PDL_INST(temptype,opcode,code,cycles1,cycles2, opcode_text) \
		{&temptype, opcode, ((void *) code), cycles1, cycles2, opcode_text}
#else
#define PDL_INST(temptype,opcode,code,cycles1,cycles2, opcode_text) \
		{&temptype, opcode, NULL, cycles1, cycles2, opcode_text}
#endif

//#end of file





