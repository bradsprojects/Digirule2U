#if defined( __APPLE__ ) || defined( __linux__ )

#include "udr2.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

static int port;
static struct termios config, original_config;

void port_open( const char *device )
{
	fprintf( stderr, "Opening port..." );
	
	if( (port = open( device, O_RDWR | O_NOCTTY | O_NONBLOCK )) == -1 )
	{
		fprintf( stderr, " FAILED!\a\n" );
		exit( 1 );
	}
	
	ioctl( port, TIOCEXCL );							// prevent additional opens
	fcntl( port, F_SETFL, 0 );							// clear the O_NONBLOCK flag so subsequent I/O will block
	
	tcgetattr( port, &config );							// get current config
	original_config = config;							// save for later restoration
	cfmakeraw( &config );								// raw mode (disable all input and output processing and select 8/N)
	cfsetspeed( &config, B38400 );						// 38400 baud
	config.c_cflag &= ~CSIZE; config.c_cflag |= CS8;	// 8 data bits
	config.c_cflag &= ~PARENB;							// no parity
	config.c_cflag &= ~CSTOPB;							// 1 stop bit
	config.c_cflag |= CLOCAL;							// ignore modem status lines
	config.c_cc[VMIN] = 0;
	config.c_cc[VTIME] = 50;							// timeout (in 100 mS increments)
	tcsetattr( port, TCSANOW, &config );				// set new config
	
	tcflush( port, TCIOFLUSH );							// flush input and output
	
	fprintf( stderr, " OK\n" );
}

void port_read( void *buffer, size_t size )
{
	while( size )
	{
		ssize_t actual = read( port, buffer, size );
		
		if( actual <= 0 )								// if error, or no data read and timeout
		{
			fprintf( stderr, " FAILED!\a (read error)\n" );
			exit( 1 );
		}
		
		buffer = (uint8 *) buffer + actual;
		size -= actual;
	}
}

void port_write( const void *buffer, size_t size )
{
	while( size )
	{
		ssize_t actual = write( port, buffer, size );
		
		if( actual <= 0 )								// if error, or no data written
		{
			fprintf( stderr, " FAILED!\a (write error)\n" );
			exit( 1 );
		}
		
		buffer = (uint8 *) buffer + actual;
		size -= actual;
	}
}

void port_close( void )
{
	fprintf( stderr, "Closing port..." );
	
	tcdrain( port );									// block until drained
	sleep( 3 );											// allow time for UART to finish transmitting buffered data
	tcsetattr( port, TCSANOW, &original_config );		// restore original config
	close( port );
	
	fprintf( stderr, " OK\n" );
}

#endif
