#include "adr2.h"

#define BYTE_COUNT_MAX	16

typedef enum { RecordTypeData, RecordTypeEndOfFile } record_type_t;

static struct
{
	char  buf[256], *pbuf;
	uint8 byte_count, checksum;
	int   lc;
} priv;

////////////////////////////////////////////////////////////////////////////////

static void emit_char( char c )
{
	*priv.pbuf++ = c;
}

static void emit_nibble( uint8 data )
{
	emit_char( "0123456789ABCDEF" [data & 0xF] );
}

static void emit_byte( uint8 data )
{
	emit_nibble( data >> 4 );
	emit_nibble( data );
	
	priv.checksum += data;
}

static void line_open( record_type_t type, uint16 address )
{
	priv.checksum = 0;
	
	priv.pbuf = priv.buf;
	emit_char( ':' );
	emit_byte( priv.byte_count );
	emit_byte( address >> 8 );
	emit_byte( address );
	emit_byte( type );
}

static void line_close( void )
{
	priv.pbuf = priv.buf + 1;
	emit_byte( priv.byte_count );
	priv.pbuf = priv.buf + 1 + 2 + 4 + 2 + 2 * priv.byte_count;
	emit_byte( -priv.checksum );
	emit_char( '\r' );
	emit_char( '\n' );
	fwrite( priv.buf, 1, priv.pbuf - priv.buf, stdout );
	
	priv.byte_count = 0;
}

////////////////////////////////////////////////////////////////////////////////

static void open( void )
{
	priv.lc = -1;
}

static void emit8( uint8 data )
{
	list_emit8( data );
	
	if( lc >= RAM_SIZE )
		error( msg_address_exceeds_ram, lc );
	
	else if( flags.pass2 )
	{
		if( lc != priv.lc || priv.byte_count == BYTE_COUNT_MAX )
		{
			if( priv.byte_count )
				line_close();
			
			line_open( RecordTypeData, lc );
		}
		
		emit_byte( data );
		priv.byte_count++;
	}
	
	lc++;
	
	if( flags.pass2 )
		priv.lc = lc;
}

static void close( void )
{
	if( priv.byte_count )
		line_close();
	
	line_open( RecordTypeEndOfFile, end_address );
	line_close();
}

const object_t object_hex = { open, emit8, close };
