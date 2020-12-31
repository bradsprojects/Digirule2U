#include "adr2.h"

fields_t fields;

static struct
{
	char *pline;
} priv;

void parse_get_label_operation( void )
{
	enum
	{
		StateFirst,
		StateLabel,
		StateWhite,
		StateOperation,
		StateWhite2,
		StateComment,
	} state;
	char c, *plabel, *poperation;
	
	priv.pline = list_line.source;
	plabel	   = fields.label;
	poperation = fields.operation;
	state	   = StateFirst;
	
	for( ;; )
	{
		c = *priv.pline++;
		
		switch( state )
		{
			case StateFirst:
				switch( c )
				{
					case NUL:
						priv.pline--;
						*plabel = *poperation = NUL;
						return;
						
					default:
						*plabel++ = c;
						state = StateLabel;
						break;
						
					case ' ':
						*plabel = NUL;
						state = StateWhite;
						break;
						
					case '*':	// valid in column 1 only
					case ';':
						*plabel = *poperation = NUL;
						state = StateComment;
						break;
				}
				
				break;
				
			case StateLabel:
				switch( c )
				{
					case NUL:
						priv.pline--;
						*plabel = *poperation = NUL;
						return;
						
					default:
						*plabel++ = c;
						break;
						
					case ' ':
						*plabel = NUL;
						state = StateWhite;
						break;
						
					case ';':
						*plabel = *poperation = NUL;
						state = StateComment;
						break;
				}
				
				break;
				
			case StateWhite:
				switch( c )
				{
					case NUL:
						priv.pline--;
						*poperation = NUL;
						return;
						
					case ' ':
						break;
						
					default:
						*poperation++ = c;
						state = StateOperation;
						break;
						
					case ';':
						*poperation = NUL;
						state = StateComment;
						break;
				}
				
				break;
				
			case StateOperation:
				switch( c )
				{
					case NUL:
						priv.pline--;
						*poperation = NUL;
						return;
						
					default:
						*poperation++ = c;
						break;
						
					case ' ':
						*poperation = NUL;
						state = StateWhite2;
						break;
						
					case ';':
						*poperation = NUL;
						state = StateComment;
						break;
				}
				
				break;
				
			case StateWhite2:
				switch( c )
				{
					default:
						priv.pline--;
						return;
						
					case ' ':
						break;
				}
				
				break;
				
			case StateComment:
				switch( c )
				{
					case NUL:
						priv.pline--;
						return;
						
					default:
						break;
				}
				
				break;
		}
	}
}

int parse_get_operand( char *buf )
{
	enum
	{
		StateFirst,
		StateOperand,
		StateOperandQuote,
		StateOperandQuote2,
		StateComment,
	} state;
	char c;

	state = StateFirst;
	
	for( ;; )
	{
		c = *priv.pline++;
		
		switch( state )
		{
			case StateFirst:
				switch( c )
				{
					case NUL:
						priv.pline--;
						*buf = NUL;
						return 0;
						
					case ',':
						*buf = NUL;
						return 1;
						
					case '\'':
						*buf++ = c;
						state = StateOperandQuote;
						break;
						
					case ' ':
					case ';':
						state = StateComment;
						break;
						
					default:
						*buf++ = c;
						state = StateOperand;
						break;
				}
				
				break;
				
			case StateOperand:
				switch( c )
				{
					case NUL:
					case ' ':
					case ';':
						priv.pline--;
						*buf = NUL;
						return 1;
						
					case ',':
						*buf = NUL;
						return 1;
						
					case '\'':
						*buf++ = c;
						state = StateOperandQuote;
						break;
						
					default:
						*buf++ = c;
						break;
				}
				
				break;
				
			case StateOperandQuote:
				switch( c )
				{
					case NUL:
						priv.pline--;
						*buf = NUL;
						return 1;
						
					case '\'':
						*buf++ = c;
						state = StateOperandQuote2;
						break;
						
					default:
						*buf++ = c;
						break;
				}
				
				break;
				
			case StateOperandQuote2:
				switch( c )
				{
					case NUL:
						priv.pline--;
						*buf = NUL;
						return 1;
						
					case '\'':
						*buf++ = c;
						state = StateOperandQuote;
						break;
						
					default:
						priv.pline--;
						state = StateOperand;
						break;
				}
				
				break;
				
			case StateComment:
				switch( c )
				{
					case NUL:
						priv.pline--;
						*buf = NUL;
						return 0;
						
					default:
						break;
				}
				
				break;
		}
	}
}

void parse_get_line( char *buf )
{
	char c;
	
	for( ;; )
	{
		c = *priv.pline++;
		
		switch( c )
		{
			case NUL:
				priv.pline--;
				*buf = NUL;
				return;
				
			default:
				*buf++ = c;
				break;
		}
	}
}

void parse_lookup_operation( const char *s, operation_info_t *poi )
{
	static const struct
	{
		const char   *s;
		operation_t   operation;
		void        (*operation_fn)( struct operation_info_tag * );
	} lut[] =
	{
		{ "",        OpDefault, inst_default },
		
		// instructions (in alphabetical order)
		{ "ADDLA",   OpADDLA,   inst_2byte   },
		{ "ADDRA",   OpADDRA,   inst_2byte   },
		{ "ADDRPC",  OpADDRPC,  inst_2byte   },
		{ "ANDLA",   OpANDLA,   inst_2byte   },
		{ "ANDRA",   OpANDRA,   inst_2byte   },
		{ "BCHG",    OpBCHG,    inst_3byte   },
		{ "BCLR",    OpBCLR,    inst_3byte   },
		{ "BSET",    OpBSET,    inst_3byte   },
		{ "BTSTSC",  OpBTSTSC,  inst_3byte   },
		{ "BTSTSS",  OpBTSTSS,  inst_3byte   },
		{ "CALL",    OpCALL,    inst_call    },
		{ "CALLI",   OpCALLI,   inst_call    },
		{ "COMIN",   OpCOMIN,   inst_1byte   },
		{ "COMOUT",  OpCOMOUT,  inst_1byte   },
		{ "COMRDY",  OpCOMRDY,  inst_1byte   },
		{ "COPYAI",  OpCOPYAI,  inst_2byte   },
		{ "COPYAR",  OpCOPYAR,  inst_2byte   },
		{ "COPYIA",  OpCOPYIA,  inst_2byte   },
		{ "COPYII",  OpCOPYII,  inst_3byte   },
		{ "COPYIR",  OpCOPYIR,  inst_3byte   },
		{ "COPYLA",  OpCOPYLA,  inst_2byte   },
		{ "COPYLI",  OpCOPYLI,  inst_3byte   },
		{ "COPYLR",  OpCOPYLR,  inst_3byte   },
		{ "COPYRA",  OpCOPYRA,  inst_2byte   },
		{ "COPYRI",  OpCOPYRI,  inst_3byte   },
		{ "COPYRR",  OpCOPYRR,  inst_3byte   },
		{ "DECR",    OpDECR,    inst_2byte   },
		{ "DECRJZ",  OpDECRJZ,  inst_2byte   },
		{ "DIV",     OpDIV,     inst_3byte   },
		{ "HALT",    OpHALT,    inst_1byte   },
		{ "INCR",    OpINCR,    inst_2byte   },
		{ "INCRJZ",  OpINCRJZ,  inst_2byte   },
		{ "INITSP",  OpINITSP,  inst_initsp  },
		{ "JUMP",    OpJUMP,    inst_2byte   },
		{ "JUMPI",   OpJUMPI,   inst_2byte   },
		{ "MUL",     OpMUL,     inst_3byte   },
		{ "NOP",     OpNOP,     inst_1byte   },
		{ "ORLA",    OpORLA,    inst_2byte   },
		{ "ORRA",    OpORRA,    inst_2byte   },
		{ "PINDIR",  OpPINDIR,  inst_2byte   },
		{ "PININ",   OpPININ,   inst_2byte   },
		{ "PINOUT",  OpPINOUT,  inst_2byte   },
		{ "RANDA",   OpRANDA,   inst_1byte   },
		{ "RETLA",   OpRETLA,   inst_2byte   },
		{ "RETURN",  OpRETURN,  inst_1byte   },
		{ "SHIFTRL", OpSHIFTRL, inst_2byte   },
		{ "SHIFTRR", OpSHIFTRR, inst_2byte   },
		{ "SPEED",   OpSPEED,   inst_2byte   },
		{ "SUBLA",   OpSUBLA,   inst_2byte   },
		{ "SUBRA",   OpSUBRA,   inst_2byte   },
		{ "SWAPRA",  OpSWAPRA,  inst_2byte   },
		{ "SWAPRR",  OpSWAPRR,  inst_3byte   },
		{ "XORLA",   OpXORLA,   inst_2byte   },
		{ "XORRA",   OpXORRA,   inst_2byte   },

		// directives (in alphabetical order)
		{ "BYTE",    OpBYTE,    dir_byte     },
		{ "END",     OpEND,     dir_end      },
		{ "EQU",     OpEQU,     dir_equ      },
		{ "INCLUDE", OpINCLUDE, dir_include  },
		{ "LIST",    OpLIST,    dir_list     },
		{ "NOLIST",  OpNOLIST,  dir_nolist   },
		{ "NOPAGE",  OpNOPAGE,  dir_nopage   },
		{ "ORG",     OpORG,     dir_org      },
		{ "PAGE",    OpPAGE,    dir_page     },
		{ "SPACE",   OpSPACE,   dir_space    },
		
		{ NULL,      OpInvalid, inst_invalid },
	}, *p;
	
	for( p = lut; p < lut + ELEMENTSOF( lut ) - 1; p++ )
		if( strcasecmp( s, p->s ) == 0 )
			break;
	
	poi->operation    = p->operation;
	poi->operation_fn = p->operation_fn;
}
