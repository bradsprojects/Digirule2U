#include "adr2.h"

void label_optional( void )
{
	if( *fields.label )
	{
		list_lc();
		
		switch( symtab_put( fields.label, lc ) )
		{
			case SymTabOK:
				break;
				
			case SymTabNameInvalid:
				error( msg_invalid_symbol_name, fields.label );
				break;
				
			case SymTabDuplicate:
				error( msg_duplicate_symbol, fields.label );
				break;
				
			case SymTabDifferent:
				error( msg_phase_error, fields.label );
				break;
		}
	}
}

void label_prohibited( void )
{
	if( *fields.label )
		error( msg_label_prohibited );
}
