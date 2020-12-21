#if defined( _WIN32 )

#include "udr2.h"
#include <windows.h>

static HANDLE port;
static DCB dcb, original_dcb;
static COMMTIMEOUTS to, original_to;

void port_open( const char *device )
{
	fprintf( stderr, "Opening port..." );

	if( (port = CreateFile( device, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL )) == INVALID_HANDLE_VALUE )
	{
		fprintf( stderr, " FAILED!\a\n" );
		exit( 1 );
	}

	GetCommState( port, &dcb );
	original_dcb = dcb;
	dcb.BaudRate        = 38400;
	dcb.ByteSize        = 8;
	dcb.Parity          = NOPARITY;
	dcb.StopBits        = ONESTOPBIT;
	dcb.fBinary         = 1;						// binary mode
	dcb.fParity         = 0;						// disable parity checking for input
	dcb.fErrorChar      = 0;						// disable parity error character substitution
	dcb.fAbortOnError   = 0;						// disable termination on error
	dcb.fNull           = 0;						// disable NUL character discard
	dcb.fInX            = 0;						// disable X-ON/X-OFF for input
	dcb.fOutX           = 0;						// disable X-ON/X-OFF for output
	dcb.fDsrSensitivity = 0;						// ignore DSR for input
	dcb.fOutxDsrFlow    = 0;						// ignore DSR for output
	dcb.fOutxCtsFlow    = 0;						// ignore CTS for output
	dcb.fDtrControl     = DTR_CONTROL_DISABLE;		// negate DTR
	dcb.fRtsControl     = RTS_CONTROL_DISABLE;		// negate RTS
	SetCommState( port, &dcb );

	GetCommTimeouts( port, &to );
	original_to = to;
	to.ReadIntervalTimeout         = 0;
	to.ReadTotalTimeoutMultiplier  = 0;
	to.ReadTotalTimeoutConstant    = 5000;
	to.WriteTotalTimeoutMultiplier = 0;
	to.WriteTotalTimeoutConstant   = 0;
	SetCommTimeouts( port, &to );

	fprintf( stderr, " OK\n" );
}

void port_read( void *buffer, size_t size )
{
	DWORD actual;

	if( !ReadFile( port, buffer, size, &actual, NULL ) || actual != size )
	{
		fprintf( stderr, " FAILED!\a\n" );
		exit( 1 );
	}
}

void port_write( const void *buffer, size_t size )
{
	DWORD actual;

	if( !WriteFile( port, buffer, size, &actual, NULL ) || actual != size )
	{
		fprintf( stderr, " FAILED!\a\n" );
		exit( 1 );
	}
}

void port_close( void )
{
	fprintf( stderr, "Closing port..." );

	SetCommState( port, &original_dcb );
	SetCommTimeouts( port, &original_to );
	CloseHandle( port );

	fprintf( stderr, " OK\n" );
}

#endif
