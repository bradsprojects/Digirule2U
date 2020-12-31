#include "adr2.h"

void inst_invalid( operation_info_t *poi )
{
	label_optional();
	error( msg_invalid_operation, fields.operation );
	parse_get_line( fields.operand );		// discard operand(s)
}
