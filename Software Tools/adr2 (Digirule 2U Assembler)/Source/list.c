#include "adr2.h"

#define LINES_PER_PAGE		60

char list_path[4096] = "adr2.lst";
list_line_t list_line;

static struct
{
	FILE  *fp;
	time_t time;
	int    page, line, lc_done;
	char  *pobj;
	uint8  dirty[RAM_SIZE];
} priv;

////////////////////////////////////////////////////////////////////////////////

void list_open( void )
{
	if( (priv.fp = fopen( list_path, "wb" )) == NULL )
	{
		fprintf( stderr, "adr2: cannot create listing file\n" );
		exit( 3 );
	}

	priv.time = time( NULL );
	priv.page = priv.line = 0;
}

void list_page_eject( void )
{
	strcpy( list_line.buf, "" );
	priv.line += LINES_PER_PAGE - priv.line % LINES_PER_PAGE;
}

void list_line_open( void )
{
	char temp[16];
	
	strcpy( list_line.buf, "    " "  " "        " "  " "     ." "  " );
	sprintf( temp, "%10u", source_lineno );
	memcpy( list_line.lineno, temp + 5, 5 );
	priv.lc_done = 0;
	priv.pobj = list_line.object;
}

void list_line_append( void )
{
	list_line_close();
	
	strcpy( list_line.buf, "    " "  " "        " );
	priv.pobj = list_line.object;
}

void list_lc( void )
{
	char temp[16];
	
	if( !priv.lc_done )
	{
		sprintf( temp, "%04X", lc );
		memcpy( list_line.lc, temp, 4 );
		priv.lc_done = 1;
	}
}

void list_emit8( uint8 data )
{
	char temp[16];
	
	list_lc();
	
	sprintf( temp, "%02X", data );
	
	if( priv.pobj + 2 > list_line.object_end )
		list_line_append();
	
	memcpy( priv.pobj, temp, 2 );
	priv.pobj += 2 + 1;
	
	if( lc < ELEMENTSOF( priv.dirty ) )
		priv.dirty[lc] = 1;
}

void list_emit_equ( uint16 data )
{
	char temp[16];
	
	sprintf( temp, "%04X", data );
	memcpy( list_line.equ, temp, 4 );
}

void list_line_close( void )
{
	char *p;
	
	if( *list_line.buf )
	{
		for( p = list_line.buf; *p != NUL; p++ )
			;
		
		while( p != list_line.buf && *--p == ' ' )		// trim trailing spaces
			*p = NUL;
		
		listf( "%s\n", list_line.buf );
	}
}

void listf( const char *format, ... )
{
	va_list ap;
	char temp[1024], *p;

	if( priv.fp && flags.pass2 && !flags.dir.nolist )
	{
		va_start( ap, format );
		vsprintf( temp, format, ap );
		va_end( ap );
		
		for( p = temp; *p != NUL; p++ )
		{
			if( !flags.dir.nopage && (priv.line % LINES_PER_PAGE) == 0 )
			{
				if( priv.line != 0 )
					fputc( '\f', priv.fp );
				
				fprintf( priv.fp, "%s | %.24s | Page %u" "\r\n\r\n", TITLE, asctime( localtime( &priv.time ) ), ++priv.page );
				priv.line += 2;
			}
			
			switch( *p )
			{
				default:
					fputc( *p, priv.fp );
					break;
					
				case '\r':
					break;
					
				case '\n':
					fputs( "\r\n", priv.fp );
					priv.line++;
					break;
			}
		}
	}
}

void list_close( void )
{
	uint16 address;
	
	if( priv.fp )
	{
		flags.dir.nolist = 0;			// resume listing in case disabled
		
		listf( "\nAssembly complete\n" );
		listf( "%u error%s\n", total_errors, (total_errors == 1) ? "" : "s" );
		listf( "%u warning%s\n", total_warnings, (total_warnings == 1) ? "" : "s" );
		listf( "\n\n" );
		
		symtab_dump();
		listf( "\n\n" );
		
		listf( "RAM Usage"				"\n"
			   "----------------------"	"\n" );

		for( address = 0; address < RAM_SIZE; address++ )
		{
			if( (address & 15) == 0 )
				listf( "%04X  ", address );
			
			listf( "%c", priv.dirty[address] ? 'X' : '.' );
			
			if( (address & 15) == 15 )
				listf( "\n" );
		}
		
		listf( "\n" );
		
		fclose( priv.fp );
	}
}
