#include "adr2.h"

void inst_2byte( operation_info_t *poi )
{
	uint16 operand = 0;
	
	label_optional();
	
	if( parse_get_operand( fields.operand ) )
	{
		eval_expr( fields.operand, &operand );
		object->emit8( poi->operation );
		object->emit8( is_unsigned8( operand ) );
	}
	else
		error( msg_operand_required );
}
