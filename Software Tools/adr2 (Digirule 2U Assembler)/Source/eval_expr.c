#include "adr2.h"

typedef enum
{
	TokenEOL,
	TokenLeftParen,
	TokenRightParen,
	TokenNot,
	TokenShiftLeft,
	TokenShiftRight,
	TokenRotateLeft,
	TokenRotateRight,
	TokenAnd,
	TokenXor,
	TokenOr,
	TokenPower,
	TokenMultiply,
	TokenDivide,
	TokenModulo,
	TokenPlus,
	TokenMinus,
	TokenConst,
} token_t;

static struct
{
	const char *ep;
	char        token_buf[256];
	token_t     token;
} priv;

static void level_1( uint16 *pn );

////////////////////////////////////////////////////////////////////////////////

static uint16 power( uint16 base, uint16 exponent )
{
	uint16 n;
	
	if( exponent >= 16 )
		return 0;
	
	n = 1;
	
	while( exponent-- )
		n *= base;
	
	return n;
}

///////////////////////////////////////////////////////////////////////////////

#define OPCHAR	'(': case ')': case '~': case '<': case '>': case '&': case '^': case '|': case '*': case '/': case '%': case '+': case '-'

static token_t get_token_may_throw( void )
{
	enum
	{
		StateFirst,
		StateNext,
		StateLess,
		StateLess2,
		StateGreater,
		StateGreater2,
		StateAsterisk,
		StateQuoteOdd,
		StateQuoteEven,
		StateConst,
	} state;
	char *tp;
	
	tp = priv.token_buf;
	state = StateFirst;
	
	for( ;; )
	{
		switch( state )
		{
			case StateFirst:
				switch( *priv.ep )
				{
					case NUL:
						*tp = NUL;
						return TokenEOL;
						
					case ' ':
						priv.ep++;
						break;
						
					default:
						state = StateNext;
						break;
				}
				
				break;
				
			case StateNext:
				switch( *priv.ep )
				{
					case '(':
						*tp++ = *priv.ep++;
						*tp = NUL;
						return TokenLeftParen;
						
					case ')':
						*tp++ = *priv.ep++;
						*tp = NUL;
						return TokenRightParen;
						
					case '~':
						*tp++ = *priv.ep++;
						*tp = NUL;
						return TokenNot;
						
					case '<':
						*tp++ = *priv.ep++;
						state = StateLess;
						break;
						
					case '>':
						*tp++ = *priv.ep++;
						state = StateGreater;
						break;
						
					case '&':
						*tp++ = *priv.ep++;
						*tp = NUL;
						return TokenAnd;
						
					case '^':
						*tp++ = *priv.ep++;
						*tp = NUL;
						return TokenXor;
						
					case '|':
						*tp++ = *priv.ep++;
						*tp = NUL;
						return TokenOr;
						
					case '*':
						*tp++ = *priv.ep++;
						state = StateAsterisk;
						break;
						
					case '/':
						*tp++ = *priv.ep++;
						*tp = NUL;
						return TokenDivide;
						
					case '%':
						*tp++ = *priv.ep++;
						*tp = NUL;
						return TokenModulo;
						
					case '+':
						*tp++ = *priv.ep++;
						*tp = NUL;
						return TokenPlus;
						
					case '-':
						*tp++ = *priv.ep++;
						*tp = NUL;
						return TokenMinus;
						
					case '\'':
						*tp++ = *priv.ep++;
						state = StateQuoteOdd;
						break;
						
					default:
						*tp++ = *priv.ep++;
						state = StateConst;
						break;
				}
				
				break;
				
			case StateLess:
				switch( *priv.ep )
				{
					case '<':
						*tp++ = *priv.ep++;
						state = StateLess2;
						break;
						
					default:
						*tp = NUL;
						THROW( ExSyntaxError );
				}
				
				break;
				
			case StateLess2:
				switch( *priv.ep )
				{
					case '<':
						*tp++ = *priv.ep++;
						*tp = NUL;
						return TokenRotateLeft;
						
					default:
						*tp = NUL;
						return TokenShiftLeft;
				}
				
				break;
				
			case StateGreater:
				switch( *priv.ep )
				{
					case '>':
						*tp++ = *priv.ep++;
						state = StateGreater2;
						break;
						
					default:
						*tp = NUL;
						THROW( ExSyntaxError );
				}
				
				break;
				
			case StateGreater2:
				switch( *priv.ep )
				{
					case '>':
						*tp++ = *priv.ep++;
						*tp = NUL;
						return TokenRotateRight;
						
					default:
						*tp = NUL;
						return TokenShiftRight;
				}
				
				break;
				
			case StateAsterisk:
				switch( *priv.ep )
				{
					case '*':
						*tp++ = *priv.ep++;
						*tp = NUL;
						return TokenPower;
						
					default:
						*tp = NUL;
						return TokenMultiply;
				}
				
				break;
				
			case StateQuoteOdd:
				switch( *priv.ep )
				{
					case NUL:
						*tp = NUL;
						return TokenConst;
						
					case '\'':
						*tp++ = *priv.ep++;
						state = StateQuoteEven;
						break;
						
					default:
						*tp++ = *priv.ep++;
						break;
				}
				
				break;
				
			case StateQuoteEven:
				switch( *priv.ep )
				{
					case NUL:
					case ' ':
					case OPCHAR:
						*tp = NUL;
						return TokenConst;
						
					case '\'':
						*tp++ = *priv.ep++;
						state = StateQuoteOdd;
						break;
						
					default:
						*tp++ = *priv.ep++;
						break;
				}
				
				break;
				
			case StateConst:
				switch( *priv.ep )
				{
					case NUL:
					case ' ':
					case OPCHAR:
						*tp = NUL;
						return TokenConst;
						
					default:
						*tp++ = *priv.ep++;
						break;
				}
				
				break;
		}
	}
}

static token_t get_token_may_throw_debug( void )
{
	token_t token = get_token_may_throw();
	
	if( flags.cl_opt.debug )
		fprintf( stderr, "token %02u [%s]\n", token, priv.token_buf );
	
	return token;
}

///////////////////////////////////////////////////////////////////////////////

static void level_9( uint16 *pn )
{
	if( priv.token == TokenLeftParen )
	{
		priv.token = get_token_may_throw_debug();
		level_1( pn );
		
		if( priv.token != TokenRightParen )
			THROW( ExSyntaxError );
	}
	else
	{
		if( priv.token != TokenConst && priv.token != TokenMultiply )
			THROW( ExSyntaxError );
		
		*pn = eval_const_may_throw( priv.token_buf );
	}
	
	priv.token = get_token_may_throw_debug();
}

static void level_8( uint16 *pn )
{
	int op;
	
	if( priv.token == TokenPlus || priv.token == TokenMinus || priv.token == TokenNot )
	{
		op = priv.token;
		priv.token = get_token_may_throw_debug();
	}
	else
		op = TokenEOL;
	
	level_9( pn );
	
	switch( op )
	{
		case TokenPlus:
			break;
			
		case TokenMinus:
			*pn = -*pn;
			break;
			
		case TokenNot:
			*pn = ~*pn;
			break;
	}
}

static void level_7( uint16 *pn )
{
	int op;
	uint16 n;
	
	level_8( pn );
	
	while( priv.token == TokenShiftLeft || priv.token == TokenShiftRight || priv.token == TokenRotateLeft || priv.token == TokenRotateRight )
	{
		op = priv.token;
		priv.token = get_token_may_throw_debug();
		level_8( &n );
		
		switch( op )
		{
			case TokenShiftLeft:
				if( n > 0 )
				{
					if( n < 16 )
						*pn <<= n;
					else
						*pn = 0;
				}
				
				break;
				
			case TokenShiftRight:
				if( n > 0 )
				{
					if( n < 16 )
						*pn >>= n;
					else
						*pn = 0;
				}
				
				break;
				
			case TokenRotateLeft:
				if( n > 0 )
					*pn = (*pn << (n & 15)) | (*pn >> (-n & 15));
				
				break;
				
			case TokenRotateRight:
				if( n > 0 )
					*pn = (* (uint16 *) pn >> (n & 15)) | (*pn << (-n & 15));
				
				break;
		}
	}
}

static void level_6( uint16 *pn )
{
	uint16 n;
	
	level_7( pn );
	
	while( priv.token == TokenAnd )
	{
		priv.token = get_token_may_throw_debug();
		level_7( &n );
		*pn &= n;
	}
}

static void level_5( uint16 *pn )
{
	uint16 n;
	
	level_6( pn );
	
	while( priv.token == TokenXor )
	{
		priv.token = get_token_may_throw_debug();
		level_6( &n );
		*pn ^= n;
	}
}

static void level_4( uint16 *pn )
{
	uint16 n;
	
	level_5( pn );
	
	while( priv.token == TokenOr )
	{
		priv.token = get_token_may_throw_debug();
		level_5( &n );
		*pn |= n;
	}
}

static void level_3( uint16 *pn )
{
	uint16 n;
	
	level_4( pn );
	
	while( priv.token == TokenPower )
	{
		priv.token = get_token_may_throw_debug();
		level_4( &n );
		*pn = power( *pn, n );
	}
}

static void level_2( uint16 *pn )
{
	int op;
	uint16 n;
	
	level_3( pn );
	
	while( priv.token == TokenMultiply || priv.token == TokenDivide || priv.token == TokenModulo )
	{
		op = priv.token;
		priv.token = get_token_may_throw_debug();
		level_3( &n );
		
		switch( op )
		{
			case TokenMultiply:
				*pn *= n;
				break;
				
			case TokenDivide:
				if( n == 0 )
					THROW( ExDivideByZero );
				
				*pn /= n;
				break;
				
			case TokenModulo:
				if( n == 0 )
					THROW( ExDivideByZero );
				
				*pn %= n;
				break;
		}
	}
}

static void level_1( uint16 *pn )
{
	int op;
	uint16 n;
	
	level_2( pn );
	
	while( priv.token == TokenPlus || priv.token == TokenMinus )
	{
		op = priv.token;
		priv.token = get_token_may_throw_debug();
		level_2( &n );
		
		switch( op )
		{
			case TokenPlus:
				*pn += n;
				break;
				
			case TokenMinus:
				*pn -= n;
				break;
		}
	}
}

uint16 eval_expr_may_throw( const char *s )
{
	uint16 n;
	
	priv.ep = s;
	priv.token = get_token_may_throw_debug();
	level_1( &n );
	
	if( priv.token != TokenEOL )
		THROW( ExSyntaxError );
	
	return n;
}

int eval_expr( const char *s, uint16 *pn )
{
	TRY
	{
		case ExTry:
			*pn = eval_expr_may_throw( s );
			return 1;
		
		case ExSyntaxError:
			error( msg_syntax_error, priv.token_buf );
			return 0;
		
		case ExDivideByZero:
			error( msg_divide_by_zero );
			return 0;
			
		case ExInvalidSymbolName:
			error( msg_invalid_symbol_name, priv.token_buf );
			return 0;
			
		case ExUndefinedSymbol:
			error( msg_undefined_symbol, priv.token_buf );
			return 0;
		
		default:
			return 0;
	}
	
	return 0;
}
