/*
 *	The Digirule 2U is dedicated to our good friend and colleague Olivier Lecluse -
 *	creator of DigiQt and many DR2U demo programs. Thank you for everything Olivier.
 */

#ifndef DIGIRULE2_H
#define	DIGIRULE2_H

#include <xc.h>

///////////////////////////////////////////////////////////////////////////////
// build options

#define BUILD_DATE		"22 Sep 2020"
#define STACK_DEPTH		64
#define RX_FIFO_SIZE	256

///////////////////////////////////////////////////////////////////////////////
// general macros

#define SP		cpu.sp.byte
#define PC		cpu.pc.byte
#define ACCUM	cpu.accum.low_byte

#define RAM		ram.byte
#define SR		ram.regs.status.byte
#define ZERO	ram.regs.status.zero
#define CARRY	ram.regs.status.carry

#define ELEMENTSOF( a )			(sizeof( a ) / sizeof( (a)[0] ))
#define MIN( a, b )				((a) < (b) ? (a) : (b))
#define IS_PRINT( c )			((c) >= ' ' && (c) <= '~')
#define IS_PGM_ADDR( addr )		((addr) < ELEMENTSOF( ram.program ))
#define IS_REG_ADDR( addr )		((addr) >= ELEMENTSOF( ram.program ))
#define PROGRESS_BAR( addr )	((uint8) (0xFF80 >> ((addr) >> 5)))

#define LISTENING_LED	data_leds.d7
#define TALKING_LED		data_leds.d6

#define DIGIT	'0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9'
#define XDIGIT	'A': case 'B': case 'C': case 'D': case 'E': case 'F'

///////////////////////////////////////////////////////////////////////////////
// type declarations

typedef enum { false, true } bool_t;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;

typedef union
{
	uint8 byte;
	struct
	{
		uint8 low_nibble  : 4;
		uint8 high_nibble : 4;
	};
	struct
	{
		uint8 bit0 : 1;
		uint8 bit1 : 1;
		uint8 bit2 : 1;
		uint8 bit3 : 1;
		uint8 bit4 : 1;
		uint8 bit5 : 1;
		uint8 bit6 : 1;
		uint8 bit7 : 1;
	};
} union8_t;

typedef union
{
	uint16 word;
	struct
	{
		uint8 low_byte, high_byte;
	};
	struct
	{
		uint8 low_low_nibble   : 4;
		uint8 low_high_nibble  : 4;
		uint8 high_low_nibble  : 4;
		uint8 high_high_nibble : 4;
	};
	struct
	{
		uint8 bit0  : 1;
		uint8 bit1  : 1;
		uint8 bit2  : 1;
		uint8 bit3  : 1;
		uint8 bit4  : 1;
		uint8 bit5  : 1;
		uint8 bit6  : 1;
		uint8 bit7  : 1;
		uint8 bit8  : 1;
		uint8 bit9  : 1;
		uint8 bit10 : 1;
		uint8 bit11 : 1;
		uint8 bit12 : 1;
		uint8 bit13 : 1;
		uint8 bit14 : 1;
		uint8 bit15 : 1;
	};
} union16_t;

typedef struct
{
	bool_t	  address_blanking;
	union8_t  sp, pc;
	union16_t accum;
} cpu_t;

typedef union
{
	uint8 byte[256];
	struct
	{
		uint8 low_nibble  : 4;
		uint8 high_nibble : 4;
	} byte2[256];
	struct
	{
		uint8 program[252];
		struct
		{
			union
			{
				uint8 byte;
				struct
				{
					uint8 low_nibble  : 4;
					uint8 high_nibble : 4;
				};
				struct
				{
					uint8 zero					: 1;
					uint8 carry					: 1;
					uint8 show_address_led_reg	: 1;
					uint8						: 5;
				};
			} status;
			uint8 button;
			uint8 address_led;
			uint8 data_led;
		} regs;
	};
} ram_t;

///////////////////////////////////////////////////////////////////////////////
// public function declarations

void comm_load_init( void );
void comm_load_process( void );
char comm_rx( void );
void comm_rx_flush( void );
bool_t comm_rx_ready( void );
void comm_save_init( void );
void comm_save_process( void );
void cpu_process( void );
void cpu_reset( void );
void cpu_step( void );
void file_load( uint8 slot, bool_t quiet );
void file_save( uint8 slot, bool_t quiet );
void lamp_test( void );
void monitor_init( void );
void monitor_process( void );
void sleep_ms( uint8 ms );
void ui_process( void );

///////////////////////////////////////////////////////////////////////////////
// public variable declarations

extern volatile union
{
	uint8 byte;
	struct
	{
		uint8 d0       : 1;
		uint8 d1       : 1;
		uint8 d2       : 1;
		uint8 d3       : 1;
		uint8 d4       : 1;
		uint8 d5       : 1;
		uint8 d6       : 1;
		uint8 d7       : 1;
	};
} data_leds __at( 0xF89 );	// LATA

extern volatile union
{
	union
	{
		uint8 data;
		struct
		{
			uint8 d0   : 1;
			uint8 d1   : 1;
			uint8 d2   : 1;
			uint8 d3   : 1;
			uint8 d4   : 1;
			uint8 d5   : 1;
			uint8 d6   : 1;
			uint8 d7   : 1;
		};
	} button;
} portb __at( 0xF81 );

extern volatile union
{
	struct
	{
		uint8 run_stop : 1;
		uint8          : 1;
		uint8 save_    : 1;
		uint8          : 2;
		uint8 load     : 1;
		uint8          : 2;
	} button;
	struct
	{
		uint8          : 3;
		uint8  A       : 1;
		uint8  B       : 1;
		uint8          : 3;
	} pin;
	struct
	{
		uint8          : 3;
		uint8 AB       : 2;
		uint8          : 3;
	} pins;
} portc __at( 0xF82 );

extern volatile union
{
	struct
	{
		uint8          : 1;
		uint8 run      : 1;
		uint8          : 6;
	} led;
	struct
	{
		uint8          : 3;
		uint8  A       : 1;
		uint8  B       : 1;
		uint8          : 3;
	} pin;
	struct
	{
		uint8          : 3;
		uint8 AB       : 2;
		uint8          : 3;
	} pins;
} latc __at( 0xF8B );

extern volatile union
{
	struct
	{
		uint8          : 3;
		uint8  A       : 1;
		uint8  B       : 1;
		uint8          : 3;
	} pin;
	struct
	{
		uint8          : 3;
		uint8 AB       : 2;
		uint8          : 3;
	} pins;
} trisc __at( 0xF94 );

extern volatile uint8 address_leds __at( 0xF8C );	// LATD

extern volatile union
{
	struct
	{
		uint8 goto_    : 1;
		uint8 store    : 1;
		uint8 prev     : 1;
		uint8 next     : 1;
		uint8          : 4;
	} button;
} porte __at( 0xF84 );

extern bool_t t1ms, monitor_active;
extern const char hex[];
extern const uint16 speed_to_rate[];
extern uint16 end_of_file_record_address;
extern cpu_t cpu;
extern ram_t ram;
extern void (*process_fn)( void );

#endif	// DIGIRULE2_H

// Required linker options
// -Wl,-AINIT=0800h-0803h -Wl,-Pinit=INIT
// -Wl,-AISR=0808h-08FFh -Wl,-Pisr_text=ISR
// -Wl,-AFILES=F800h-FFFFh -Wl,-Pfiles=FILES
