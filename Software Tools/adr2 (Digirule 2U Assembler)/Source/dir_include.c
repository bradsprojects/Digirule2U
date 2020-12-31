#include "adr2.h"

void dir_include( operation_info_t *poi )
{
	label_prohibited();
	
	if( !parse_get_operand( fields.operand ) )
		error( msg_operand_required );
	
	else if( !source_push_file( fields.operand ) )
		error( msg_cannot_open_include_file, fields.operand );
}
