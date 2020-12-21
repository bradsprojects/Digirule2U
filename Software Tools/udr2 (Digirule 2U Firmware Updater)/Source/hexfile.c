#include "udr2.h"

static uint8 load_byte( void );
static char load_char( void );

static uint8 checksum;

void hexfile_load( buffer_t *buffer )
{
	bool done;
	uint8 data_byte_count, record_type, data;
	address_t address;
	
	fprintf( stderr, "Loading hex file..." );
	
	address.word = 0;
	
	for( done = 0; !done; )
	{
		while( load_char() != ':' )
			;
		
		checksum = 0;
		
		data_byte_count       = load_byte();
		address.byte.low_high = load_byte();
		address.byte.low_low  = load_byte();
		record_type           = load_byte();
		
		switch( record_type )
		{
			case 4:		// extended address
				if( data_byte_count != 2 )
				{
					fprintf( stderr, " FAILED!\a (unexpected data count)\n" );
					exit( 1 );
				}
				
				address.byte.high_high = load_byte();
				address.byte.high_low = load_byte();
				break;
				
			case 0:		// data
				while( data_byte_count-- )
				{
					data = load_byte();
					
					if( address.word < sizeof( buffer->byte ) )
						buffer->byte[address.word] = data;
					
					address.word++;
				}
				
				break;
				
			case 1:		// end of file
				while( data_byte_count-- )
					load_byte();
				
				done = 1;
				break;
				
			default:
				fprintf( stderr, " FAILED!\a (unexpected record type)\n" );
				exit( 1 );
		}
		
		load_byte();
		
		if( checksum != 0 )
		{
			fprintf( stderr, " FAILED!\a (checksum error)\n" );
			exit( 1 );
		}
	}
	
	fprintf( stderr, " OK\n" );
}

#define DIGIT	'0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9'
#define XDIGIT	'A': case 'B': case 'C': case 'D': case 'E': case 'F'

static uint8 load_byte( void )
{
	char c;
	union
	{
		struct
		{
			uint8 low : 4;
			uint8 high : 4;
		} nibble;
		uint8 byte;
	} n;
	
	c = load_char();
	
	switch( c )
	{
		case DIGIT:
			n.nibble.high = c - '0';
			break;
			
		case XDIGIT:
			n.nibble.high = c - 'A' + 10;
			break;
			
		default:
			fprintf( stderr, " FAILED!\a (unexpected character)\n" );
			exit( 1 );
	}
	
	c = load_char();
	
	switch( c )
	{
		case DIGIT:
			n.nibble.low = c - '0';
			break;
			
		case XDIGIT:
			n.nibble.low = c - 'A' + 10;
			break;
			
		default:
			fprintf( stderr, " FAILED!\a (unexpected character)\n" );
			exit( 1 );
	}

	checksum += n.byte;
	
	return n.byte;
}

static char load_char( void )
{
	int c;
	
	c = getchar();
	
	if( c == EOF )
	{
		fprintf( stderr, " FAILED!\a (unexpected EOF)\n" );
		exit( 1 );
	}
	
	return c;
}
