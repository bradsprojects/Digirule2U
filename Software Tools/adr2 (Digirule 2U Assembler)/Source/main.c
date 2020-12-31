#include "adr2.h"

flags_t flags;
const object_t *object = &object_hex;
jmp_buf wormhole;

int main( int argc, const char * argv[] )
{
	while( *++argv )
	{
		if( strcmp( *argv, "--quiet" ) == 0 )
		{
			flags.cl_opt.quiet = 1;
		}
		else if( strcmp( *argv, "--list" ) == 0 )
		{
			if( *(argv + 1) )
				strcpy( list_path, *++argv );
		}
		else if( strcmp( *argv, "--nolist" ) == 0 )
		{
			flags.cl_opt.nolist = 1;
		}
		else if( strcmp( *argv, "--source" ) == 0 )
		{
			object = &object_src;
		}
		else if( strcmp( *argv, "--debug" ) == 0 )
		{
			flags.cl_opt.debug = 1;
		}
		else	// including --help
		{
			fprintf( stderr,
				BANNER																		"\n"
				"usage: adr2 [options] <sourcefile >hexfile"								"\n"
				"options:"																	"\n"
				"  --quiet          suppress banner and completion info"					"\n"
				"  --nolist         suppress listing file creation"							"\n"
				"  --list <path>    specify listing file pathname (default is adr2.lst)"	"\n"
				"  --source         emit object code in C array initialization form"		"\n"
				"  --help           display this help information"							"\n"
			);
			return 3;
		}
	}
	
	if( !flags.cl_opt.quiet )
		fprintf( stderr, BANNER "\n" );

	object->open();
	
	if( !flags.cl_opt.nolist )
		list_open();
	
	assemble();
	
	object->close();
	list_close();
	
	if( !flags.cl_opt.quiet )
	{
		fprintf( stderr, "Assembly complete" );
		
		if( total_errors )
			fprintf( stderr, ", %u error%s", total_errors, (total_errors == 1) ? "" : "s" );
		
		if( total_warnings )
			fprintf( stderr, ", %u warning%s", total_warnings, (total_warnings == 1) ? "" : "s" );
		
		fprintf( stderr, ".\n" );
	}
	
	return total_errors ? 2 : total_warnings ? 1 : 0;
}
