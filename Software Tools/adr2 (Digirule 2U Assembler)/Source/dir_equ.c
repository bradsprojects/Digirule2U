#include "adr2.h"

void dir_equ( operation_info_t *poi )
{
	uint16 operand;
	
	if( !*fields.label )
		error( msg_label_required );
	
	if( !parse_get_operand( fields.operand ) )
		error( msg_operand_required );
	
	if( *fields.label && *fields.operand && eval_expr( fields.operand, &operand ) )
	{
		switch( symtab_put( fields.label, operand ) )
		{
			case SymTabOK:
				list_emit_equ( operand );
				break;
				
			case SymTabNameInvalid:
				error( msg_invalid_symbol_name, fields.label );
				return;
				
			case SymTabDuplicate:
				error( msg_duplicate_symbol, fields.label );
				return;
				
			case SymTabDifferent:
				error( msg_phase_error, fields.label );
				return;
		}
	}
}
