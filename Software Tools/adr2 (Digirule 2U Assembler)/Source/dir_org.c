#include "adr2.h"

void dir_org( operation_info_t *poi )
{
	uint16 operand;
	
	label_prohibited();
	
	if( !parse_get_operand( fields.operand ) )
		error( msg_operand_required );
	
	else if( eval_expr( fields.operand, &operand ) )
	{
		if( operand < lc )
			error( msg_lc_reversal );
		
		else
		{
			lc = operand;
			list_lc();
		}
	}
}
