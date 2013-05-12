//
// pdl_rdr.cpp
//

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

//
// Comments from author:
//
// C-style program to read the Parallax Assembler/Compiler (SXB) 
// listing for the generated SX code.
//
// Important:
// This "lst" routine is a heuristic "hack" and needs to be rewitten! Known
// limits and workarounds:
// 1)  it DOES detect the DEVICE "carryx" setting and the SX
//     processor type.
// 2)  it DOES NOT correctly handle Device settings conditional
//     assembly code -- for example in Sx_arith.src.  Workaround is
//     to comment out unwanted DEVICE lines.
// 3)  any assembler macros used MUST BE expanded.  Workaround is 
//     to avoid using the "noexpand" and be sure the macro source
//     contains "expand" -- for example in uart_vp.src.
// 4)  it DOES handle multi-line instruction expansions
// 5)  it DOES probably have bugs.....
//
// 2006-11-09: Added hexrdr() function - this allows for reading input pgm in
// "standard" hex format.  This is useful with SX-C C compiler.
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "pdl_rdr.h"

//
// "lst" file reader, very Parallax SX specific
int lstrdr(char * file_name, int array_size, unsigned short array[] ) {

	int i;
	unsigned int ui;
	char * ptr;
	char * ptr1;
	char * ptr2;
	char * ptr3;
	char * ptr4;
	unsigned int addr;
	unsigned int value;
	bool ok = true;
	bool has_linenum;
	char  buffer[255];
	char  orig_buffer[255];
	FILE *lst_file;
	int	return_value;

	for (i=0; i < array_size; i++) {
		array[i] = 0xfff;
	}

	lst_file = fopen(file_name, "r");
	if (lst_file == NULL) {
		return RDR_ERROR_OPEN_FAIL;
	}

	return_value = 0;

	while(ok) {
		ptr = fgets(buffer, sizeof(buffer)-1, lst_file);
		if (ptr == NULL) {
			ok = false;
		} else {
			strcpy(orig_buffer,ptr);
			// make all upper case for simplicity...
			for (ui=0; ui < strlen(ptr); ui++) {
				ptr[ui] = toupper(ptr[ui]);

			}
			if (strlen(ptr) > 7) {
				if (memcmp(ptr,"CROSS R", 7) == 0) {
					ok = false;
					break;
				}
			}
			if (strlen(ptr) > 3) {
				if (memcmp(ptr, "***", 3) == 0) {
					continue;	// skip warning lines
				}
			}
			// looking for decnum hexnum hexnum or
			//                    hexnum hexnum or
			//					  hexnum hexnum hexnum ....
			// or
			//				decnum hexnum dexnum  device ....
			// ignore comment lines and literals with "=" present
			// note:  comments don't have hexnums (data values)
			has_linenum = true;
			if (strlen(ptr) > 5) {
				if (memcmp(ptr,"      ", 6) == 0) {
					has_linenum = false;
				}
			}

			if (has_linenum) {
				ptr1 = strpbrk(ptr,"1234567890"); //decimal num
				if (ptr1 == NULL) continue;	
				ptr2 = strstr(ptr1," ");
				if (ptr2 == NULL) continue;	
			} else {
				ptr3 = strpbrk(ptr2,"0123456789ABCDEF"); // hex num
				if (ptr3 == NULL) {
					ok = false;
				}
				ptr2 = ptr3;
			} 

			// kludgy way to handle Device (or FUSES or PROCESSOR)
			if (ok && has_linenum) {
				ptr4 = strstr(ptr2," ");	// skip space after line num
				if ((ptr3 = strstr(ptr2,";")) != NULL) {
					*ptr3 = 0;			// kill comment area
				}
// suspect this code is not needed....
#if 0
				if (ptr4 != NULL) {
					ptr4 = strpbrk(ptr4,"0123456789ABCDEF"); // skip to hex num
					if (ptr4 != NULL) {
						ptr4 = strstr(ptr4," ");  // skip to space after hex num
						if (ptr4 != NULL) {
							ptr4 = strpbrk(ptr4,"0123456789ABCDEF"); // skip to hex num
							if (ptr4 != NULL) {
								ptr4 = strstr(ptr4," ");  // skip to space after hex num
							}
						}
					}
				}
#endif
				if (ptr4 != NULL) {
					while (ptr4 != NULL && *ptr4 == ' ') {
						ptr4++;
					}
					// now can look for DEVICE and options...
					if (strstr(ptr4,"DEVICE") != NULL ||
						strstr(ptr4,"FUSE") != NULL ||
						strstr(ptr4, "PROCESSOR") != NULL) {
						if (strstr(ptr4, "CARRYX") != NULL) {
							return_value |= RDR_CARRYX_PRESENT;
						}
						if (strstr(ptr4, "SX28") != NULL ||
							strstr(ptr4, "PINS28") != NULL) {
							return_value |= RDR_SX28_DEFINED;
						}
						continue;	// don't process DEVICE farther...
					}
				}
			}

			// only process generated code values, no assembler text
			if (has_linenum && strlen(buffer) > 22) {
				buffer[22] = 0;
			}

			// printf("DEBUG: %s\n", buffer);

			if (ok) {
				ptr2 = strpbrk(ptr2,";=0123456789ABCDEF"); // hex num plus
				if (ptr2 == NULL) continue;
				if (*ptr2 == ';') continue;		// skip comment
				if (*ptr2 == '=') continue;		// skip literal
				ptr3 = strstr(ptr2," ");
				if (ptr3 == NULL) continue;
				ptr3 = strpbrk(ptr3,"0123456789ABCDEF"); // hex num
				if (ptr3 == NULL) continue;
				sscanf(ptr2,"%x",&addr);
				sscanf(ptr3,"%x",&value);
				// printf("DEBUG %04x %04x\n", addr, value);
				if (addr > 0x7ff) {
					printf("Internal error in rdr (1), addr over 0x7ff!\n");
					printf("line: %s\n", orig_buffer);
					return RDR_ERROR_INTERNAL_FAILURE;
				}
				// printf("DEBUG: addr: %x value: %x\n", addr, value);

				array[addr] = value;
				// check for more values
				while (!has_linenum) {
					ptr3 = strpbrk(ptr3," ");
					if (ptr3 == NULL) break;
					ptr3 = strpbrk(ptr3,"0123456789ABCDEF"); // hex num
					if (ptr3 == NULL) break; 
					sscanf(ptr3,"%x",&value);
					addr++;
					if (addr > 0x7ff) {
						printf("Internal error in rdr (2), addr over 0x7ff!\n");
						printf("line: %s\n", orig_buffer);
						return -1;
					}
					array[addr] = value;
				}
			}

			// printf("%s", buffer);
		}

	}

	fclose(lst_file);

	return return_value;
}

//
// "hex" file reader
//
// input example:
// :100000001000000A10001000050A6C006505FF0CC6
// :100010002D00010C2E000E022F000F0230000300F5
// :020FFE00000AE7
// :020FFE00020AE5
// :0C2020001B077F0F0024F400FF05FF05E4
// :00000001FF
// ;SX28AC
//
// format:
// CLLFFFFSS00112233445566778899001122334455RR
// C    is the line code (":' or ";")
// LL   is the byte count
// FFFF is the offset (hex, not byte swapped)
// SS   is skipped
// 11 .. nn are the data bytes
// RR   is likely the CRC and is skipped 
int hexrdr(char * file_name, int array_size, unsigned short array[] ) {

	char  buffer[255];
	FILE *hex_file;
	int	return_value;
	bool ok = true;
	char * ptr;
	int curr_char_offset;
	unsigned int calc_count;

	unsigned int count;
	unsigned int offset;	// in terms of bytes, not 12-bit words!
	unsigned int i;
	int c;
	int byte;
	unsigned int work;
	unsigned int work_hi;
	unsigned int work_lo;

	char count_buffer[16];
	char offset_buffer[16];
	char byte_buffer[16];

	hex_file = fopen(file_name, "r");
	if (hex_file == NULL) {
		return RDR_ERROR_OPEN_FAIL;
	}

	for (i=0; i < ((unsigned int) array_size); i++) {
		array[i] = 0xfff;
	}

	return_value = 0;

	while(ok) {
		ptr = fgets(buffer, sizeof(buffer)-1, hex_file);
		if (ptr == NULL) {
			ok = false;
		} else {
			curr_char_offset = 0;
			// remove any eol character
			if (buffer[strlen(buffer)-1] == '\n') { // 0x0a
				buffer[strlen(buffer)-1] = 0;
			}
			if (buffer[curr_char_offset] == ';') {
				// comment -- skip line
				continue;
			} else {
				if (buffer[curr_char_offset] != ':') {
					return_value = RDR_ERROR_INVALID_FORMAT;  
					break;
				}
				else {
					curr_char_offset++;

					// character validity check
					for (i=curr_char_offset; i < strlen(buffer); i++ ) {
						c = toupper(buffer[i]);
						if (strchr("01234567890ABCDEF", c) == NULL) {
							return_value = RDR_ERROR_BAD_HEX_CHAR;
							break;
						}
					}
					if (return_value != 0) {
						break;
					}

					memset(count_buffer,0,sizeof(count_buffer));
					memset(offset_buffer,0,sizeof(offset_buffer));


					memcpy(count_buffer,&(buffer[curr_char_offset]),2);	// 2 chars for LL
					curr_char_offset += 2;
					memcpy(offset_buffer,&(buffer[curr_char_offset]),4);	// 4 chars for FF
					sscanf(count_buffer,"%x",&count);
					sscanf(offset_buffer,"%x",&offset);
					if (count > 0) {
						// have a good line

						// skip SS
						curr_char_offset += 6;

						// sanity check
						calc_count = 1 + 2 + 4 + 2 + (count * 2) + 2;
						if (strlen(buffer) < calc_count) {
							return_value = RDR_ERROR_INVALID_FORMAT;
							break;
						}

						for (i=0; i < count; i++) {
							memset(byte_buffer,0,sizeof(byte_buffer));
							memcpy(byte_buffer,&(buffer[curr_char_offset]),2);
							curr_char_offset += 2;
							sscanf(byte_buffer,"%x",&byte);
							
							if ((offset / 2) > ((unsigned int) array_size)) {
								return_value = -98;
							} else {
								// note offset is in terms of bytes and array is indexed
								// in terms of shorts (simulated 12-bit words)
								work = array[offset / 2];
								work_hi = work / 256;
								work_lo = work % 256;
								if ((offset % 2) == 0) {
									work_lo = byte;
								} else {
									work_hi = byte;
								}
								work = work_hi * 256 + work_lo;
								array[offset / 2] = work;
							}
							offset++;							
						}
					}
				} // if == ':'

			} // if != ';'

		}  // if null ptr
	} // while


	fclose(hex_file);

	if (return_value >= 0) {
		return_value |= RDR_SX28_DEFINED;
	}

	return return_value;
}

//
// general reader the looks at file name and calls the correct reader
int genrdr(char * file_name, int array_size, unsigned short array[] ) {

	char lower_name[255];
	unsigned int i;

	// ignore case....
	strcpy(lower_name, file_name);
	for (i=0; i < strlen(lower_name); i++) {
		lower_name[i] = tolower(lower_name[i]);
	}

	if (strstr(lower_name, ".hex") != NULL) {
		return hexrdr(file_name, array_size, array);
	} else {
		return lstrdr(file_name, array_size, array);
	}

}

// end of pdl_rdr.cpp