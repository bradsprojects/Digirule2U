#include "adr2.h"

static struct
{
	uint8 ram[256];
} priv;

////////////////////////////////////////////////////////////////////////////////

static void open( void )
{
}

static void emit8( uint8 data )
{
	list_emit8( data );
	
	if( lc >= RAM_SIZE )
		error( msg_address_exceeds_ram, lc );
	
	else if( flags.pass2 )
		priv.ram[lc] = data;
	
	lc++;
}

static void close( void )
{
	int i;
	
	printf( "\t{\r\n" );
	
	for( i = 0; i < sizeof( priv.ram ); i++ )
	{
		if( (i % 8) == 0 )
			printf( "\t\t" );
		
		printf( "0x%02X, ", priv.ram[i] );
		
		if( (i % 8) == 7 )
			printf( "\r\n" );
	}
	
	printf( "\t},\r\n" );
}

const object_t object_src = { open, emit8, close };
