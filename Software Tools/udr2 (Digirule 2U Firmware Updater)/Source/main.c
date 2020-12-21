#include "udr2.h"

#define TITLE	"Digirule 2U Firmware Updater"
#define BUILT	"18 Dec 2020"
#define BY		"KD0GLS"
#define BANNER	TITLE " | " BUILT " | " BY

static void help( void )
{
	fprintf( stderr,
		BANNER											"\n"
		"usage: udr2 [options] device <hexfile"			"\n"
		"options:"										"\n"
		"  --program    program and verify (default)"	"\n"
		"  --verify     verify only"					"\n"
	);
	exit( 1 );
}

int main( int argc, const char * argv[] )
{
	static char device[1024];
	static buffer_t buffer;
	static bool program = 1;
	
	while( *++argv )
	{
		if( **argv == '-' )
		{
			if( strcmp( *argv, "--program" ) == 0 )
				program = 1;
			
			else if( strcmp( *argv, "--verify" ) == 0 )
				program = 0;
			
			else
				help();
		}
		else
			strcpy( device, *argv );
	}
	
	if( *device == '\0' )
		help();

	fprintf( stderr, BANNER "\n" );
	
	memset( buffer.byte, 0xFF, sizeof( buffer.byte ) );
	hexfile_load( &buffer );
	digirule_update( device, &buffer, program );
	
	return 0;
}
