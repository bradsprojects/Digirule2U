#include "adr2.h"

int total_errors, total_warnings;

////////////////////////////////////////////////////////////////////////////////

void error( const char *format, ... )
{
	va_list ap;
	char temp[1024];
	
	if( flags.pass2 )
	{
		va_start( ap, format );
		vsprintf( temp, format, ap );
		va_end( ap );
		
		fprintf( stderr, "adr2: error: line %u: %s\n", source_lineno, temp );
		listf( "----> error: line %u: %s\n", source_lineno, temp );
		
		total_errors++;
	}
}

void warning( const char *format, ... )
{
	va_list ap;
	char temp[1024];
	
	if( flags.pass2 )
	{
		va_start( ap, format );
		vsprintf( temp, format, ap );
		va_end( ap );
		
		fprintf( stderr, "adr2: warning: line %u: %s\n", source_lineno, temp );
		listf( "----> warning: line %u: %s\n", source_lineno, temp );
		
		total_warnings++;
	}
}
