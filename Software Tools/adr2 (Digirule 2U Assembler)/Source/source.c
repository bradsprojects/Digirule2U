#include "adr2.h"

int source_lineno;

typedef struct node
{
	struct node *prev;
	FILE *fp;
} node_t;

static node_t stack, *sp = &stack;

////////////////////////////////////////////////////////////////////////////////

static void pop( void )
{
	node_t *node;
 
	node = sp;
	fclose( sp->fp );
	sp = sp->prev;
	free( node );
}

void source_reset( void )
{
	while( sp != &stack )
		pop();
	
	sp->fp = stdin;
	
	rewind( sp->fp );
	source_lineno = 0;
}

int source_line_read( void )
{
	char *pline;
	int col, c;
	
	pline = list_line.source;
	col = 0;
	
	for( ;; )
	{
		c = getc( sp->fp );
		
		switch( c )
		{
			case ' ':
			case ALPHA:
			case DIGIT:
			case PUNCT:
				if( pline < list_line.source + sizeof( list_line.source ) - 1 )
					*pline++ = c;
				
				col++;
				break;
				
			case '\t':		// expand tabs to spaces
				do
					if( pline < list_line.source + sizeof( list_line.source ) - 1 )
						*pline++ = ' ';
				
				while( ++col % 8 );
				break;
				
			case EOL:
				*pline = NUL;
				return 1;
				
			case EOF:
				if( col )				// non-blank unterminated line
				{
					*pline = NUL;
					return 1;
				}
				
				if( sp == &stack )		// end of source file
				{
					*pline = NUL;
					return 0;
				}
				
				pop();					// end of include file
				break;
		}
	}
}

int source_push_file( const char *path )
{
	FILE *fp;
	node_t *node;
	
	if( (fp = fopen( path, "rb" )) == NULL )
		return 0;
	
	node = malloc( sizeof( node_t ) );
	node->fp = fp;
	node->prev = sp;
	sp = node;
	
	return 1;
}
