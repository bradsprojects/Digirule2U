#include "digirule2.h"

#define	HALT	latc.led.run = 0

///////////////////////////////////////////////////////////////////////////////
// public data

cpu_t cpu;
ram_t ram;

///////////////////////////////////////////////////////////////////////////////
// private data

static uint8 arg1, arg2, stack[STACK_DEPTH];
static uint16 inst_timer, inst_rate;
static union16_t temp;

///////////////////////////////////////////////////////////////////////////////
// private functions

static void inst_halt( void )			// stop the CPU (1 byte)
{
	HALT;
}

static void inst_nop( void )			// no operation (1 byte)
{
}

static void inst_speed( void )			// set instruction execution rate (2 bytes)
{
	inst_rate = speed_to_rate[RAM[PC++]];
}

static void inst_initsp( void )			// initialize stack pointer (1 byte)
{
	SP = 0;
}

static void inst_copyla( void )			// copy literal to accumulator (2 bytes)
{
	ACCUM = RAM[PC++];
	ZERO = ACCUM == 0;
}

static void inst_copylr( void )			// copy literal to RAM (3 bytes)
{
	arg1 = RAM[PC++];
	arg2 = RAM[PC++];
	RAM[arg2] = arg1;
	ZERO = RAM[arg2] == 0;
}

static void inst_copyli( void )			// copy literal to RAM indirect (3 bytes)
{
	arg1 = RAM[PC++];
	arg2 = RAM[PC++];
	RAM[RAM[arg2]] = arg1;
	ZERO = RAM[RAM[arg2]] == 0;
}

static void inst_copyar( void )			// copy accumulator to RAM (2 bytes)
{
	arg1 = RAM[PC++];
	RAM[arg1] = ACCUM;
	ZERO = RAM[arg1] == 0;
}

static void inst_copyai( void )			// copy accumulator to RAM indirect (2 bytes)
{
	arg1 = RAM[PC++];
	RAM[RAM[arg1]] = ACCUM;
	ZERO = RAM[RAM[arg1]] == 0;
}

static void inst_copyra( void )			// copy RAM to accumulator (2 bytes)
{
	arg1 = RAM[PC++];
	ACCUM = RAM[arg1];
	ZERO = ACCUM == 0;
}

static void inst_copyrr( void )			// copy RAM to RAM (3 bytes)
{
	arg1 = RAM[PC++];
	arg2 = RAM[PC++];
	RAM[arg2] = RAM[arg1];
	ZERO = RAM[arg2] == 0;
}

static void inst_copyri( void )			// copy RAM to RAM indirect (3 bytes)
{
	arg1 = RAM[PC++];
	arg2 = RAM[PC++];
	RAM[RAM[arg2]] = RAM[arg1];
	ZERO = RAM[RAM[arg2]] == 0;
}

static void inst_copyia( void )			// copy RAM indirect to accumulator (2 bytes)
{
	arg1 = RAM[PC++];
	ACCUM = RAM[RAM[arg1]];
	ZERO = ACCUM == 0;
}

static void inst_copyir( void )			// copy RAM indirect to RAM (3 bytes)
{
	arg1 = RAM[PC++];
	arg2 = RAM[PC++];
	RAM[arg2] = RAM[RAM[arg1]];
	ZERO = RAM[arg2] == 0;
}

static void inst_copyii( void )			// copy RAM indirect to RAM indirect (3 bytes)
{
	arg1 = RAM[PC++];
	arg2 = RAM[PC++];
	RAM[RAM[arg2]] = RAM[RAM[arg1]];
	ZERO = RAM[RAM[arg2]] == 0;
}

static void inst_swapra( void )			// swap RAM and accumulator (2 bytes)
{
	arg1 = RAM[PC++];
	temp.low_byte = RAM[arg1];
	RAM[arg1] = ACCUM;
	ACCUM = temp.low_byte;
	// status unaffected
}

static void inst_swaprr( void )			// swap RAM and RAM (3 bytes)
{
	arg1 = RAM[PC++];
	arg2 = RAM[PC++];
	temp.low_byte = RAM[arg1];
	RAM[arg1] = RAM[arg2];
	RAM[arg2] = temp.low_byte;
	// status unaffected
}

static void inst_addla( void )			// add literal and CARRY to accumulator, result in accumulator (2 bytes)
{
	arg1 = RAM[PC++];
	cpu.accum.high_byte = 0;
	cpu.accum.word += arg1;
	if( CARRY ) cpu.accum.word++;
	CARRY = cpu.accum.bit8;
	ZERO = ACCUM == 0;
}

static void inst_addra( void )			// add RAM and CARRY to accumulator, result in accumulator (2 bytes)
{
	arg1 = RAM[PC++];
	cpu.accum.high_byte = 0;
	cpu.accum.word += RAM[arg1];
	if( CARRY ) cpu.accum.word++;
	CARRY = cpu.accum.bit8;
	ZERO = ACCUM == 0;
}

static void inst_subla( void )			// subtract literal and CARRY from accumulator, result in accumulator (2 bytes)
{
	arg1 = RAM[PC++];
	cpu.accum.high_byte = 0;
	cpu.accum.word -= arg1;
	if( CARRY ) cpu.accum.word--;
	CARRY = cpu.accum.bit8;
	ZERO = ACCUM == 0;
}

static void inst_subra( void )			// subtract RAM and CARRY from accumulator, result in accumulator (2 bytes)
{
	arg1 = RAM[PC++];
	cpu.accum.high_byte = 0;
	cpu.accum.word -= RAM[arg1];
	if( CARRY ) cpu.accum.word--;
	CARRY = cpu.accum.bit8;
	ZERO = ACCUM == 0;
}

static void inst_mul( void )			// unsigned 8-bit multiply (3 bytes); on entry arg1 = multiplicand and arg2 = multiplier; on exit arg1 = product
{
	arg1 = RAM[PC++];
	arg2 = RAM[PC++];
	
	temp.word = RAM[arg1];
	temp.word *= RAM[arg2];
	RAM[arg1] = temp.low_byte;
	
	CARRY = temp.high_byte != 0;
	ZERO = RAM[arg1] == 0;
}

static void inst_div( void )			// unsigned 8-bit divide (3 bytes); on entry arg1 = dividend and arg2 = divisor; on exit arg1 = quotient and accumulator = remainder
{
	arg1 = RAM[PC++];
	arg2 = RAM[PC++];
	
	if( RAM[arg2] == 0 )
		HALT;							// divide by zero
	
	else
	{
		udiv_t d = udiv( RAM[arg1], RAM[arg2] );
		
		RAM[arg1] = (uint8) d.quot;
		ACCUM = (uint8) d.rem;
		
		CARRY = ACCUM == 0;				// non-standard usage of C flag as a Z flag for remainder
		ZERO = RAM[arg1] == 0;
	}
}

static void inst_andla( void )			// AND literal with accumulator, result in accumulator (2 bytes)
{
	ACCUM &= RAM[PC++];
	ZERO = ACCUM == 0;
}

static void inst_andra( void )			// AND RAM with accumulator, result in accumulator (2 bytes)
{
	arg1 = RAM[PC++];
	ACCUM &= RAM[arg1];
	ZERO = ACCUM == 0;
}

static void inst_orla( void )			// OR literal with accumulator, result in accumulator (2 bytes)
{
	ACCUM |= RAM[PC++];
	ZERO = ACCUM == 0;
}

static void inst_orra( void )			// OR RAM with accumulator, result in accumulator (2 bytes)
{
	arg1 = RAM[PC++];
	ACCUM |= RAM[arg1];
	ZERO = ACCUM == 0;
}

static void inst_xorla( void )			// XOR literal with accumulator, result in accumulator (2 bytes)
{
	ACCUM ^= RAM[PC++];
	ZERO = ACCUM == 0;
}

static void inst_xorra( void )			// XOR RAM with accumulator, result in accumulator (2 bytes)
{
	arg1 = RAM[PC++];
	ACCUM ^= RAM[arg1];
	ZERO = ACCUM == 0;
}

static void inst_decr( void )			// decrement RAM (2 bytes)
{
	arg1 = RAM[PC++];
	RAM[arg1]--;
	ZERO = RAM[arg1] == 0;
}

static void inst_incr( void )			// increment RAM (2 bytes)
{
	arg1 = RAM[PC++];
	RAM[arg1]++;
	ZERO = RAM[arg1] == 0;
}

static void inst_decrjz( void )			// decrement RAM; if zero, skip next two bytes of code (2 bytes)
{
	arg1 = RAM[PC++];
	RAM[arg1]--;
	ZERO = RAM[arg1] == 0;

	if( ZERO )
		PC += 2;
}

static void inst_incrjz( void )			// increment RAM; if zero, skip next two bytes of code (2 bytes)
{
	arg1 = RAM[PC++];
	RAM[arg1]++;
	ZERO = RAM[arg1] == 0;

	if( ZERO )
		PC += 2;
}

static void inst_shiftrl( void )		// rotate RAM left through CARRY (2 bytes)
{
	arg1 = RAM[PC++];
	temp.low_byte = RAM[arg1];
	temp.word <<= 1, temp.bit0 = CARRY, CARRY = temp.bit8;
	RAM[arg1] = temp.low_byte;
}

static void inst_shiftrr( void )		// rotate RAM right through CARRY (2 bytes)
{
	arg1 = RAM[PC++];
	temp.high_byte = RAM[arg1];
	temp.word >>= 1, temp.bit15 = CARRY, CARRY = temp.bit7;
	RAM[arg1] = temp.high_byte;
}

static void inst_bclr( void )			// bit clear RAM (3 bytes)
{
	arg1 = RAM[PC++];
	arg2 = RAM[PC++];
	RAM[arg2] &= ~(1 << (arg1 & 7));
}

static void inst_bset( void )			// bit set RAM (3 bytes)
{
	arg1 = RAM[PC++];
	arg2 = RAM[PC++];
	RAM[arg2] |= (1 << (arg1 & 7));
}

static void inst_bchg( void )			// bit change RAM (3 bytes)
{
	arg1 = RAM[PC++];
	arg2 = RAM[PC++];
	RAM[arg2] ^= (1 << (arg1 & 7));
}

static void inst_btstsc( void )			// bit test RAM; if clear, skip next two bytes of code (3 bytes)
{
	arg1 = RAM[PC++];
	arg2 = RAM[PC++];

	if( (RAM[arg2] & (1 << (arg1 & 7))) == 0 )
		PC += 2;
}

static void inst_btstss( void )			// bit test RAM; if set, skip next two bytes of code (3 bytes)
{
	arg1 = RAM[PC++];
	arg2 = RAM[PC++];

	if( (RAM[arg2] & (1 << (arg1 & 7))) != 0 )
		PC += 2;
}

static void inst_jump( void )			// arg to PC (2 bytes)
{
	PC = RAM[PC];
}

static void inst_jumpi( void )			// RAM[arg] to PC (2 bytes)
{
	PC = RAM[RAM[PC]];
}

static void inst_call( void )			// push PC, arg to PC (2 bytes)
{
	arg1 = RAM[PC++];					// increment PC in case of halt

	if( SP == STACK_DEPTH )
		HALT;							// stack overflow
	
	else
	{
		stack[SP++] = PC;
		PC = arg1;
	}
}

static void inst_calli( void )			// push PC, RAM[arg] to PC (2 bytes)
{
	arg1 = RAM[PC++];					// increment PC in case of halt

	if( SP == STACK_DEPTH )
		HALT;							// stack overflow
	
	else
	{
		stack[SP++] = PC;
		PC = RAM[arg1];
	}
}

static void inst_return( void )			// pop stack to PC (1 byte)
{
	if( SP == 0 )
		HALT;							// stack underflow
	else
		PC = stack[--SP];
}

static void inst_retla( void )			// copy literal to accumulator, RETURN (2 bytes)
{
	ACCUM = RAM[PC++];
	
	if( SP == 0 )
		HALT;							// stack underflow
	else
		PC = stack[--SP];
}

static void inst_addrpc( void )			// add RAM to PC (2 bytes)
{
	arg1 = RAM[PC++];
	PC += RAM[arg1];
}

static void inst_randa( void )			// store pseudo-random number in accumulator (1 byte)
{
	ACCUM  = TMR0L;						// hardware requires low byte be read before high byte
	ACCUM ^= TMR0H;
	ACCUM ^= TMR1L;
	ACCUM ^= TMR1H;
	ACCUM ^= TMR2;
}

static void inst_comout( void )			// transmit character in accumulator (1 byte)
{
	if( TRMT )							// if transmitter ready
		TXREG = ACCUM;
	else
		PC--;							// try again later
	
	// status unaffected
}

static void inst_comin( void )			// receive character to accumulator (1 byte)
{
	if( comm_rx_ready() )				// if receiver ready
		ACCUM = comm_rx();
	else
		PC--;							// try again later
	
	// status unaffected
}

static void inst_comrdy( void )			// update Z flag with receiver ready status (1 byte); not ready Z = 1, ready Z = 0
{
	ZERO = !comm_rx_ready();
}

static void inst_pinout( void )
{
	switch( RAM[PC++] )
	{
		case 1:
			latc.pin.A = ACCUM;
			break;
			
		case 2:
			latc.pin.B = ACCUM;
			break;
			
		case 3:
			latc.pins.AB = ACCUM;
			break;
	}
}

static void inst_pinin( void )
{
	switch( RAM[PC++] )
	{
		case 1:
			ACCUM = portc.pin.A;
			ZERO = ACCUM == 0;
			break;
			
		case 2:
			ACCUM = portc.pin.B;
			ZERO = ACCUM == 0;
			break;
			
		case 3:
			ACCUM = portc.pins.AB;
			ZERO = ACCUM == 0;
			break;
	}
}

static void inst_pindir( void )
{
	switch( RAM[PC++] )
	{
		case 1:
			trisc.pin.A = ACCUM;
			break;
			
		case 2:
			trisc.pin.B = ACCUM;
			break;
			
		case 3:
			trisc.pins.AB = ACCUM;
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////

#define inst_illegal	inst_halt

typedef void (*inst_fn_ptr)( void );

static const inst_fn_ptr inst_lut[256] =
{
// system control
	inst_halt,			// 0x00		instructions for Digirule 2
	inst_nop,			// 0x01
	inst_speed,			// 0x02
	inst_initsp,		// 0x03		new for Digirule 2A
	
// data movement
	inst_copyla,		// 0x04
	inst_copylr,		// 0x05
	inst_copyli,		// 0x06		new for Digirule 2U
	
	inst_copyar,		// 0x07
	inst_copyai,		// 0x08		new for Digirule 2U
	
	inst_copyra,		// 0x09
	inst_copyrr,		// 0x0A
	inst_copyri,		// 0x0B		new for Digirule 2U
	
	inst_copyia,		// 0x0C		new for Digirule 2U
	inst_copyir,		// 0x0D		new for Digirule 2U
	inst_copyii,		// 0x0E		new for Digirule 2U
	
	inst_swapra,		// 0x0F		new for Digirule 2U
	inst_swaprr,		// 0x10		new for Digirule 2U
	
// arithmetic
	inst_addla,			// 0x11
	inst_addra,			// 0x12
	inst_subla,			// 0x13
	inst_subra,			// 0x14
	inst_mul,			// 0x15		new for Digirule 2U
	inst_div,			// 0x16		new for Digirule 2U
	
// logical
	inst_andla,			// 0x17
	inst_andra,			// 0x18
	inst_orla,			// 0x19
	inst_orra,			// 0x1A
	inst_xorla,			// 0x1B
	inst_xorra,			// 0x1C
	
// increment / decrement
	inst_decr,			// 0x1D
	inst_incr,			// 0x1E
	inst_decrjz,		// 0x1F
	inst_incrjz,		// 0x20
	
// shift (rotate)
	inst_shiftrl,		// 0x21
	inst_shiftrr,		// 0x22
	
// bit manipulation and testing
	inst_bclr,			// 0x23		new for Digirule 2U
	inst_bset,			// 0x24		new for Digirule 2U
	inst_bchg,			// 0x25		new for Digirule 2U
	inst_btstsc,		// 0x26		new for Digirule 2U
	inst_btstss,		// 0x27		new for Digirule 2U
	
// program control
	inst_jump,			// 0x28
	inst_jumpi,			// 0x29		new for Digirule 2U
	inst_call,			// 0x2A
	inst_calli,			// 0x2B		new for Digirule 2U
	inst_return,		// 0x2C
	inst_retla,			// 0x2D
	inst_addrpc,		// 0x2E
	
// miscellaneous
	inst_randa,			// 0x2F		new for Digirule 2A
	
// reserved
	inst_illegal,		// 0x30
	inst_illegal,		// 0x31
	inst_illegal,		// 0x32
	inst_illegal,		// 0x33
	inst_illegal,		// 0x34
	inst_illegal,		// 0x35
	inst_illegal,		// 0x36
	inst_illegal,		// 0x37
	inst_illegal,		// 0x38
	inst_illegal,		// 0x39
	inst_illegal,		// 0x3A
	inst_illegal,		// 0x3B
	inst_illegal,		// 0x3C
	inst_illegal,		// 0x3D
	inst_illegal,		// 0x3E
	inst_illegal,		// 0x3F
	inst_illegal,		// 0x40
	inst_illegal,		// 0x41
	inst_illegal,		// 0x42
	inst_illegal,		// 0x43
	inst_illegal,		// 0x44
	inst_illegal,		// 0x45
	inst_illegal,		// 0x46
	inst_illegal,		// 0x47
	inst_illegal,		// 0x48
	inst_illegal,		// 0x49
	inst_illegal,		// 0x4A
	inst_illegal,		// 0x4B
	inst_illegal,		// 0x4C
	inst_illegal,		// 0x4D
	inst_illegal,		// 0x4E
	inst_illegal,		// 0x4F
	inst_illegal,		// 0x50
	inst_illegal,		// 0x51
	inst_illegal,		// 0x52
	inst_illegal,		// 0x53
	inst_illegal,		// 0x54
	inst_illegal,		// 0x55
	inst_illegal,		// 0x56
	inst_illegal,		// 0x57
	inst_illegal,		// 0x58
	inst_illegal,		// 0x59
	inst_illegal,		// 0x5A
	inst_illegal,		// 0x5B
	inst_illegal,		// 0x5C
	inst_illegal,		// 0x5D
	inst_illegal,		// 0x5E
	inst_illegal,		// 0x5F
	inst_illegal,		// 0x60
	inst_illegal,		// 0x61
	inst_illegal,		// 0x62
	inst_illegal,		// 0x63
	inst_illegal,		// 0x64
	inst_illegal,		// 0x65
	inst_illegal,		// 0x66
	inst_illegal,		// 0x67
	inst_illegal,		// 0x68
	inst_illegal,		// 0x69
	inst_illegal,		// 0x6A
	inst_illegal,		// 0x6B
	inst_illegal,		// 0x6C
	inst_illegal,		// 0x6D
	inst_illegal,		// 0x6E
	inst_illegal,		// 0x6F
	inst_illegal,		// 0x70
	inst_illegal,		// 0x71
	inst_illegal,		// 0x72
	inst_illegal,		// 0x73
	inst_illegal,		// 0x74
	inst_illegal,		// 0x75
	inst_illegal,		// 0x76
	inst_illegal,		// 0x77
	inst_illegal,		// 0x78
	inst_illegal,		// 0x79
	inst_illegal,		// 0x7A
	inst_illegal,		// 0x7B
	inst_illegal,		// 0x7C
	inst_illegal,		// 0x7D
	inst_illegal,		// 0x7E
	inst_illegal,		// 0x7F
	inst_illegal,		// 0x80
	inst_illegal,		// 0x81
	inst_illegal,		// 0x82
	inst_illegal,		// 0x83
	inst_illegal,		// 0x84
	inst_illegal,		// 0x85
	inst_illegal,		// 0x86
	inst_illegal,		// 0x87
	inst_illegal,		// 0x88
	inst_illegal,		// 0x89
	inst_illegal,		// 0x8A
	inst_illegal,		// 0x8B
	inst_illegal,		// 0x8C
	inst_illegal,		// 0x8D
	inst_illegal,		// 0x8E
	inst_illegal,		// 0x8F
	inst_illegal,		// 0x90
	inst_illegal,		// 0x91
	inst_illegal,		// 0x92
	inst_illegal,		// 0x93
	inst_illegal,		// 0x94
	inst_illegal,		// 0x95
	inst_illegal,		// 0x96
	inst_illegal,		// 0x97
	inst_illegal,		// 0x98
	inst_illegal,		// 0x99
	inst_illegal,		// 0x9A
	inst_illegal,		// 0x9B
	inst_illegal,		// 0x9C
	inst_illegal,		// 0x9D
	inst_illegal,		// 0x9E
	inst_illegal,		// 0x9F
	inst_illegal,		// 0xA0
	inst_illegal,		// 0xA1
	inst_illegal,		// 0xA2
	inst_illegal,		// 0xA3
	inst_illegal,		// 0xA4
	inst_illegal,		// 0xA5
	inst_illegal,		// 0xA6
	inst_illegal,		// 0xA7
	inst_illegal,		// 0xA8
	inst_illegal,		// 0xA9
	inst_illegal,		// 0xAA
	inst_illegal,		// 0xAB
	inst_illegal,		// 0xAC
	inst_illegal,		// 0xAD
	inst_illegal,		// 0xAE
	inst_illegal,		// 0xAF
	inst_illegal,		// 0xB0
	inst_illegal,		// 0xB1
	inst_illegal,		// 0xB2
	inst_illegal,		// 0xB3
	inst_illegal,		// 0xB4
	inst_illegal,		// 0xB5
	inst_illegal,		// 0xB6
	inst_illegal,		// 0xB7
	inst_illegal,		// 0xB8
	inst_illegal,		// 0xB9
	inst_illegal,		// 0xBA
	inst_illegal,		// 0xBB
	inst_illegal,		// 0xBC
	inst_illegal,		// 0xBD
	inst_illegal,		// 0xBE
	inst_illegal,		// 0xBF

// serial communication
	inst_comout,		// 0xC0		new for Digirule 2U
	inst_comin,			// 0xC1		new for Digirule 2U
	inst_comrdy,		// 0xC2		new for Digirule 2U
	inst_illegal,		// 0xC3		reserved

// expansion port I/O
	inst_pinout,		// 0xC4
	inst_pinin,			// 0xC5
	inst_pindir,		// 0xC6
	inst_illegal,		// 0xC7		reserved
	
// reserved
	inst_illegal,		// 0xC8
	inst_illegal,		// 0xC9
	inst_illegal,		// 0xCA
	inst_illegal,		// 0xCB
	inst_illegal,		// 0xCC
	inst_illegal,		// 0xCD
	inst_illegal,		// 0xCE
	inst_illegal,		// 0xCF
	inst_illegal,		// 0xD0
	inst_illegal,		// 0xD1
	inst_illegal,		// 0xD2
	inst_illegal,		// 0xD3
	inst_illegal,		// 0xD4
	inst_illegal,		// 0xD5
	inst_illegal,		// 0xD6
	inst_illegal,		// 0xD7
	inst_illegal,		// 0xD8
	inst_illegal,		// 0xD9
	inst_illegal,		// 0xDA
	inst_illegal,		// 0xDB
	inst_illegal,		// 0xDC
	inst_illegal,		// 0xDD
	inst_illegal,		// 0xDE
	inst_illegal,		// 0xDF

// reserved for users
	inst_illegal,		// 0xE0
	inst_illegal,		// 0xE1
	inst_illegal,		// 0xE2
	inst_illegal,		// 0xE3
	inst_illegal,		// 0xE4
	inst_illegal,		// 0xE5
	inst_illegal,		// 0xE6
	inst_illegal,		// 0xE7
	inst_illegal,		// 0xE8
	inst_illegal,		// 0xE9
	inst_illegal,		// 0xEA
	inst_illegal,		// 0xEB
	inst_illegal,		// 0xEC
	inst_illegal,		// 0xED
	inst_illegal,		// 0xEE
	inst_illegal,		// 0xEF
	inst_illegal,		// 0xF0
	inst_illegal,		// 0xF1
	inst_illegal,		// 0xF2
	inst_illegal,		// 0xF3
	inst_illegal,		// 0xF4
	inst_illegal,		// 0xF5
	inst_illegal,		// 0xF6
	inst_illegal,		// 0xF7
	inst_illegal,		// 0xF8
	inst_illegal,		// 0xF9
	inst_illegal,		// 0xFA
	inst_illegal,		// 0xFB
	inst_illegal,		// 0xFC
	inst_illegal,		// 0xFD
	inst_illegal,		// 0xFE
	inst_illegal,		// 0xFF
};

///////////////////////////////////////////////////////////////////////////////
// public functions

void cpu_reset( void )
{
	cpu.address_blanking = 0;
	SP = 0;
	PC = 0;
	SR = 0;
	ACCUM = 0;
	inst_rate = 0;
	
	latc.pins.AB = 0;
	trisc.pins.AB = 0;
}

void cpu_process( void )
{
	if( !latc.led.run )			// if CPU stopped
		inst_timer = 0;
	
	else if( inst_rate == 0 || (t1ms && ++inst_timer == inst_rate) )		// if time to execute instruction
		cpu_step();
}

void cpu_step( void )
{
	inst_lut[RAM[PC++]]();		// fetch and execute next instruction

	address_leds = cpu.address_blanking ? 0 : ram.regs.status.show_address_led_reg ? ram.regs.address_led : PC;
	data_leds.byte = ram.regs.data_led;
	
	inst_timer = 0;
}
