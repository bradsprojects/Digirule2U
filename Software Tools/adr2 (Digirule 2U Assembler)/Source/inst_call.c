#include "adr2.h"

void inst_call( operation_info_t *poi )
{
	inst_2byte( poi );
	
	if( flags.pass2 && !flags.initsp )
		warning( "CALL/CALLI without INITSP" );
}
