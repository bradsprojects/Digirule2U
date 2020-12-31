#include "adr2.h"

void inst_initsp( operation_info_t *poi )
{
	inst_1byte( poi );
	
	if( !flags.pass2 )
		flags.initsp = 1;
}
