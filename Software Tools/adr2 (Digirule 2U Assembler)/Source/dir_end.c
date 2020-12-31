#include "adr2.h"

uint16 end_address;

void dir_end( operation_info_t *poi )
{
	label_prohibited();
	
	list_lc();
	
	end_address = 0;

	if( parse_get_operand( fields.operand ) && eval_expr( fields.operand, &end_address ) )
	{
		if( end_address >= RAM_SIZE )
		{
			warning( "start address exceeds RAM [0x%04X], using 0", end_address );
			end_address = 0;
		}
		
		list_emit_equ( end_address );
	}
	
	flags.dir.end = 1;
}
