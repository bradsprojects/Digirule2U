#include "adr2.h"

uint16 lc;

////////////////////////////////////////////////////////////////////////////////

static void pass( void )
{
	operation_info_t oi;
	
	source_reset();
	lc = 0;
	flags.dir.end = flags.dir.nolist = flags.dir.nopage = 0;
	
	do
	{
		source_lineno++;
		list_line_open();
		
		if( !source_line_read() )
			break;
		
		parse_get_label_operation();
		parse_lookup_operation( fields.operation, &oi );
		oi.operation_fn( &oi );
		
		if( parse_get_operand( fields.operand ) )
			warning( msg_extraneous_operand );
		
		list_line_close();
	} while( !flags.dir.end );
}

void assemble( void )
{
	symtab_put( "_sr", SR );		// predefined symbols
	symtab_put( "_br", BR );
	symtab_put( "_ar", AR );
	symtab_put( "_dr", DR );
	symtab_put( "_z", 0 );
	symtab_put( "_c", 1 );
	symtab_put( "_sar", 2 );
	
	pass();
	flags.pass2 = 1;
	pass();
}
