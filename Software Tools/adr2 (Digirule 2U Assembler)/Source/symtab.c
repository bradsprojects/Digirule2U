#include "adr2.h"

// the symbol table is implemented as a dynamically-allocated singly-linked list.
// new nodes are inserted alphabetically for the benefit of the listing file dump.

typedef struct node
{
	struct node *next;
	char        *symbol;
	uint16       value;
	int          lineno;
} node_t;

static node_t head;

////////////////////////////////////////////////////////////////////////////////

static int name_invalid( const char *symbol )
{
	enum
	{
		StateFirst,
		StateNext,
	} state;
	char c;
	
	state = StateFirst;
	
	for( ;; )
	{
		c = *symbol++;
		
		switch( state )
		{
			case StateFirst:
				switch( c )
				{
					case SYMBOL_FIRST:
						state = StateNext;
						break;
						
					default:
						return 1;
				}
				
				break;
				
			case StateNext:
				switch( c )
				{
					case NUL:
						return 0;
						
					case SYMBOL_NEXT:
						break;
						
					default:
						return 1;
				}
				
				break;
		}
	}
}

symtab_t symtab_put( const char *symbol, uint16 value )
{
	node_t *node, *newnode;
	
	if( name_invalid( symbol ) )
		return SymTabNameInvalid;
	
	// check if symbol already in table
	
	for( node = head.next; node != NULL; node = node->next )
	{
		if( strcmp( symbol, node->symbol ) == 0 )
		{
			// symbol found, check for duplicate (pass 1 or 2) or phase error (pass 2)
			
			if( node->lineno != source_lineno )
				return SymTabDuplicate;
			
			if( node->value != value )
				return SymTabDifferent;
			
			return SymTabOK;
		}
	}

	// symbol not found, add it (pass 1)
	
	// create new node
	newnode = malloc( sizeof( node_t ) );
	newnode->symbol = strdup( symbol );
	newnode->value  = value;
	newnode->lineno = source_lineno;

	// seek to insertion point
	for( node = &head; node->next != NULL && strcasecmp( node->next->symbol, newnode->symbol ) < 0; node = node->next )
		;

	// insert new node
	newnode->next = node->next;
	node->next = newnode;
	
	return SymTabOK;
}

uint16 symtab_get_may_throw( const char *symbol )
{
	node_t *node;
	
	if( name_invalid( symbol ) )
		THROW( ExInvalidSymbolName );
	
	for( node = head.next; node != NULL; node = node->next )
		if( strcmp( symbol, node->symbol ) == 0 )
			return node->value;
	
	THROW( ExUndefinedSymbol );
}

void symtab_dump( void )
{
	node_t *node;
	char temp[16];
	
	listf( "Line   Val   Symbol"												"\n"
		   "-----  ----  --------------------------------------------------"	"\n" );
	
	for( node = head.next; node != NULL; node = node->next )
	{
		sprintf( temp, "%10u", node->lineno );
		listf( "%s  %04X  %-50.50s\n", temp + 5, node->value, node->symbol );
	}
}
