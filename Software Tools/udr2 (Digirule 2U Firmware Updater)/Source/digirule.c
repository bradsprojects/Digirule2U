#include "udr2.h"

#define CTRL_D		"\x04"
#define CTRL_E		"\x05"
#define CTRL_R		"\x12"
#define CTRL_W		"\x17"

#define FLASH_START	0x000000
#define FLASH_APP	0x000800
#define FLASH_END	0x010000

static address_t address;
static uint8 dummy;

static void erase( void )
{
	fprintf( stderr, "Erasing flash..." );
	port_write( CTRL_E, 1 );
	port_read( &dummy, 1 );				// wait for operation to complete
	fprintf( stderr, " OK\n" );
}

static void write_( buffer_t *buffer )
{
	fprintf( stderr, "Writing flash" );
	port_write( CTRL_W, 1 );
	
	for( address.word = FLASH_APP; address.word != FLASH_END; address.word += 64 )
	{
		port_write( buffer->byte + address.word, 64 );
		port_read( &dummy, 1 );			// wait for operation to complete
		
		if( (address.word & 0x7FF) == 0 )
			fprintf( stderr, "." );
	}
	
	fprintf( stderr, " OK\n" );
}

static void verify( buffer_t *buffer )
{
	static buffer_t read_buffer;
	
	fprintf( stderr, "Verifying flash" );
	port_write( CTRL_R, 1 );
	
	for( address.word = FLASH_START; address.word != FLASH_END; address.word += 2048 )
	{
		port_read( read_buffer.byte + address.word, 2048 );
		fprintf( stderr, "." );
	}

	fprintf( stderr, " %s\n", memcmp( buffer->byte, read_buffer.byte, sizeof( buffer->byte ) ) ? "FAILED!\a" : "OK" );
}

static void restart( void )
{
	fprintf( stderr, "Restarting Digirule..." );
	port_write( CTRL_D, 1 );
	fprintf( stderr, " OK\n" );
}

void digirule_update( const char *device, buffer_t *buffer, bool program )
{
	port_open( device );
	
	if( program )
	{
		erase();
		write_( buffer );
		verify( buffer );
		restart();
	}
	else
		verify( buffer );
	
	port_close();
}
