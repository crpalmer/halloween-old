/* main.cpp */

/* driver for hex to SX-Key sasm formatter */

/* copyright.txt

 copyright 2007 Bob Senser

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

/*
  Maintenance History:
  Date     Comments
  07/19/2007 0.1 version

*/

#define VERSION "0.1"

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>


// #include "sxcore.h"

// files in pdlgen where generated....
#include "pdlgen/sx28pdlv2.h"

#include "pdlgen/pdl_inc2.h"

#include "pdl_rdr.h"

#define QUIET

#define FPRINTF lines_written++; fprintf


void help(char * program) {
	printf(
	"Help information:\n\n"
	"    Usage syntax: %s <input_file> <output_file>\n"
	"    Example: %s myprgm.hex myprgm.src\n"
	"    The output will be in <output_file>\n", program, program
	);

	return;

}

#ifndef QUIET

void show_pgm(unsigned short pgm_mem[], int pgm_mem_size, char mode) {

	// if mode is 'd', dump format
	// if mode is 'c', c-code format

	int i;

	printf("esx Program Memory: (0x%x bytes)\n", pgm_mem_size);
	
	for (i=0;i < pgm_mem_size; i++) {
		if (mode == 'd') {
			if (i == 0) printf("%04x ", i);
			if ((i % 8) == 0 && i != 0) printf("\n%04x ",i);
			printf(" %03x", pgm_mem[i]);
		} else {
			if ((i % 8) == 0 && i != 0) printf("\n");
			printf(" 0x%03x,", pgm_mem[i]);
		}
	}
	printf("\n");

}
#endif

/* from sxcore.cpp ..... and added 3 output fields */

unsigned char *get_opcode_text(unsigned int opcode, int *a0, int *a1, int *a2) {

	unsigned char* return_value = NULL;

	int i = 0;
	int shift_value;
	unsigned int opcode_from_instructions;
	unsigned int opcode_from_pgm;

	while (instructions[i].ptr != NULL) {
		shift_value = 12 - (instructions[i].ptr -> op_bits);
		opcode_from_instructions = instructions[i].opcode >> shift_value;
		opcode_from_pgm = opcode >> shift_value;
		if (opcode_from_instructions == opcode_from_pgm) {
			return_value = instructions[i].opcode_text;
			*a0 = instructions[i].ptr -> op_bits;
			*a1 = instructions[i].ptr -> a1_bits;
			*a2 = instructions[i].ptr -> a2_bits;
			break;
		}
		i++;
	}

	return return_value;

}

	/*----------------------------------------------*/
	/*		      insertFile						*/
	/*----------------------------------------------*/

	int insertFile( char * inFileName, FILE * target)
		{
		int		a ;
		FILE *		fp ;
		static	char	lineIn [ 2048 ] ;

		a = 0 ;
		fp = fopen( inFileName, "rt" ) ;
		if ( fp )
			{
			while ( fgets(lineIn,sizeof(lineIn),fp) )
				{
				fprintf(target, "%s", lineIn ) ;
				++a ;
				}
			fclose( fp ) ;
			}
		return ( a ) ;
		}	/*  end of 'insertFile'  */


int main(int argc, char *argv[]) {

	unsigned char *op_code_ptr;
	int op_bits;
	int a1;
	int a2;
	int work;
	int arg1;
	int arg2;
	char inst_buffer[128];
	char work_buffer[128];
    char arg1_buffer[128];
    char arg2_buffer[128];
    char small_buffer[4];
	int return_value;
	FILE *outfile;
	int i;
    int j;
    int m;
	int current_location;
	int instructions_processed = 0;
	int lines_written = 0;

	char hdrFile[]	= "dwutil.hdr" ;

	const int pgm_mem_size = 2 * 1024;
	unsigned short pgm_mem[pgm_mem_size];

	if (argc != 3) {
		printf("error: input file name not supplied!\n");
		help(argv[0]);
		exit(0);
	}

	if (strcmp(argv[2], argv[1]) == 0) {
		printf("error: input file = output file!\n");
		help(argv[0]);
		exit(0);
	}
	
	if ((return_value = genrdr(argv[1], pgm_mem_size , 
			pgm_mem)) < 0) {
		printf("Error: input file did not open!\n");
		help(argv[0]);
		exit(0);
	}

#ifndef QUIET
	show_pgm(pgm_mem, pgm_mem_size, 'd');
#endif

	outfile = fopen(argv[2], "w");
	if (outfile == NULL) {
		printf("output file did not open\n");
		exit(0);
	}

	FPRINTF(outfile, "; producted by dwutil (version: %s)\n", VERSION);

	/* for added flexibility use separate file.... */
#if 0
	FPRINTF(outfile, "      DEVICE	SX28L\n");
	FPRINTF(outfile, "		DEVICE	turbo\n");
	FPRINTF(outfile, "		DEVICE	optionx\n");
	FPRINTF(outfile, "		DEVICE	stackx\n");	
	FPRINTF(outfile, "		DEVICE	OSCHS3\n");
	FPRINTF(outfile, "	IRC_CAL	IRC_FAST\n");
	FPRINTF(outfile, "	FREQ	50_000_000\n");
	// fprintf(outfile, "	RESET	START\n");
#else
	insertFile( hdrFile, outfile ) ;	/*  <insert header>  */
#endif


	current_location = -1;
	for (i=0; i < pgm_mem_size; i++) {
		if (pgm_mem[i] != 0xfff) {
			if (current_location != i) {
				current_location = i;
				fprintf(outfile,"        ORG   $%x\n", current_location);
			}
		
			current_location++;	

			// if (i != 0x7ff) {
			op_code_ptr = NULL;
			op_code_ptr = get_opcode_text(pgm_mem[i], &op_bits, &a1, &a2);
			instructions_processed++;
			if (op_code_ptr != NULL) {
				work = pgm_mem[i];
				arg2 = ((1 << a2) - 1) & work;
				work = work >> a2;
				arg1 = ((1 << a1) - 1) & work;
				strcpy(arg1_buffer,"");
				strcpy(arg2_buffer,"");
				strcpy(inst_buffer,"");
				sprintf(work_buffer,"%s ", op_code_ptr);
				if (a1 > 0) {
					sprintf(arg1_buffer,"0x%x", arg1);
					// strcat(inst_buffer, arg1_buffer);
				}
				if (a2 > 0) {
					sprintf(arg2_buffer,"0x%x", arg2);
					// strcat(inst_buffer, arg2_buffer);
				}
                m = 0;
				for (j=0; j <= (signed int) strlen(work_buffer); j++) {
					strcpy(small_buffer," ");
                    if (work_buffer[j] != '$') {
                        // just copy
						small_buffer[0] = work_buffer[j];
						strcat(inst_buffer,small_buffer);
					} else {
                        // replace $ by value
                        // if two args, use arg2 first....
                        if (m == 0) {
							// first $
							if (a1 > 0 && a2 > 0) {
								strcat(inst_buffer,arg2_buffer);
							} else {
								strcat(inst_buffer,arg1_buffer);
							}
                         } else {
                            // second $, must be arg1
							strcat(inst_buffer,arg1_buffer);										
                         }
                         m++;
					}
                
				}
				FPRINTF(outfile,"        DW   $%03x  ; %03x %s \n",
					pgm_mem[i], i, inst_buffer);
			} else {
				FPRINTF(outfile,"        DW   $%03x\n", pgm_mem[i]);
			}
		}
	}

	FPRINTF(outfile, "     end\n");

	fclose(outfile);

	printf("%s created by dwutil (version: %s).\n", argv[2], VERSION);
	printf("instructions processed: %d\n", instructions_processed);
	printf("lines written: %d\n", lines_written);

	return 0;

}