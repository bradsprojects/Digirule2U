#include "adr2.h"

static void char_const( const char *s );

void dir_byte( operation_info_t *poi )
{
	uint16 n;
	
	label_optional();
	
	if( parse_get_operand( fields.operand ) )
	{
		do
		{
			if( *fields.operand == '\'' )
				char_const( fields.operand + 1 );
			
			else
			{
				n = 0;
				eval_expr( fields.operand, &n );
				object->emit8( is_unsigned8( n ) );
			}
		} while( parse_get_operand( fields.operand ) );
	}
	else
		error( msg_operand_required );
}

static void char_const( const char *s )
{
	enum
	{
		StateChar,
		StateChar2,
		StateChar3,
		StateChar4,
	} state;
	char c;
	
	state = StateChar;

	for( ;; )
	{
		c = *s++;
		
		switch( state )
		{
			case StateChar:
				switch( c )
				{
					case NUL:
						error( msg_char_const_not_closed );
						return;
						
					case '\'':
						state = StateChar2;
						break;
						
					default:
						object->emit8( c );
						state = StateChar3;
						break;
				}
				
				break;
				
			case StateChar2:
				switch( c )
				{
					case NUL:
						error( msg_char_const_empty );
						return;
						
					case '\'':
						object->emit8( c );
						state = StateChar3;
						break;
						
					default:
						error( msg_char_const_empty );
						error( msg_char_const_extraneous );
						return;
				}
				
				break;
				
			case StateChar3:
				switch( c )
				{
					case NUL:
						error( msg_char_const_not_closed );
						return;
						
					case '\'':
						state = StateChar4;
						break;
						
					default:
						object->emit8( c );
						break;
				}
				
				break;
				
			case StateChar4:
				switch( c )
				{
					case NUL:
						return;
						
					case '\'':
						object->emit8( c );
						state = StateChar3;
						break;
						
					default:
						error( msg_char_const_extraneous );
						return;
				}
				
				break;
		}
	}
}
