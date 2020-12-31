#include "adr2.h"

/*
 *	*		location counter
 *	0b1010	binary
 *	1010	decimal
 *	0x1010	hexadecimal
 *	'1010'	character
 *	A..Z	symbol
 */

uint16 eval_const_may_throw( const char *s )
{
	enum
	{
		StateFirst,
		StateLocationCounter,
		StateZero,
		StateBinFirst,
		StateBin,
		StateDec,
		StateHexFirst,
		StateHex,
		StateChar,
		StateChar2,
		StateChar3,
		StateChar4,
		StateSymbol,
	} state;
	char c, symbol[256], *psym;
	union16_t n;
	
	n.word = 0;
	psym = symbol;
	state = StateFirst;
	
	for( ;; )
	{
		c = *s++;
		
		switch( state )
		{
			case StateFirst:
				switch( c )
				{
					case NUL:
						THROW( ExSyntaxError );
						
					case '*':
						state = StateLocationCounter;
						break;
						
					case '0':
						state = StateZero;
						break;
						
					case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
						n.word = n.word * 10 + c - '0';
						state = StateDec;
						break;
						
					case '\'':
						state = StateChar;
						break;
						
					case SYMBOL_FIRST:
						*psym++ = c;
						state = StateSymbol;
						break;
						
					default:
						THROW( ExSyntaxError );
				}
				
				break;
				
			////////////////////////////////////////////////////////////
				
			case StateLocationCounter:
				switch( c )
				{
					case NUL:
						return lc;
						
					default:
						THROW( ExSyntaxError );
				}
				
				break;
				
			////////////////////////////////////////////////////////////
				
			case StateZero:
				switch( c )
				{
					case NUL:
						return 0;
						
					case 'B': case 'b':
						state = StateBinFirst;
						break;
						
					case DIGIT:
						n.word = n.word * 10 + c - '0';
						state = StateDec;
						break;
						
					case 'X': case 'x':
						state = StateHexFirst;
						break;
						
					default:
						THROW( ExSyntaxError );
				}
				
				break;
				
			////////////////////////////////////////////////////////////
				
			case StateBinFirst:
				switch( c )
				{
					case '0':
					case '1':
						n.word = n.word * 2 + c - '0';
						state = StateBin;
						break;
						
					default:
						THROW( ExSyntaxError );
				}
				
				break;
				
			case StateBin:
				switch( c )
				{
					case NUL:
						return n.word;
						
					case '0':
					case '1':
						n.word = n.word * 2 + c - '0';
						break;
						
					default:
						THROW( ExSyntaxError );
				}
				
				break;
				
			////////////////////////////////////////////////////////////
				
			case StateDec:
				switch( c )
				{
					case NUL:
						return n.word;
						
					case DIGIT:
						n.word = n.word * 10 + c - '0';
						break;
						
					default:
						THROW( ExSyntaxError );
				}
				
				break;
				
			////////////////////////////////////////////////////////////
				
			case StateHexFirst:
				switch( c )
				{
					case DIGIT:
						n.word = n.word * 16 + c - '0';
						state = StateHex;
						break;
						
					case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
						n.word = n.word * 16 + c - 'A' + 10;
						state = StateHex;
						break;
						
					case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
						n.word = n.word * 16 + c - 'a' + 10;
						state = StateHex;
						break;
						
					default:
						THROW( ExSyntaxError );
				}
				
				break;
				
			case StateHex:
				switch( c )
				{
					case NUL:
						return n.word;
						
					case DIGIT:
						n.word = n.word * 16 + c - '0';
						break;
						
					case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
						n.word = n.word * 16 + c - 'A' + 10;
						break;
						
					case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
						n.word = n.word * 16 + c - 'a' + 10;
						break;
						
					default:
						THROW( ExSyntaxError );
				}
				
				break;
				
			////////////////////////////////////////////////////////////
				
			case StateChar:
				switch( c )
				{
					case NUL:
						THROW( ExSyntaxError );
						
					case '\'':
						state = StateChar2;
						break;
						
					default:
						n.low_byte = c;
						state = StateChar3;
						break;
				}
				
				break;
				
			case StateChar2:
				switch( c )
				{
					case NUL:
						THROW( ExSyntaxError );
						
					case '\'':
						n.low_byte = c;
						state = StateChar3;
						break;
						
					default:
						THROW( ExSyntaxError );
				}
				
				break;
				
			case StateChar3:
				switch( c )
				{
					case NUL:
						THROW( ExSyntaxError );
						
					case '\'':
						state = StateChar4;
						break;
						
					default:
						n.high_byte = n.low_byte;
						n.low_byte = c;
						break;
				}
				
				break;
				
			case StateChar4:
				switch( c )
				{
					case NUL:
						return n.word;
						
					case '\'':
						n.high_byte = n.low_byte;
						n.low_byte = c;
						state = StateChar3;
						break;
						
					default:
						THROW( ExSyntaxError );
				}
				
				break;
				
			////////////////////////////////////////////////////////////
				
			case StateSymbol:
				switch( c )
				{
					case NUL:
						*psym = NUL;
						return symtab_get_may_throw( symbol );
						
					case SYMBOL_NEXT:
						if( psym < symbol + sizeof( symbol ) - 1 )
							*psym++ = c;
						
						break;
						
					default:
						THROW( ExInvalidSymbolName );
				}
				
				break;
		}
	}
}
