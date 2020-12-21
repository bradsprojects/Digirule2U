#include "digirule2.h"

#define RECORDTYPE_ERROR	data_leds.d2
#define CHECKSUM_ERROR		data_leds.d1
#define NONHEXCHAR_ERROR	data_leds.d0

static void load_state_start( char c );
static void load_state_byte_count( char c );
static void load_state_byte_count2( char c );
static void load_state_address( char c );
static void load_state_address2( char c );
static void load_state_address3( char c );
static void load_state_address4( char c );
static void load_state_record_type( char c );
static void load_state_record_type2( char c );
static void load_state_data( char c );
static void load_state_data2( char c );
static void load_state_data3( char c );
static void load_state_data4( char c );
static void load_state_checksum( char c );
static void load_state_checksum2( char c );
static void load_state_done( char c );

static void save_state_start( void );
static void save_state_byte_count( void );
static void save_state_byte_count2( void );
static void save_state_address( void );
static void save_state_address2( void );
static void save_state_address3( void );
static void save_state_address4( void );
static void save_state_record_type( void );
static void save_state_record_type2( void );
static void save_state_data( void );
static void save_state_data2( void );
static void save_state_checksum( void );
static void save_state_checksum2( void );
static void save_state_end_of_line( void );
static void save_state_end_of_line2( void );
static void save_state_end_of_file( void );
static void save_state_end_of_file2( void );
static void save_state_end_of_file3( void );
static void save_state_end_of_file4( void );
static void save_state_end_of_file5( void );
static void save_state_end_of_file6( void );
static void save_state_end_of_file7( void );
static void save_state_end_of_file8( void );
static void save_state_end_of_file9( void );
static void save_state_end_of_file10( void );
static void save_state_end_of_file11( void );
static void save_state_end_of_file12( void );
static void save_state_end_of_file13( void );
static void save_state_done( void );

enum { RecordTypeData, RecordTypeEndOfFile };

///////////////////////////////////////////////////////////////////////////////
// public data

uint16 end_of_file_record_address;

///////////////////////////////////////////////////////////////////////////////
// private data

static bool_t eof;
static union8_t byte_count, record_type, data, checksum;
static union16_t address;
static uint16 blink_timer;
static void (*load_state)( char c );
static void (*save_state)( void );

static struct
{
	volatile char *put, *get, buf[RX_FIFO_SIZE];
} rx_fifo = { rx_fifo.buf, rx_fifo.buf };

///////////////////////////////////////////////////////////////////////////////
// public functions

void comm_load_init( void )
{
	comm_rx_flush();
	eof = 0;
	blink_timer = 0;
	load_state = load_state_start;
}

void comm_load_process( void )
{
	if( comm_rx_ready() )
		load_state( comm_rx() );

	if( t1ms && ++blink_timer == 500 )
	{
		LISTENING_LED ^= 1;
		blink_timer = 0;
	}
}

///////////////////////////////////////////////////////////

void comm_save_init( void )
{
	address.word = 0;
	blink_timer = 0;
	save_state = save_state_start;
}

void comm_save_process( void )
{
	if( TRMT )
		save_state();
	
	if( t1ms && ++blink_timer == 125 )
	{
		TALKING_LED ^= 1;
		blink_timer = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
// private functions

static void load_state_start( char c )
{
	switch( c )
	{
		case ':':
			checksum.byte = 0;
			load_state = load_state_byte_count;
			break;
	}
}

static void load_state_byte_count( char c )
{
	switch( c )
	{
		case DIGIT:
			byte_count.high_nibble = c - '0';
			break;

		case XDIGIT:
			byte_count.high_nibble = c - 'A' + 10;
			break;

		default:
			byte_count.high_nibble = 0;
			NONHEXCHAR_ERROR = 1;
			break;
	}

	load_state = load_state_byte_count2;
}

static void load_state_byte_count2( char c )
{
	switch( c )
	{
		case DIGIT:
			byte_count.low_nibble = c - '0';
			break;

		case XDIGIT:
			byte_count.low_nibble = c - 'A' + 10;
			break;

		default:
			byte_count.low_nibble = 0;
			NONHEXCHAR_ERROR = 1;
			break;
	}

	checksum.byte += byte_count.byte;
	load_state = load_state_address;
}

static void load_state_address( char c )
{
	switch( c )
	{
		case DIGIT:
			address.high_high_nibble = c - '0';
			break;

		case XDIGIT:
			address.high_high_nibble = c - 'A' + 10;
			break;

		default:
			address.high_high_nibble = 0;
			NONHEXCHAR_ERROR = 1;
			break;
	}

	load_state = load_state_address2;
}

static void load_state_address2( char c )
{
	switch( c )
	{
		case DIGIT:
			address.high_low_nibble = c - '0';
			break;

		case XDIGIT:
			address.high_low_nibble = c - 'A' + 10;
			break;

		default:
			address.high_low_nibble = 0;
			NONHEXCHAR_ERROR = 1;
			break;
	}

	checksum.byte += address.high_byte;
	load_state = load_state_address3;
}

static void load_state_address3( char c )
{
	switch( c )
	{
		case DIGIT:
			address.low_high_nibble = c - '0';
			break;

		case XDIGIT:
			address.low_high_nibble = c - 'A' + 10;
			break;

		default:
			address.low_high_nibble = 0;
			NONHEXCHAR_ERROR = 1;
			break;
	}

	load_state = load_state_address4;
}

static void load_state_address4( char c )
{
	switch( c )
	{
		case DIGIT:
			address.low_low_nibble = c - '0';
			break;

		case XDIGIT:
			address.low_low_nibble = c - 'A' + 10;
			break;

		default:
			address.low_low_nibble = 0;
			NONHEXCHAR_ERROR = 1;
			break;
	}

	checksum.byte += address.low_byte;
	load_state = load_state_record_type;
}

static void load_state_record_type( char c )
{
	switch( c )
	{
		case DIGIT:
			record_type.high_nibble = c - '0';
			break;

		case XDIGIT:
			record_type.high_nibble = c - 'A' + 10;
			break;

		default:
			record_type.high_nibble = 0;
			NONHEXCHAR_ERROR = 1;
			break;
	}

	load_state = load_state_record_type2;
}

static void load_state_record_type2( char c )
{
	switch( c )
	{
		case DIGIT:
			record_type.low_nibble = c - '0';
			break;

		case XDIGIT:
			record_type.low_nibble = c - 'A' + 10;
			break;

		default:
			record_type.low_nibble = 0;
			NONHEXCHAR_ERROR = 1;
			break;
	}

	checksum.byte += record_type.byte;

	switch( record_type.byte )
	{
		case RecordTypeData:
			if( byte_count.byte != 0 )
				load_state = load_state_data;
			else
				load_state = load_state_checksum;

			break;

		case RecordTypeEndOfFile:
			eof = 1;

			if( byte_count.byte != 0 )
				load_state = load_state_data3;
			else
				load_state = load_state_checksum;

			break;

		default:
			RECORDTYPE_ERROR = 1;

			if( byte_count.byte != 0 )
				load_state = load_state_data3;
			else
				load_state = load_state_checksum;

			break;
	}
}

static void load_state_data( char c )
{
	switch( c )
	{
		case DIGIT:
			data.high_nibble = c - '0';
			break;

		case XDIGIT:
			data.high_nibble = c - 'A' + 10;
			break;

		default:
			data.high_nibble = 0;
			NONHEXCHAR_ERROR = 1;
			break;
	}

	load_state = load_state_data2;
}

static void load_state_data2( char c )
{
	switch( c )
	{
		case DIGIT:
			data.low_nibble = c - '0';
			break;

		case XDIGIT:
			data.low_nibble = c - 'A' + 10;
			break;

		default:
			data.low_nibble = 0;
			NONHEXCHAR_ERROR = 1;
			break;
	}

	if( address.word < sizeof( ram.program ) )
	{
		address_leds = PROGRESS_BAR( address.low_byte );
		ram.program[address.word++] = data.byte;
	}

	checksum.byte += data.byte;

	if( --byte_count.byte != 0 )
		load_state = load_state_data;
	else
		load_state = load_state_checksum;
}

static void load_state_data3( char c )
{
	switch( c )
	{
		case DIGIT:
			data.high_nibble = c - '0';
			break;

		case XDIGIT:
			data.high_nibble = c - 'A' + 10;
			break;

		default:
			data.high_nibble = 0;
			NONHEXCHAR_ERROR = 1;
			break;
	}

	load_state = load_state_data4;
}

static void load_state_data4( char c )
{
	switch( c )
	{
		case DIGIT:
			data.low_nibble = c - '0';
			break;

		case XDIGIT:
			data.low_nibble = c - 'A' + 10;
			break;

		default:
			data.low_nibble = 0;
			NONHEXCHAR_ERROR = 1;
			break;
	}

	checksum.byte += data.byte;

	if( --byte_count.byte != 0 )
		load_state = load_state_data3;
	else
		load_state = load_state_checksum;
}

static void load_state_checksum( char c )
{
	switch( c )
	{
		case DIGIT:
			data.high_nibble = c - '0';
			break;

		case XDIGIT:
			data.high_nibble = c - 'A' + 10;
			break;

		default:
			data.high_nibble = 0;
			NONHEXCHAR_ERROR = 1;
			break;
	}

	load_state = load_state_checksum2;
}

static void load_state_checksum2( char c )
{
	switch( c )
	{
		case DIGIT:
			data.low_nibble = c - '0';
			break;

		case XDIGIT:
			data.low_nibble = c - 'A' + 10;
			break;

		default:
			data.low_nibble = 0;
			NONHEXCHAR_ERROR = 1;
			break;
	}

	checksum.byte += data.byte;

	if( checksum.byte != 0 )
		CHECKSUM_ERROR = 1;

	if( !eof )
		load_state = load_state_start;
	else
	{
		end_of_file_record_address = address.word;
		latc.led.run = 0;
		load_state = load_state_done;
	}
}

static void load_state_done( char c )
{
}

///////////////////////////////////////////////////////////

static void save_state_start( void )
{
	TXREG = ':';
	checksum.byte = 0;
	byte_count.byte = (uint8) MIN( sizeof( ram.program ) - address.word, 16 );
	save_state = save_state_byte_count;
}

static void save_state_byte_count( void )
{
	TXREG = hex[byte_count.high_nibble];
	save_state = save_state_byte_count2;
}

static void save_state_byte_count2( void )
{
	TXREG = hex[byte_count.low_nibble];
	checksum.byte += byte_count.byte;
	save_state = save_state_address;
}

static void save_state_address( void )
{
	TXREG = hex[address.high_high_nibble];
	save_state = save_state_address2;
}

static void save_state_address2( void )
{
	TXREG = hex[address.high_low_nibble];
	checksum.byte += address.high_byte;
	save_state = save_state_address3;
}

static void save_state_address3( void )
{
	TXREG = hex[address.low_high_nibble];
	save_state = save_state_address4;
}

static void save_state_address4( void )
{
	TXREG = hex[address.low_low_nibble];
	checksum.byte += address.low_byte;
	record_type.byte = RecordTypeData;
	save_state = save_state_record_type;
}

static void save_state_record_type( void )
{
	TXREG = hex[record_type.high_nibble];
	save_state = save_state_record_type2;
}

static void save_state_record_type2( void )
{
	TXREG = hex[record_type.low_nibble];
	checksum.byte += record_type.byte;

	if( byte_count.byte != 0 )
	{
		address_leds = PROGRESS_BAR( address.low_byte );
		save_state = save_state_data;
	}
	else
		save_state = save_state_checksum;
}

static void save_state_data( void )
{
	data.byte = ram.program[address.word++];
	TXREG = hex[data.high_nibble];
	save_state = save_state_data2;
}

static void save_state_data2( void )
{
	TXREG = hex[data.low_nibble];
	checksum.byte += data.byte;

	if( --byte_count.byte != 0 )
		save_state = save_state_data;
	else
		save_state = save_state_checksum;
}

static void save_state_checksum( void )
{
	checksum.byte = -checksum.byte;
	TXREG = hex[checksum.high_nibble];
	save_state = save_state_checksum2;
}

static void save_state_checksum2( void )
{
	TXREG = hex[checksum.low_nibble];
	save_state = save_state_end_of_line;
}

static void save_state_end_of_line( void )
{
	TXREG = '\r';
	save_state = save_state_end_of_line2;
}

static void save_state_end_of_line2( void )
{
	TXREG = '\n';

	if( address.word < sizeof( ram.program ) )
		save_state = save_state_start;
	else
		save_state = save_state_end_of_file;
}

static void save_state_end_of_file( void )
{
	TXREG = ':';
	save_state = save_state_end_of_file2;
}

static void save_state_end_of_file2( void )
{
	TXREG = '0';
	save_state = save_state_end_of_file3;
}

static void save_state_end_of_file3( void )
{
	TXREG = '0';
	save_state = save_state_end_of_file4;
}

static void save_state_end_of_file4( void )
{
	TXREG = '0';
	save_state = save_state_end_of_file5;
}

static void save_state_end_of_file5( void )
{
	TXREG = '0';
	save_state = save_state_end_of_file6;
}

static void save_state_end_of_file6( void )
{
	TXREG = '0';
	save_state = save_state_end_of_file7;
}

static void save_state_end_of_file7( void )
{
	TXREG = '0';
	save_state = save_state_end_of_file8;
}

static void save_state_end_of_file8( void )
{
	TXREG = '0';
	save_state = save_state_end_of_file9;
}

static void save_state_end_of_file9( void )
{
	TXREG = '1';
	save_state = save_state_end_of_file10;
}

static void save_state_end_of_file10( void )
{
	TXREG = 'F';
	save_state = save_state_end_of_file11;
}

static void save_state_end_of_file11( void )
{
	TXREG = 'F';
	save_state = save_state_end_of_file12;
}

static void save_state_end_of_file12( void )
{
	TXREG = '\r';
	save_state = save_state_end_of_file13;
}

static void save_state_end_of_file13( void )
{
	TXREG = '\n';
	latc.led.run = 0;
	save_state = save_state_done;
}

static void save_state_done( void )
{
}

///////////////////////////////////////////////////////////

void comm_rx_flush( void )
{
	GIE = 0;
	rx_fifo.get = rx_fifo.put;
	GIE = 1;
}

bool_t comm_rx_ready( void )
{
	bool_t state;
	
	GIE = 0;
	state = rx_fifo.get != rx_fifo.put;
	GIE = 1;
	
	return state;
}

char comm_rx( void )
{
	char c;
	
	GIE = 0;

	c = *rx_fifo.get++;

	if( rx_fifo.get == rx_fifo.buf + ELEMENTSOF( rx_fifo.buf ) )
		rx_fifo.get = rx_fifo.buf;

	GIE = 1;
	
	return c;
}

///////////////////////////////////////////////////////////

void __interrupt( high_priority ) __section( "isr_text" ) rx_isr( void )
{
	*rx_fifo.put++ = RCREG;
	
	if( rx_fifo.put == rx_fifo.buf + ELEMENTSOF( rx_fifo.buf ) )
		rx_fifo.put = rx_fifo.buf;
}
