#include "adr2.h"

void dir_space( operation_info_t *poi )
{
	uint16 operand;
	
	label_optional();
	
	if( !parse_get_operand( fields.operand ) )
		error( msg_operand_required );
	
	else if( eval_expr( fields.operand, &operand ) )
	{
		if( operand < 1 || operand > 255 )
			error( msg_invalid_space_operand );
		
		else
			lc += operand;
	}
}
