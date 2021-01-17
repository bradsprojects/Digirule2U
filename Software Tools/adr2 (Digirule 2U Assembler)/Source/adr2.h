#ifndef adr2_h
#define adr2_h

#include <ctype.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef __APPLE__
#define __printflike( a, b )
#endif

////////////////////////////////////////////////////////////////////////////////

#define TITLE		"Digirule 2U Cross-Assembler"
#define BUILT		"17 Jan 2021"
#define BY			"KD0GLS"
#define BANNER		TITLE " | " BUILT " | " BY

#define	RAM_SIZE	252
#define SR			252
#define BR			253
#define AR			254
#define DR			255

#define	ELEMENTSOF( a )		(sizeof( a ) / sizeof( (a)[0] ))

#define SYMBOL_FIRST		'_': case ALPHA
#define SYMBOL_NEXT			'_': case ALPHA: case DIGIT
#define ALPHA				UPPER: case LOWER
#define UPPER				'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z'
#define LOWER				'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z'
#define DIGIT				'0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9'
#define PUNCT				'!': case '"': case '#': case '$': case '%': case '&': case '\'': case '(': case ')': case '*': case '+': case ',': case '-': case '.': case '/': case ':': case ';': case '<': case '=': case '>': case '?': case '@': case '[': case '\\': case ']': case '^': case '_': case '`': case '{': case '|': case '}': case '~'
#define EOL					'\n'
#define NUL					'\0'

#define TRY					switch( (exception_t) setjmp( wormhole ) )
#define THROW( exception )	longjmp( wormhole, exception )

#define msg_address_exceeds_ram				"address exceeds RAM [0x%04X]"
#define msg_cannot_open_include_file		"cannot open include file [%s]"
#define msg_char_const_empty				"character constant empty"
#define msg_char_const_extraneous			"character constant extraneous character(s)"
#define msg_char_const_not_closed			"character constant not closed"
#define msg_divide_by_zero					"divide by zero in expression"
#define msg_duplicate_symbol				"duplicate symbol [%s]"
#define msg_extraneous_operand				"extraneous operand(s)"
#define msg_invalid_operation				"invalid operation [%s]"
#define msg_invalid_space_operand			"invalid space operand"
#define msg_invalid_symbol_name				"invalid symbol name [%s]"
#define msg_label_prohibited				"label prohibited"
#define msg_label_required					"label required"
#define msg_lc_reversal						"location counter reversal prohibited"
#define msg_operand_required				"operand required"
#define msg_operands_required				"operands required"
#define msg_phase_error						"pass 1-2 symbol [%s] values differ"
#define msg_syntax_error					"syntax error [%s]"
#define msg_undefined_symbol				"undefined symbol [%s]"
#define msg_value_truncated					"value truncated"

////////////////////////////////////////////////////////////////////////////////

typedef unsigned char  uint8;
typedef unsigned short uint16;

typedef union
{
	struct
	{
		uint8 low_byte;
		uint8 high_byte;
	};
	uint16 word;
} union16_t;

typedef struct
{
	struct
	{
		int quiet	: 1;
		int nolist	: 1;
		int debug	: 1;
	} cl_opt;
	struct
	{
		int nolist	: 1;
		int nopage	: 1;
		int end		: 1;
	} dir;
	struct
	{
		int initsp	: 1;
		int pass2	: 1;
	};
} flags_t;

typedef struct
{
	void (*open)( void );
	void (*emit8)( uint8 data );
	void (*close)( void );
} object_t;

typedef union
{
	char buf[4+2+8+2+5+1+2+256];
	struct
	{
		char lc			[4];
		char pad		[2];
		char object		[8];
		char object_end	[2];
		char lineno		[5];
		char dot		[1];
		char pad2		[2];
		char source		[256];
	};
	struct
	{
		char pad3		[4+2+4];
		char equ		[4];
	};
} list_line_t;

typedef struct
{
	char label		[256];
	char operation	[256];
	char operand	[256];
	char operand2	[256];
} fields_t;

typedef enum
{
	OpInvalid = -2,
	OpDefault,
	
	OpHALT = 0,		// instructions (must match opcode values)
	OpNOP,
	OpSPEED,
	OpINITSP,
	
	OpCOPYLA,
	OpCOPYLR,
	OpCOPYLI,
	
	OpCOPYAR,
	OpCOPYAI,
	
	OpCOPYRA,
	OpCOPYRR,
	OpCOPYRI,
	
	OpCOPYIA,
	OpCOPYIR,
	OpCOPYII,
	
	OpSWAPRA,
	OpSWAPRR,
	
	OpADDLA,
	OpADDRA,
	OpSUBLA,
	OpSUBRA,
	OpMUL,
	OpDIV,
	
	OpANDLA,
	OpANDRA,
	OpORLA,
	OpORRA,
	OpXORLA,
	OpXORRA,
	
	OpDECR,
	OpINCR,
	OpDECRJZ,
	OpINCRJZ,
	
	OpSHIFTRL,
	OpSHIFTRR,
	
	OpBCLR,
	OpBSET,
	OpBCHG,
	OpBTSTSC,
	OpBTSTSS,
	
	OpJUMP,
	OpJUMPI,
	OpCALL,
	OpCALLI,
	OpRETURN,
	OpRETLA,
	OpADDRPC,
	
	OpRANDA,
	
	OpCOMOUT = 192,
	OpCOMIN,
	OpCOMRDY,
	
	OpPINOUT = 196,
	OpPININ,
	OpPINDIR,
	
	OpBYTE = 256,	// directives
	OpEND,
	OpEQU,
	OpINCLUDE,
	OpLIST,
	OpNOLIST,
	OpNOPAGE,
	OpORG,
	OpPAGE,
	OpSPACE,
} operation_t;

typedef struct operation_info_tag
{
	operation_t  operation;
	void      (* operation_fn)( struct operation_info_tag * );
} operation_info_t;

typedef enum
{
	ExTry,
	ExSyntaxError,
	ExDivideByZero,
	ExInvalidSymbolName,
	ExUndefinedSymbol,
} exception_t;

typedef enum
{
	SymTabOK,
	SymTabNameInvalid,
	SymTabDuplicate,
	SymTabDifferent,
} symtab_t;

////////////////////////////////////////////////////////////////////////////////

void		assemble( void );
void		dir_byte( operation_info_t *poi );
void		dir_end( operation_info_t *poi );
void		dir_equ( operation_info_t *poi );
void		dir_include( operation_info_t *poi );
void		dir_list( operation_info_t *poi );
void		dir_nolist( operation_info_t *poi );
void		dir_nopage( operation_info_t *poi );
void		dir_org( operation_info_t *poi );
void		dir_page( operation_info_t *poi );
void		dir_space( operation_info_t *poi );
void		error( const char *format, ... ) __printflike( 1, 2 );
uint16		eval_const_may_throw( const char *s );
int			eval_expr( const char *s, uint16 *pn );
uint16		eval_expr_may_throw( const char *s );
void		inst_1byte( operation_info_t *poi );
void		inst_2byte( operation_info_t *poi );
void		inst_3byte( operation_info_t *poi );
void		inst_call( operation_info_t *poi );
void		inst_default( operation_info_t *poi );
void		inst_initsp( operation_info_t *poi );
void		inst_invalid( operation_info_t *poi );
uint16		is_unsigned8( uint16 n );
void		label_optional( void );
void		label_prohibited( void );
void		list_close( void );
void		list_emit8( uint8 data );
void		list_emit_equ( uint16 data );
void		list_lc( void );
void		list_line_close( void );
void		list_line_open( void );
void		list_open( void );
void		list_page_eject( void );
void		listf( const char *format, ... ) __printflike( 1, 2 );
void		parse_get_label_operation( void );
void		parse_get_line( char *buf );
int			parse_get_operand( char *buf );
void		parse_lookup_operation( const char *s, operation_info_t *poi );
int			source_push_file( const char *path );
int			source_line_read( void );
void		source_reset( void );
void		symtab_dump( void );
uint16		symtab_get_may_throw( const char *symbol );
symtab_t	symtab_put( const char *symbol, uint16 value );
void		warning( const char *format, ... ) __printflike( 1, 2 );

////////////////////////////////////////////////////////////////////////////////

extern fields_t		fields;
extern flags_t		flags;
extern jmp_buf		wormhole;
extern char			list_path[];
extern list_line_t	list_line;
extern int			source_lineno, total_errors, total_warnings;
extern uint16		lc, end_address;
extern const object_t *object, object_hex, object_src;

#endif // adr2_h
