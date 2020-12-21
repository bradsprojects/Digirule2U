#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main( int argc, const char * argv[] )
{
	int port, c;
	struct termios config, original_config;
	
	if( argc != 2 )
	{
		fprintf( stderr, "usage: dldr2 <device>\n" );
		return 1;
	}
	
	if( (port = open( argv[1], O_RDWR | O_NOCTTY | O_NONBLOCK )) == -1 )
	{
		fprintf( stderr, "dldr2: failed to open device\n" );
		return 2;
	}
	
	ioctl( port, TIOCEXCL );							// prevent additional opens
	fcntl( port, F_SETFL, 0 );							// clear the O_NONBLOCK flag so subsequent I/O will block

	tcgetattr( port, &config );							// get current config
	original_config = config;							// save for later restoration
	cfmakeraw( &config );								// raw mode (disable all input and output processing and select 8/N)
	cfsetspeed( &config, B9600 );						// 9600 baud
	config.c_cflag &= ~CSIZE; config.c_cflag |= CS8;	// 8 data bits
	config.c_cflag &= ~PARENB;							// no parity
	config.c_cflag &= ~CSTOPB;							// 1 stop bit
	config.c_cflag |= CLOCAL;							// ignore modem status lines
	tcsetattr( port, TCSANOW, &config );				// set new config
	
	tcflush( port, TCIOFLUSH );							// flush input and output

	while( (c = getchar()) != EOF )						// copy stdin to port
		write( port, &c, 1 );
		
	tcdrain( port );									// block until drained
	sleep( 3 );											// allow time for UART to finish transmitting buffered data
	tcsetattr( port, TCSANOW, &original_config );		// restore original config
	close( port );
	
	return 0;
}
