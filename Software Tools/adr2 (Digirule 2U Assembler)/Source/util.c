#include "adr2.h"

uint16 is_unsigned8( uint16 n )
{
	if( n > 255 )
		warning( msg_value_truncated );
	
	return n;
}
