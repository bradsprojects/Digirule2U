#include "digirule2.h"
#include <ctype.h>

#pragma warning disable 1481	// disable nuisance warning

#define BEL		'\x07'
#define CR		'\x0D'
#define ESC		'\x1B'

#define DISASM_LINES	12

static void state_prompt( void );
static void state_prompt2( void );
static void state_prompt3( void );

static void state_display_memory( void );
static void state_display_memory2( void );
static void state_display_memory3( void );
static void state_display_memory4( void );
static void state_display_memory5( void );
static void state_display_memory6( void );
static void state_display_memory7( void );
static void state_display_memory8( void );
static void state_display_memory9( void );
static void state_display_memory10( void );
static void state_display_memory11( void );
static void state_display_memory12( void );
static void state_display_memory13( void );

static void state_disassemble_memory( void );
static void state_disassemble_memory2( void );
static void state_disassemble_memory3( void );
static void state_disassemble_memory4( void );
static void state_disassemble_memory5( void );
static void state_disassemble_memory6( void );
static void state_disassemble_memory7( void );
static void state_disassemble_memory8( void );
static void state_disassemble_memory9( void );
static void state_disassemble_memory10( void );
static void state_disassemble_memory11( void );
static void state_disassemble_memory12( void );
static void state_disassemble_memory13( void );

static void state_edit_memory( void );
static void state_edit_memory2( void );
static void state_edit_memory3( void );
static void state_edit_memory4( void );
static void state_edit_memory5( void );
static void state_edit_memory6( void );
static void state_edit_memory7( void );
static void state_edit_memory8( void );
static void state_edit_memory9( void );
static void state_edit_memory10( void );
static void state_edit_memory11( void );
static void state_edit_memory12( void );
static void state_edit_memory13( void );
static void state_edit_memory14( void );

static void state_fill_memory( void );
static void state_fill_memory2( void );
static void state_fill_memory3( void );
static void state_fill_memory4( void );
static void state_fill_memory5( void );
static void state_fill_memory6( void );
static void state_fill_memory7( void );
static void state_fill_memory8( void );
static void state_fill_memory9( void );
static void state_fill_memory10( void );

static void state_load_memory( void );
static void state_load_memory2( void );

static void state_save_memory( void );
static void state_save_memory2( void );

static void state_display_registers( void );
static void state_display_registers2( void );
static void state_display_registers3( void );
static void state_display_registers4( void );
static void state_display_registers5( void );
static void state_display_registers6( void );
static void state_display_registers7( void );
static void state_display_registers8( void );
static void state_display_registers9( void );
static void state_display_registers10( void );
static void state_display_registers11( void );
static void state_display_registers12( void );
static void state_display_registers13( void );
static void state_display_registers14( void );
static void state_display_registers15( void );
static void state_display_registers16( void );
static void state_display_registers17( void );
static void state_display_registers18( void );
static void state_display_registers19( void );
static void state_display_registers20( void );
static void state_display_registers21( void );
static void state_display_registers22( void );
static void state_display_registers23( void );
static void state_display_registers24( void );
static void state_display_registers25( void );
static void state_display_registers26( void );
static void state_display_registers27( void );

static void state_edit_accumulator( void );
static void state_edit_accumulator2( void );
static void state_edit_accumulator3( void );
static void state_edit_accumulator4( void );
static void state_edit_accumulator5( void );
static void state_edit_accumulator6( void );
static void state_edit_accumulator7( void );

static void state_edit_breakpoint( void );
static void state_edit_breakpoint2( void );
static void state_edit_breakpoint3( void );
static void state_edit_breakpoint4( void );
static void state_edit_breakpoint5( void );
static void state_edit_breakpoint6( void );
static void state_edit_breakpoint7( void );

static void state_edit_program_counter( void );
static void state_edit_program_counter2( void );
static void state_edit_program_counter3( void );
static void state_edit_program_counter4( void );
static void state_edit_program_counter5( void );
static void state_edit_program_counter6( void );
static void state_edit_program_counter7( void );

static void state_run( void );
static void state_run2( void );

static void state_quit( void );
static void state_done( void );

///////////////////////////////////////////////////////////////////////////////
// public data

bool_t monitor_active;

///////////////////////////////////////////////////////////////////////////////
// private data

static const char
	menu[] =										"\r\n"
		"Digirule 2U Debug Monitor | " BUILD_DATE	"\r\n"
													"\r\n"
		"  D - Display Memory"						"\r\n"
		"  I - Disassemble Memory"					"\r\n"
		"  E - Edit Memory"							"\r\n"
		"  F - Fill Memory"							"\r\n"
		"  L - Load Memory"							"\r\n"
		"  V - Save Memory"							"\r\n"
													"\r\n"
		"  R - Display Registers"					"\r\n"
		"  A - Edit Accumulator"					"\r\n"
		"  B - Edit Breakpoint"						"\r\n"
		"  P - Edit Program Counter"				"\r\n"
													"\r\n"
		"  G - Go (Run)"							"\r\n"
		"  S - Step"								"\r\n"
		"  Z - Reset CPU"							"\r\n"
													"\r\n"
		"  ? - Display Menu"						"\r\n"
		"  Q - Quit"								"\r\n"
													"\r\n",
	*p;

static uint8 lines;
static union8_t address, end_address, data, breakpoint;
static void (*state)( void );

static const struct
{
	char *inst;
	uint8 size;
} lut[256] =
{
	{ "HALT   ", 1 },
	{ "NOP    ", 1 },
	{ "SPEED  ", 2 },
	{ "INITSP ", 1 },
	{ "COPYLA ", 2 },
	{ "COPYLR ", 3 },
	{ "COPYLI ", 3 },
	{ "COPYAR ", 2 },
	{ "COPYAI ", 2 },
	{ "COPYRA ", 2 },
	{ "COPYRR ", 3 },
	{ "COPYRI ", 3 },
	{ "COPYIA ", 2 },
	{ "COPYIR ", 3 },
	{ "COPYII ", 3 },
	{ "SWAPRA ", 2 },
	{ "SWAPRR ", 3 },
	{ "ADDLA  ", 2 },
	{ "ADDRA  ", 2 },
	{ "SUBLA  ", 2 },
	{ "SUBRA  ", 2 },
	{ "MUL    ", 3 },
	{ "DIV    ", 3 },
	{ "ANDLA  ", 2 },
	{ "ANDRA  ", 2 },
	{ "ORLA   ", 2 },
	{ "ORRA   ", 2 },
	{ "XORLA  ", 2 },
	{ "XORRA  ", 2 },
	{ "DECR   ", 2 },
	{ "INCR   ", 2 },
	{ "DECRJZ ", 2 },
	{ "INCRJZ ", 2 },
	{ "SHIFTRL", 2 },
	{ "SHIFTRR", 2 },
	{ "BCLR   ", 3 },
	{ "BSET   ", 3 },
	{ "BCHG   ", 3 },
	{ "BTSTSC ", 3 },
	{ "BTSTSS ", 3 },
	{ "JUMP   ", 2 },
	{ "JUMPI  ", 2 },
	{ "CALL   ", 2 },
	{ "CALLI  ", 2 },
	{ "RETURN ", 1 },
	{ "RETLA  ", 2 },
	{ "ADDRPC ", 2 },
	{ "RANDA  ", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "COMOUT ", 1 },
	{ "COMIN  ", 1 },
	{ "COMRDY ", 1 },
	{ "ILLEGAL", 1 },
	{ "PINOUT ", 2 },
	{ "PININ  ", 2 },
	{ "PINDIR ", 2 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
	{ "ILLEGAL", 1 },
};

///////////////////////////////////////////////////////////////////////////////
// public functions

void monitor_init( void )
{
	breakpoint.byte = 0xFF;
	p = menu;
	state = state_prompt;
}

void monitor_process( void )
{
	state();
}

///////////////////////////////////////////////////////////////////////////////
// private functions

static void state_prompt( void )
{
	if( *p == '\0' )
	{
		p = "> ";
		state = state_prompt2;
	}
	else if( TRMT )
		TXREG = *p++;
}

static void state_prompt2( void )
{
	if( *p == '\0' )
	{
		comm_rx_flush();
		state = state_prompt3;
	}
	else if( TRMT )
		TXREG = *p++;
}

static void state_prompt3( void )
{
	if( comm_rx_ready() )
	{
		switch( comm_rx() )
		{
			case CR:
				p = "\r\n";
				state = state_prompt;
				break;

			case '?':
				p = menu;
				state = state_prompt;
				break;
				
			case 'A': case 'a':
				p = "Edit Accumulator | AC=";
				state = state_edit_accumulator;
				break;
				
			case 'B': case 'b':
				p = "Edit Breakpoint | BP=";
				state = state_edit_breakpoint;
				break;
				
			case 'D': case 'd':
				address.byte = 0;
				p = "Display Memory\r\n  ";
				state = state_display_memory;
				break;

			case 'E': case 'e':
				p = "Edit Memory | Address? ";
				state = state_edit_memory;
				break;

			case 'F': case 'f':
				p = "Fill Memory | Start address? ";
				state = state_fill_memory;
				break;

			case 'G': case 'g':
				latc.led.run = 1;
				p = "Go";
				state = state_run;
				break;
				
			case 'I': case 'i':
				address.byte = PC;
				lines = 0;
				p = "Disassemble Memory | Address (default=PC) ? ";
				state = state_disassemble_memory;
				break;

			case 'L': case 'l':
				p = "Load Memory | Location (0-7) ? ";
				state = state_load_memory;
				break;

			case 'P': case 'p':
				p = "Edit Program Counter | PC=";
				state = state_edit_program_counter;
				break;

			case 'Q': case 'q':
				p = "Quit\r\n";
				state = state_quit;
				break;

			case 'R': case 'r':
				p = "Display Registers\r\n";
				state = state_display_registers;
				break;

			case 'S': case 's':
				latc.led.run = 1;
				cpu_step();
				latc.led.run = 0;
				p = "Step\r\n";
				state = state_display_registers;
				break;

			case 'V': case 'v':
				p = "Save Memory | Location (0-7) ? ";
				state = state_save_memory;
				break;

			case 'Z': case 'z':
				cpu_reset();
				p = "Reset CPU\r\n";
				state = state_display_registers;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

///////////////////////////////////////////////////////////

static void state_display_memory( void )
{
	if( *p == '\0' )
		state = state_display_memory2;
	
	else if( TRMT )
		TXREG = *p++;
}

static void state_display_memory2( void )
{
	if( TRMT )
	{
		TXREG = hex[address.high_nibble];
		state = state_display_memory3;
	}
}

static void state_display_memory3( void )
{
	if( TRMT )
	{
		TXREG = hex[address.low_nibble];
		state = state_display_memory4;
	}
}

static void state_display_memory4( void )
{
	if( TRMT )
	{
		TXREG = ' ';
		state = state_display_memory5;
	}
}

static void state_display_memory5( void )
{
	if( TRMT )
	{
		TXREG = (address.byte == PC) ? '*' : ' ';
		state = state_display_memory6;
	}
}

static void state_display_memory6( void )
{
	if( TRMT )
	{
		TXREG = hex[ram.byte2[address.byte].high_nibble];
		state = state_display_memory7;
	}
}

static void state_display_memory7( void )
{
	if( TRMT )
	{
		TXREG = hex[ram.byte2[address.byte].low_nibble];
		state = state_display_memory8;
	}
}

static void state_display_memory8( void )
{
	if( TRMT )
	{
		if( (++address.byte & 15) == 0 )
		{
			address.byte -= 16;
			TXREG = ' ';
			state = state_display_memory9;
		}
		else
			state = state_display_memory5;
	}
}

static void state_display_memory9( void )
{
	if( TRMT )
	{
		TXREG = ' ';
		state = state_display_memory10;
	}
}

static void state_display_memory10( void )
{
	if( TRMT )
	{
		TXREG = '|';
		state = state_display_memory11;
	}
}

static void state_display_memory11( void )
{
	if( TRMT )
	{
		TXREG = IS_PRINT( RAM[address.byte] ) ? RAM[address.byte] : '.';

		if( (++address.byte & 15) == 0 )
			state = state_display_memory12;
	}
}

static void state_display_memory12( void )
{
	if( TRMT )
	{
		TXREG = '|';
		state = state_display_memory13;
	}
}

static void state_display_memory13( void )
{
	if( TRMT )
	{
		if( comm_rx_ready() )			// if cancel
		{
			p = "\r\n";
			state = state_prompt;
		}
		else if( address.byte == 0 )	// if end of memory
		{
			p = "\r\n";
			state = state_prompt;
		}
		else							// next line
		{
			p = "\r\n  ";
			state = state_display_memory;
		}
	}
}

///////////////////////////////////////////////////////////

static void state_disassemble_memory( void )
{
	if( *p == '\0' )
	{
		comm_rx_flush();
		state = state_disassemble_memory2;
	}
	else if( TRMT )
		TXREG = *p++;
}

static void state_disassemble_memory2( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( c )
		{
			case DIGIT:
				TXREG = c;
				address.byte = c - '0';
				state = state_disassemble_memory3;
				break;
				
			case XDIGIT:
				TXREG = c;
				address.byte = c - 'A' + 10;
				state = state_disassemble_memory3;
				break;

			case CR:
				state = state_disassemble_memory4;
				break;
				
			case ESC:
				p = "\r\n";
				state = state_prompt;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

static void state_disassemble_memory3( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( c )
		{
			case DIGIT:
				TXREG = c;
				address.high_nibble = address.low_nibble;
				address.low_nibble = c - '0';
				state = state_disassemble_memory4;
				break;
				
			case XDIGIT:
				TXREG = c;
				address.high_nibble = address.low_nibble;
				address.low_nibble = c - 'A' + 10;
				state = state_disassemble_memory4;
				break;
				
			case CR:
				state = state_disassemble_memory4;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

static void state_disassemble_memory4( void )
{
	end_address.byte = address.byte + lut[RAM[address.byte]].size;
	
	if( address.byte >= sizeof( ram.program ) || end_address.byte > sizeof( ram.program ) )
	{
		p = "\r\n";
		state = state_prompt;
	}
	else
	{
		p = "\r\n  ";
		state = state_disassemble_memory5;
	}
}

static void state_disassemble_memory5( void )
{
	if( *p == '\0' )
		state = state_disassemble_memory6;
	
	else if( TRMT )
		TXREG = *p++;
}

static void state_disassemble_memory6( void )
{
	if( TRMT )
	{
		TXREG = hex[address.high_nibble];
		state = state_disassemble_memory7;
	}
}

static void state_disassemble_memory7( void )
{
	if( TRMT )
	{
		TXREG = hex[address.low_nibble];
		state = state_disassemble_memory8;
	}
}

static void state_disassemble_memory8( void )
{
	if( TRMT )
	{
		TXREG = ' ';
		state = state_disassemble_memory9;
	}
}

static void state_disassemble_memory9( void )
{
	if( TRMT )
	{
		p = lut[RAM[address.byte]].inst;
		state = state_disassemble_memory10;
	}
}

static void state_disassemble_memory10( void )
{
	if( *p == '\0' )
		state = state_disassemble_memory11;
	
	else if( TRMT )
		TXREG = *p++;
}

static void state_disassemble_memory11( void )
{
	if( TRMT )
	{
		if( ++address.byte == end_address.byte )
		{
			end_address.byte = address.byte + lut[RAM[address.byte]].size;

			if( ++lines == DISASM_LINES || address.byte >= sizeof( ram.program ) || end_address.byte > sizeof( ram.program ) )
			{
				p = "\r\n";
				state = state_prompt;
			}
			else
			{
				p = "\r\n  ";
				state = state_disassemble_memory5;
			}
		}
		else
		{
			TXREG = ' ';
			state = state_disassemble_memory12;
		}
	}
}

static void state_disassemble_memory12( void )
{
	if( TRMT )
	{
		TXREG = hex[ram.byte2[address.byte].high_nibble];
		state = state_disassemble_memory13;
	}
}

static void state_disassemble_memory13( void )
{
	if( TRMT )
	{
		TXREG = hex[ram.byte2[address.byte].low_nibble];
		state = state_disassemble_memory11;
	}
}

///////////////////////////////////////////////////////////

static void state_edit_memory( void )
{
	if( *p == '\0' )
	{
		address.byte = 0;
		comm_rx_flush();
		state = state_edit_memory2;
	}
	else if( TRMT )
		TXREG = *p++;
}

static void state_edit_memory2( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( c )
		{
			case DIGIT:
				TXREG = c;
				address.byte = c - '0';
				state = state_edit_memory3;
				break;
				
			case XDIGIT:
				TXREG = c;
				address.byte = c - 'A' + 10;
				state = state_edit_memory3;
				break;

			case CR:
			case ESC:
				p = "\r\n";
				state = state_prompt;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

static void state_edit_memory3( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( c )
		{
			case DIGIT:
				TXREG = c;
				address.high_nibble = address.low_nibble;
				address.low_nibble = c - '0';
				p = "\r\n  ";
				state = state_edit_memory4;
				break;
				
			case XDIGIT:
				TXREG = c;
				address.high_nibble = address.low_nibble;
				address.low_nibble = c - 'A' + 10;
				p = "\r\n  ";
				state = state_edit_memory4;
				break;
				
			case CR:
				p = "\r\n  ";
				state = state_edit_memory4;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

static void state_edit_memory4( void )
{
	if( *p == '\0' )
		state = state_edit_memory5;

	else if( TRMT )
		TXREG = *p++;
}

static void state_edit_memory5( void )
{
	if( TRMT )
	{
		TXREG = hex[address.high_nibble];
		state = state_edit_memory6;
	}
}

static void state_edit_memory6( void )
{
	if( TRMT )
	{
		TXREG = hex[address.low_nibble];
		state = state_edit_memory7;
	}
}

static void state_edit_memory7( void )
{
	if( TRMT )
	{
		TXREG = '=';
		state = state_edit_memory8;
	}
}

static void state_edit_memory8( void )
{
	if( TRMT )
	{
		TXREG = hex[ram.byte2[address.byte].high_nibble];
		state = state_edit_memory9;
	}
}

static void state_edit_memory9( void )
{
	if( TRMT )
	{
		TXREG = hex[ram.byte2[address.byte].low_nibble];
		state = state_edit_memory10;
	}
}

static void state_edit_memory10( void )
{
	if( TRMT )
	{
		TXREG = ' ';
		state = state_edit_memory11;
	}
}

static void state_edit_memory11( void )
{
	if( TRMT )
	{
		TXREG = '?';
		state = state_edit_memory12;
	}
}

static void state_edit_memory12( void )
{
	if( TRMT )
	{
		TXREG = ' ';
		comm_rx_flush();
		state = state_edit_memory13;
	}
}

static void state_edit_memory13( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( toupper( c ) )
		{
			case DIGIT:
				TXREG = c;
				ram.byte[address.byte] = c - '0';
				state = state_edit_memory14;
				break;
				
			case XDIGIT:
				TXREG = c;
				ram.byte[address.byte] = c - 'A' + 10;
				state = state_edit_memory14;
				break;
			
			case ' ':
				address.byte++;
				p = "\r\n  ";
				state = state_edit_memory4;
				break;

			case CR:
			case ESC:
				p = "\r\n";
				state = state_prompt;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

static void state_edit_memory14( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( toupper( c ) )
		{
			case DIGIT:
				TXREG = c;
				ram.byte2[address.byte].high_nibble = ram.byte2[address.byte].low_nibble;
				ram.byte2[address.byte].low_nibble = c - '0';
				address.byte++;
				p = "\r\n  ";
				state = state_edit_memory4;
				break;
				
			case XDIGIT:
				TXREG = c;
				ram.byte2[address.byte].high_nibble = ram.byte2[address.byte].low_nibble;
				ram.byte2[address.byte].low_nibble = c - 'A' + 10;
				address.byte++;
				p = "\r\n  ";
				state = state_edit_memory4;
				break;

			case CR:
				address.byte++;
				p = "\r\n  ";
				state = state_edit_memory4;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

///////////////////////////////////////////////////////////

static void state_fill_memory( void )
{
	if( *p == '\0' )
	{
		address.byte = 0;
		comm_rx_flush();
		state = state_fill_memory2;
	}
	else if( TRMT )
		TXREG = *p++;
}

static void state_fill_memory2( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( c )
		{
			case DIGIT:
				TXREG = c;
				address.byte = c - '0';
				state = state_fill_memory3;
				break;
				
			case XDIGIT:
				TXREG = c;
				address.byte = c - 'A' + 10;
				state = state_fill_memory3;
				break;

			case CR:
			case ESC:
				p = "\r\n";
				state = state_prompt;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

static void state_fill_memory3( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( c )
		{
			case DIGIT:
				TXREG = c;
				address.high_nibble = address.low_nibble;
				address.low_nibble = c - '0';
				p = " | End address? ";
				state = state_fill_memory4;
				break;
				
			case XDIGIT:
				TXREG = c;
				address.high_nibble = address.low_nibble;
				address.low_nibble = c - 'A' + 10;
				p = " | End address? ";
				state = state_fill_memory4;
				break;
				
			case CR:
				p = " | End address? ";
				state = state_fill_memory4;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

static void state_fill_memory4( void )
{
	if( *p == '\0' )
	{
		end_address.byte = 0;
		comm_rx_flush();
		state = state_fill_memory5;
	}
	else if( TRMT )
		TXREG = *p++;
}

static void state_fill_memory5( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( c )
		{
			case DIGIT:
				TXREG = c;
				end_address.byte = c - '0';
				state = state_fill_memory6;
				break;
				
			case XDIGIT:
				TXREG = c;
				end_address.byte = c - 'A' + 10;
				state = state_fill_memory6;
				break;

			case CR:
			case ESC:
				p = "\r\n";
				state = state_prompt;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

static void state_fill_memory6( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( c )
		{
			case DIGIT:
				TXREG = c;
				end_address.high_nibble = end_address.low_nibble;
				end_address.low_nibble = c - '0';
				p = " | Data? ";
				state = state_fill_memory7;
				break;
				
			case XDIGIT:
				TXREG = c;
				end_address.high_nibble = end_address.low_nibble;
				end_address.low_nibble = c - 'A' + 10;
				p = " | Data? ";
				state = state_fill_memory7;
				break;
				
			case CR:
				p = " | Data? ";
				state = state_fill_memory7;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

static void state_fill_memory7( void )
{
	if( *p == '\0' )
	{
		data.byte = 0;
		comm_rx_flush();
		state = state_fill_memory8;
	}
	else if( TRMT )
		TXREG = *p++;
}

static void state_fill_memory8( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( c )
		{
			case DIGIT:
				TXREG = c;
				data.byte = c - '0';
				state = state_fill_memory9;
				break;
				
			case XDIGIT:
				TXREG = c;
				data.byte = c - 'A' + 10;
				state = state_fill_memory9;
				break;

			case CR:
			case ESC:
				p = "\r\n";
				state = state_prompt;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

static void state_fill_memory9( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( c )
		{
			case DIGIT:
				TXREG = c;
				data.high_nibble = data.low_nibble;
				data.low_nibble = c - '0';
				p = "\r\n";
				state = state_fill_memory10;
				break;
				
			case XDIGIT:
				TXREG = c;
				data.high_nibble = data.low_nibble;
				data.low_nibble = c - 'A' + 10;
				p = "\r\n";
				state = state_fill_memory10;
				break;
				
			case CR:
				p = "\r\n";
				state = state_fill_memory10;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

static void state_fill_memory10( void )
{
	uint16 i;
	
	for( i = address.byte; i <= end_address.byte; i++ )
		ram.byte[i] = data.byte;
	
	state = state_prompt;
}

///////////////////////////////////////////////////////////

static void state_load_memory( void )
{
	if( *p == '\0' )
	{
		comm_rx_flush();
		state = state_load_memory2;
	}
	else if( TRMT )
		TXREG = *p++;
}

static void state_load_memory2( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();

		switch( c )
		{
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
				TXREG = c;
				file_load( c - '0', 1 );
				cpu_reset();
				breakpoint.byte = 0xFF;
				p = "\r\n";
				state = state_prompt;
				break;

			case CR:
			case ESC:
				p = "\r\n";
				state = state_prompt;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

///////////////////////////////////////////////////////////

static void state_save_memory( void )
{
	if( *p == '\0' )
	{
		comm_rx_flush();
		state = state_save_memory2;
	}
	else if( TRMT )
		TXREG = *p++;
}

static void state_save_memory2( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();

		switch( c )
		{
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':
				TXREG = c;
				file_save( c - '0', 1 );
				p = "\r\n";
				state = state_prompt;
				break;

			case CR:
			case ESC:
				p = "\r\n";
				state = state_prompt;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

///////////////////////////////////////////////////////////

static void state_display_registers( void )
{
	if( *p == '\0' )
	{
		p = "PC=";
		state = state_display_registers2;
	}
	else if( TRMT )
		TXREG = *p++;
}

static void state_display_registers2( void )
{
	if( *p == '\0' )
		state = state_display_registers3;

	else if( TRMT )
		TXREG = *p++;
}

static void state_display_registers3( void )
{
	if( TRMT )
	{
		TXREG = hex[cpu.pc.high_nibble];
		state = state_display_registers4;
	}
}

static void state_display_registers4( void )
{
	if( TRMT )
	{
		TXREG = hex[cpu.pc.low_nibble];
		state = state_display_registers5;
	}
}

static void state_display_registers5( void )
{
	if( TRMT )
	{
		p = " | SR=";
		TXREG = *p++;
		state = state_display_registers6;
	}
}

static void state_display_registers6( void )
{
	if( *p == '\0' )
		state = state_display_registers7;

	else if( TRMT )
		TXREG = *p++;
}

static void state_display_registers7( void )
{
	if( TRMT )
	{
		TXREG = hex[ram.regs.status.high_nibble];
		state = state_display_registers8;
	}
}

static void state_display_registers8( void )
{
	if( TRMT )
	{
		TXREG = hex[ram.regs.status.low_nibble];
		state = state_display_registers9;
	}
}

static void state_display_registers9( void )
{
	if( TRMT )
	{
		TXREG = ' ';
		state = state_display_registers10;
	}
}

static void state_display_registers10( void )
{
	if( TRMT )
	{
		TXREG = ram.regs.status.show_address_led_reg ? 'A' : '.';
		state = state_display_registers11;
	}
}

static void state_display_registers11( void )
{
	if( TRMT )
	{
		TXREG = ram.regs.status.carry ? 'C' : '.';
		state = state_display_registers12;
	}
}

static void state_display_registers12( void )
{
	if( TRMT )
	{
		TXREG = ram.regs.status.zero ? 'Z' : '.';
		state = state_display_registers13;
	}
}

static void state_display_registers13( void )
{
	if( TRMT )
	{
		p = " | AC=";
		TXREG = *p++;
		state = state_display_registers14;
	}
}

static void state_display_registers14( void )
{
	if( *p == '\0' )
		state = state_display_registers15;

	else if( TRMT )
		TXREG = *p++;
}

static void state_display_registers15( void )
{
	if( TRMT )
	{
		TXREG = hex[cpu.accum.low_high_nibble];
		state = state_display_registers16;
	}
}

static void state_display_registers16( void )
{
	if( TRMT )
	{
		TXREG = hex[cpu.accum.low_low_nibble];
		state = state_display_registers17;
	}
}

static void state_display_registers17( void )
{
	if( TRMT )
	{
		p = " | SP=";
		TXREG = *p++;
		state = state_display_registers18;
	}
}

static void state_display_registers18( void )
{
	if( *p == '\0' )
		state = state_display_registers19;

	else if( TRMT )
		TXREG = *p++;
}

static void state_display_registers19( void )
{
	if( TRMT )
	{
		TXREG = hex[cpu.sp.high_nibble];
		state = state_display_registers20;
	}
}

static void state_display_registers20( void )
{
	if( TRMT )
	{
		TXREG = hex[cpu.sp.low_nibble];
		state = state_display_registers21;
	}
}

static void state_display_registers21( void )
{
	if( TRMT )
	{
		address.byte = PC;
		end_address.byte = address.byte + lut[RAM[address.byte]].size;
		
		if( address.byte >= sizeof( ram.program ) || end_address.byte > sizeof( ram.program ) )
		{
			p = "\r\n";
			state = state_prompt;
		}
		else
		{
			p = " | @PC=";
			TXREG = *p++;
			state = state_display_registers22;
		}
	}
}

static void state_display_registers22( void )
{
	if( *p == '\0' )
	{
		p = lut[RAM[PC]].inst;
		TXREG = *p++;
		state = state_display_registers23;
	}
	else if( TRMT )
		TXREG = *p++;
}

static void state_display_registers23( void )
{
	if( *p == '\0' )
		state = state_display_registers24;
	
	else if( TRMT )
		TXREG = *p++;
}

static void state_display_registers24( void )
{
	if( TRMT )
	{
		if( ++address.byte == end_address.byte )
		{
			p = "\r\n";
			state = state_prompt;
		}
		else
			state = state_display_registers25;
	}
}

static void state_display_registers25( void )
{
	if( TRMT )
	{
		TXREG = ' ';
		state = state_display_registers26;
	}
}

static void state_display_registers26( void )
{
	if( TRMT )
	{
		TXREG = hex[ram.byte2[address.byte].high_nibble];
		state = state_display_registers27;
	}
}

static void state_display_registers27( void )
{
	if( TRMT )
	{
		TXREG = hex[ram.byte2[address.byte].low_nibble];
		state = state_display_registers24;
	}
}

///////////////////////////////////////////////////////////

static void state_edit_accumulator( void )
{
	if( *p == '\0' )
		state = state_edit_accumulator2;
	
	else if( TRMT )
		TXREG = *p++;
}

static void state_edit_accumulator2( void )
{
	if( TRMT )
	{
		TXREG = hex[cpu.accum.low_high_nibble];
		state = state_edit_accumulator3;
	}
}

static void state_edit_accumulator3( void )
{
	if( TRMT )
	{
		TXREG = hex[cpu.accum.low_low_nibble];
		state = state_edit_accumulator4;
	}
}

static void state_edit_accumulator4( void )
{
	if( TRMT )
	{
		p = " | Data? ";
		TXREG = *p++;
		state = state_edit_accumulator5;
	}
}

static void state_edit_accumulator5( void )
{
	if( *p == '\0' )
	{
		comm_rx_flush();
		state = state_edit_accumulator6;
	}
	else if( TRMT )
		TXREG = *p++;
}

static void state_edit_accumulator6( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( c )
		{
			case DIGIT:
				TXREG = c;
				cpu.accum.low_byte = c - '0';
				state = state_edit_accumulator7;
				break;
				
			case XDIGIT:
				TXREG = c;
				cpu.accum.low_byte = c - 'A' + 10;
				state = state_edit_accumulator7;
				break;

			case CR:
			case ESC:
				p = "\r\n";
				state = state_prompt;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

static void state_edit_accumulator7( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( c )
		{
			case DIGIT:
				TXREG = c;
				cpu.accum.low_high_nibble = cpu.accum.low_low_nibble;
				cpu.accum.low_low_nibble = c - '0';
				p = "\r\n";
				state = state_prompt;
				break;
				
			case XDIGIT:
				TXREG = c;
				cpu.accum.low_high_nibble = cpu.accum.low_low_nibble;
				cpu.accum.low_low_nibble = c - 'A' + 10;
				p = "\r\n";
				state = state_prompt;
				break;
				
			case CR:
				p = "\r\n";
				state = state_prompt;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

///////////////////////////////////////////////////////////

static void state_edit_breakpoint( void )
{
	if( *p == '\0' )
		state = state_edit_breakpoint2;
	
	else if( TRMT )
		TXREG = *p++;
}

static void state_edit_breakpoint2( void )
{
	if( TRMT )
	{
		TXREG = hex[breakpoint.high_nibble];
		state = state_edit_breakpoint3;
	}
}

static void state_edit_breakpoint3( void )
{
	if( TRMT )
	{
		TXREG = hex[breakpoint.low_nibble];
		state = state_edit_breakpoint4;
	}
}

static void state_edit_breakpoint4( void )
{
	if( TRMT )
	{
		p = " | Address (FF=disable) ? ";
		TXREG = *p++;
		state = state_edit_breakpoint5;
	}
}

static void state_edit_breakpoint5( void )
{
	if( *p == '\0' )
	{
		comm_rx_flush();
		state = state_edit_breakpoint6;
	}
	else if( TRMT )
		TXREG = *p++;
}

static void state_edit_breakpoint6( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( c )
		{
			case DIGIT:
				TXREG = c;
				breakpoint.byte = c - '0';
				state = state_edit_breakpoint7;
				break;
				
			case XDIGIT:
				TXREG = c;
				breakpoint.byte = c - 'A' + 10;
				state = state_edit_breakpoint7;
				break;

			case CR:
			case ESC:
				p = "\r\n";
				state = state_prompt;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

static void state_edit_breakpoint7( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( c )
		{
			case DIGIT:
				TXREG = c;
				breakpoint.high_nibble = breakpoint.low_nibble;
				breakpoint.low_nibble = c - '0';
				p = "\r\n";
				state = state_prompt;
				break;
				
			case XDIGIT:
				TXREG = c;
				breakpoint.high_nibble = breakpoint.low_nibble;
				breakpoint.low_nibble = c - 'A' + 10;
				p = "\r\n";
				state = state_prompt;
				break;
				
			case CR:
				p = "\r\n";
				state = state_prompt;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

///////////////////////////////////////////////////////////

static void state_edit_program_counter( void )
{
	if( *p == '\0' )
		state = state_edit_program_counter2;
	
	else if( TRMT )
		TXREG = *p++;
}

static void state_edit_program_counter2( void )
{
	if( TRMT )
	{
		TXREG = hex[cpu.pc.high_nibble];
		state = state_edit_program_counter3;
	}
}

static void state_edit_program_counter3( void )
{
	if( TRMT )
	{
		TXREG = hex[cpu.pc.low_nibble];
		state = state_edit_program_counter4;
	}
}

static void state_edit_program_counter4( void )
{
	if( TRMT )
	{
		p = " | Address? ";
		TXREG = *p++;
		state = state_edit_program_counter5;
	}
}

static void state_edit_program_counter5( void )
{
	if( *p == '\0' )
	{
		comm_rx_flush();
		state = state_edit_program_counter6;
	}
	else if( TRMT )
		TXREG = *p++;
}

static void state_edit_program_counter6( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( c )
		{
			case DIGIT:
				TXREG = c;
				PC = c - '0';
				state = state_edit_program_counter7;
				break;
				
			case XDIGIT:
				TXREG = c;
				PC = c - 'A' + 10;
				state = state_edit_program_counter7;
				break;

			case CR:
			case ESC:
				p = "\r\n";
				state = state_prompt;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

static void state_edit_program_counter7( void )
{
	char c;
	
	if( comm_rx_ready() )
	{
		c = comm_rx();
		c = (char) toupper( c );
		
		switch( c )
		{
			case DIGIT:
				TXREG = c;
				cpu.pc.high_nibble = cpu.pc.low_nibble;
				cpu.pc.low_nibble = c - '0';
				p = "\r\n";
				state = state_prompt;
				break;
				
			case XDIGIT:
				TXREG = c;
				cpu.pc.high_nibble = cpu.pc.low_nibble;
				cpu.pc.low_nibble = c - 'A' + 10;
				p = "\r\n";
				state = state_prompt;
				break;
				
			case CR:
				p = "\r\n";
				state = state_prompt;
				break;
				
			default:
				TXREG = BEL;
				break;
		}
	}
}

///////////////////////////////////////////////////////////

static void state_run( void )
{
	if( *p == '\0' )
	{
		cpu_step();				// step over possible breakpoint location
		state = state_run2;
	}
	else if( TRMT )
		TXREG = *p++;
}

static void state_run2( void )
{
	if( comm_rx_ready() || !latc.led.run )
	{
		latc.led.run = 0;
		p = " Stop\r\n";
		state = state_display_registers;
	}
	else
	{
		cpu_process();
		
		if( PC == breakpoint.byte )
		{
			latc.led.run = 0;
			p = " Break\r\n";
			state = state_display_registers;
		}
	}
}

///////////////////////////////////////////////////////////

static void state_quit( void )
{
	if( *p == '\0' )
	{
		monitor_active = 0;
		state = state_done;
	}			
	else if( TRMT )
		TXREG = *p++;
}

static void state_done( void )
{
}
