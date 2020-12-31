#include "adr2.h"

void inst_3byte( operation_info_t *poi )
{
	uint16 operand = 0, operand2 = 0;
	
	label_optional();
	
	if( parse_get_operand( fields.operand ) && parse_get_operand( fields.operand2 ) )
	{
		eval_expr( fields.operand, &operand );
		eval_expr( fields.operand2, &operand2 );
		object->emit8( poi->operation );
		object->emit8( is_unsigned8( operand ) );
		object->emit8( is_unsigned8( operand2 ) );
	}
	else
		error( msg_operands_required );
}
