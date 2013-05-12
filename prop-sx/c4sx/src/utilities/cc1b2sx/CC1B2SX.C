/*
CC1B2SX.C

copyright (C) 2007,  SimuLogics
All rights reserved worldwide

2007-Aug-21 0.00.06 (Bob)

		. added code to comment out STATUS and RTCC assember EQUs, if present 

2007-Jul-30 0.00.05 (Bob)

        . disabled the reading of the HDR and TRL files
        . these can be reenabled by adding '#define ENABLE_HDRTRL'
        . changed to version '.05'
         

2007-Jul-25	0.00.04

		. added / *<?asm ... ?>* / minimalist handling
		. parameterized pragma name (for future enhancements)
		. removed #pragma processing as CC1B chokes on unknown pragma names


2007-Jul-24	0.00.03

		. added #pragma SX <SX-Key line> to capabilities
		. added time and date of translation for reference
		. invalid command line arguments now display error message


2007-Apr-27	0.00.02

		. added insert header file option;
		  -h for explicit name, else default of 'cc1bsx.hdr'
		. added insert trailer file option;
		  -t for explicit name, else default of 'cc1bsx.trl'
		. 'RESET main' substitution removed from the change list;
		  use header or trailer file as appropriate


2007-Apr-21	0.00.01	initial development alpha release


	converts CC1B compiler assembler code to SX-Key acceptable code


uses output from compiler:

	CC1B version 0.6H beta, Copyright (c) B Knudsen Data
	(version 0.6I now tested)



To create an .asm file from original .c source file:

	cc1b  -a  <filename.c>


	use:		cc1b2sx  [options] < input_file  > output_file

	where:
		options:

		-h  <filename>	name of header  insert file, default is 'cc1bsx.hdr' in current directory
		-t  <filename>	name of trailer insert file, default is 'cc1bsx.trl' in current directory

		< input_file	redirection of input file

		> output_file	redirection of output file

	return:	0	no errors (success?)
		1	invalid option,
			or no argument specified to option


todo:
	- in-line assembler option

Notes:
	- header and trailer files are optional

Warnings:
	- very picky and sensitive about compiler output format
	- no error message sent to stdout for command line option error
	  so output file can have 0 bytes on error, but stderror is used for errors
	- compiler does not know about assembler code so may use registers asm code uses!
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


#define	PROG_VERSION	"0.00.06"


/*  additional text substitution or reserved words
    (from CC5B Compiler Manual Appendix)
W
INDF	TMR0	PCL	STATUS	FSR	PORTA	PORTB	PORTC
INDF0	RTCC	PC  // alternative names of above //
OPTION	TRISA	TRISB	TRISC
Carry	DC	Zero_	PD	TO	PA1	PA2
FSR_5	FSR_6
*/


static	int	shift		= 0 ;		/*  shift listing comments to left margin  */
static	char	lineIn [ 2048 ] ;
static	char	lineOut[ 2048 ] ;
static	char *	hdrFile		= NULL ;
static	char *	trlFile		= NULL ;


		/*----------------------------------------------*/
		/*	      text substitution table		*/
		/*----------------------------------------------*/
static struct
	{
	char *		matchStr ;
	char *		newStr ;
	}    subTable[] =

	{
		/*  SX register references  */

	  { "RA "			, ";-- RA "		}
	, { "RB "			, ";-- RB "		}
	, { "RC "			, ";-- RC "		}
	, { "RD "			, ";-- RD "		}
	, { "RE "			, ";-- RE "		}
	, { "INDF "			, ";-- INDF "		}
	, { "FSR "			, ";-- FSR "		}
	, { "PC "			, ";-- PC "		}
	, { "PCL"			, ";-- PCL "		}
	, { "RTCC "         , ";-- RTCC "	}
    , { "STATUS "		, ";-- STATUS "	}
	, { "        ORG $07FF"		, "        ;--- ORG $07FF"		}
	, { "        ORG $01FF"		, "        ;--- ORG $01FF"		}
/*	, { "        JMP main"		, "        RESET main     ;-- JMP main" }
 */
	, { "        JMP main"		, "        ;-- JMP main" }

	, { NULL			, NULL			}
	} ;


		/*----------------------------------------------*/
		/*		    substitute			*/
		/*----------------------------------------------*/

	static char * substitute( char in[], char out[] )
		{
		char *		p ;
		char *		p1 ;
		int		a ;
		int		b ;

		out[0] = '\0' ;
		p1 = p = in ;

		/* retain leading whitespace for now
		 *
		 * for ( p1 = p = in ; ((*p1 == ' ') || (*p1 == '\t')) ; ++p1 )
		 *	{
		 *	}
		 */
		p1 = p ;
		for ( a = 0 ; (subTable[a].matchStr) ; ++a )
			{
			b = strlen( subTable[a].matchStr ) ;
			if ( strncmp(p1,subTable[a].matchStr,b) == 0 )
				{
				strcat( out, subTable[a].newStr ) ;
				p1 = p + b ;
				break ;
				}
			}
		strcat( out, p1 ) ;
		return ( out ) ;
		}	/*  end of 'substitute'  */


#if	defined(_NO_MORE_)

		/*----------------------------------------------*/
		/*		    pragma_line			*/
		/*----------------------------------------------*/

	char * pragma_line( char in[], char out[], char * pragmaName )
		{
		char *		xlate ;
		char *		p ;

		/*  #pragma SX <SX-Key line>
		 */
		/*  (use token routines in here instead of in-line code?)  */
		xlate = NULL ;
		/*  eat leading whitespace  */
		for ( p = in ; ((*p == ' ') || (*p == '\t')) ; ++p ) {}
		if ( *p == '#' )
			{
			/*  eat any whitespace after the '#'  */
			for ( ++p ; ((*p == ' ') || (*p == '\t')) ; ++p ) {}
			if ( strncmp(p,"pragma",6) == 0 )
			   if ( (*(p+6) == ' ') || (*(p+6) == '\t') )
				{
				/* [<whitespace>]#[<whitespace>]pragma seen  */
				for ( p = p + 6 ; ((*p == ' ') || (*p == '\t')) ; ++p ) {}
				if ( strnicmp(p,pragmaName,strlen(pragmaName)) == 0 )
				   if ( (*(p+2) == ' ') || (*(p+2) == '\t') )
					{
					/* [<whitespace>]#[<whitespace>]pragma<whitespace>SX<whitespace> seen  */
					/* <copy line here>  */
					strcpy( out, (p+2) ) ;
					xlate = out ;
					}
				}
			}
		return ( xlate ) ;
		}	/*  end of 'pragma_line'  */

#endif	/*  _NO_MORE_  */

		/*----------------------------------------------*/
		/*		      sxkey_code		*/
		/*----------------------------------------------*/

	int sxkey_code( char in[], char out[], char * leadin, char * leadout, FILE * inFp, FILE * outFp )
		{
		char *		p ;
		char *		p1 ;
		int		result ;

		/* Q & D search for 'leadin' and 'leadout' done,
		 * no trailing whitespace detection done;
		 * no end of file without 'leadout' detection done
		 */
		result = 0 ;
		/*  eat leading whitespace  */
		for ( p = in ; ((*p == ' ') || (*p == '\t')) ; ++p ) {}
		if ( *p == ';' )
			{
			for ( ++p ; ((*p == ' ') || (*p == '\t')) ; ++p ) {}
			if ( strncmp(p,leadin,strlen(leadin)) == 0 )
				{
				/*  start with next line  */
				/*  copy in to out until 'leadout' seen  */

				while ( (fgets(in,sizeof(lineIn),inFp)) )
					{
					for ( p = in ; ((*p == ' ') || (*p == '\t')) ; ++p ) {}
					if ( *p == ';' )
						{
						for ( p1 = ++p ; ((*p1 == ' ') || (*p1 == '\t')) ; ++p1 ) {}
						if ( strncmp(p1,leadout,strlen(leadout)) == 0 )
							{
							break ;
							}
						fprintf( outFp, "%s", p ) ;
						}
					}
				result = 1 ;
				}
			}
		return ( result ) ;
		}	/*  end of 'sxkey_code' */


		/*----------------------------------------------*/
		/*		      do_shift			*/
		/*----------------------------------------------*/

	static char * do_shift( char * p )
		{
		char *		p1 ;

		/*  determine if ';' is first non-whitespace character in line;
		 *  return original line if not,
		 *  return position of ';' if true.
		 */
		for ( p1 = p ; ((*p1 == ' ') || (*p1 == '\t')) ; ++p1 )
			{
			}
		return ( (*p1 == ';') ? p1 : p ) ;
		}	/*  end of 'do_shift'  */


		/*----------------------------------------------*/
		/*		      insertFile		*/
		/*----------------------------------------------*/

	int insertFile( char * fileName)
		{
		int		a ;
		FILE *		fp ;

		a = 0 ;
		fp = fopen( fileName, "rt" ) ;
		if ( fp )
			{
			while ( (fgets(lineIn,sizeof(lineIn),fp)) )
				{
				printf( "%s", lineIn ) ;
				++a ;
				}
			fclose( fp ) ;
			}
		return ( a ) ;
		}	/*  end of 'insertFile'  */



	/*==============================================================*/
	/*				main				*/
	/*==============================================================*/

int main( int argc, char * argv[] )
	{
	int		a ;
	char *		p ;
	time_t		ltime ;

	shift	= 0 ;
	hdrFile	= "cc1b2sx.hdr" ;
	trlFile	= "cc1b2sx.trl" ;

	for ( a = 1 ; a < argc ; ++a )
		{
		if ( strcmp(argv[a],"-h") == 0 )
			{
			if ( (++a) >= argc )
				{
				/*
				 * printf( "error - no argument for option %s \n", argv[(a-1] ) ;
				 */
				return ( 1 ) ;
				}
			hdrFile = argv[a] ;
			}
		else if ( strcmp(argv[a],"-t") == 0 )
			{
			if ( (++a) >= argc )
				{
				/*
				 * printf( "error - no argument for option %s \n", argv[(a-1] ) ;
				 */
				return ( 1 ) ;
				}
			trlFile = argv[a] ;
			}
		else if ( strcmp(argv[a],"-s") == 0 )
			{
			shift = 1 ;
			}
		else
			{
			fprintf( stderr, "error - invalid option: %s \n", argv[a] ) ;
			return ( 1 ) ;
			}
		}	

	printf( ";-- cc1b2sx [rev %s] \n", PROG_VERSION ) ;
	time( &ltime ) ;
	printf( ";-- %s \n", ctime(&ltime) ) ;

#ifdef ENABLE_HDRTRL
	insertFile( hdrFile ) ;	/*  <insert header>  */
#endif

	while ( fgets(lineIn,sizeof(lineIn),stdin) )
		{
		if ( ! sxkey_code(lineIn,lineOut,"/*<?asm", "?>*/",stdin,stdout) )
			{
			substitute( lineIn, lineOut ) ;
			p = (shift) ? do_shift(lineOut) : lineOut ;
			printf( "%s", p ) ;
			}
		}

#ifdef ENABLE_HDRTRL
	insertFile( trlFile ) ;	/*  <insert trailer>  */
#endif

	return ( 0 ) ;
	}	/*  end of 'main'  */

