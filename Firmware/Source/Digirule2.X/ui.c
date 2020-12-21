#include "digirule2.h"

#define	DEBOUNCE_TIME	33		// in milliseconds
#define HOLD_TIME		1000	// in milliseconds
#define SCAN_TIME		50		// in milliseconds

static void state_stop( void );
static void state_data7( void );
static void state_data0( void );
static void state_load( void );
static void state_save( void );
static void state_run( void );
static void state_run2( void );
static void state_run3( void );
static void state_comm_load( void );
static void state_comm_load2( void );
static void state_comm_load3( void );
static void state_comm_save( void );
static void state_monitor( void );
static void state_wait_release( void );
static void state_wait_release2( void );

///////////////////////////////////////////////////////////////////////////////
// private data

static union
{
	uint16 word;
	struct
	{
		union
		{
			uint8 data;
			struct
			{
				uint8 data0	: 1;
				uint8 data1	: 1;
				uint8 data2	: 1;
				uint8 data3	: 1;
				uint8 data4	: 1;
				uint8 data5	: 1;
				uint8 data6	: 1;
				uint8 data7	: 1;
			};
		};
		uint8 run_stop		: 1;
		uint8 goto_			: 1;
		uint8 store			: 1;
		uint8 prev			: 1;
		uint8 next			: 1;
		uint8 load			: 1;
		uint8 save_			: 1;
	};
} previous, current, button;

static uint8 debounce_timer, scan_timer, scan_index, comm_load_timer, comm_load_errors;
static uint16 hold_timer;
static void (*state)( void ) = state_wait_release;

static const uint8 scan[] =
{
	0b10000000,
	0b01000000,
	0b00100000,
	0b00010000,
	0b00001000,
	0b00000100,
	0b00000010,
	0b00000001,
	0b00000010,
	0b00000100,
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000
};

///////////////////////////////////////////////////////////////////////////////
// public functions

void ui_process( void )
{
	if( t1ms )		// if time to sample buttons
	{
		// sample buttons, inverting polarity
		current.data     = ~portb.button.data;
		current.run_stop = ~portc.button.run_stop;
		current.goto_    = ~porte.button.goto_;
		current.store    = ~porte.button.store;
		current.prev     = ~porte.button.prev;
		current.next     = ~porte.button.next;
		current.load     = ~portc.button.load;
		current.save_    = ~portc.button.save_;
		
		// if this sample is different than previous, start over
		if( previous.word != current.word )
		{
			previous = current;
			debounce_timer = 0;
		}
		// if successive samples are identical, update button state variable
		else if( debounce_timer != DEBOUNCE_TIME && ++debounce_timer == DEBOUNCE_TIME )
		{
			button = current;
			ram.regs.button = button.data;
		}
	
		// process events
		state();
	}
}

///////////////////////////////////////////////////////////////////////////////
// private functions

static void state_stop( void )
{
	if( button.data7 )
	{
		data_leds.d7 ^= 1;
		hold_timer = 0;
		state = state_data7;
	}
	else if( button.data6 )
	{
		data_leds.d6 ^= 1;
		state = state_wait_release;
	}
	else if( button.data5 )
	{
		data_leds.d5 ^= 1;
		state = state_wait_release;
	}
	else if( button.data4 )
	{
		data_leds.d4 ^= 1;
		state = state_wait_release;
	}
	else if( button.data3 )
	{
		data_leds.d3 ^= 1;
		state = state_wait_release;
	}
	else if( button.data2 )
	{
		data_leds.d2 ^= 1;
		state = state_wait_release;
	}
	else if( button.data1 )
	{
		data_leds.d1 ^= 1;
		state = state_wait_release;
	}
	else if( button.data0 )
	{
		data_leds.d0 ^= 1;
		hold_timer = 0;
		state = state_data0;
	}
	else if( button.goto_ )
	{
		address_leds = PC = data_leds.byte;
		data_leds.byte = RAM[PC];
		state = state_wait_release;
	}
	else if( button.store )
	{
		RAM[PC++] = data_leds.byte;
		data_leds.byte = RAM[PC];
		address_leds = PC;
		state = state_wait_release;
	}
	else if( button.prev )
	{
		data_leds.byte = RAM[--PC];
		address_leds = PC;
		state = state_wait_release;
	}
	else if( button.next )
	{
		data_leds.byte = RAM[++PC];
		address_leds = PC;
		state = state_wait_release;
	}
	else if( button.load )
	{
		address_leds = 0;
		scan_timer = scan_index = 0;
		state = state_load;
	}				
	else if( button.save_ )
	{
		address_leds = 0;
		scan_timer = scan_index = 0;
		state = state_save;
	}
	else if( button.run_stop )
	{
		comm_rx_flush();
		latc.led.run = 1;
		state = state_run;
	}
}				

static void state_data7( void )
{
	if( !button.data7 )
		state = state_stop;

	else if( ++hold_timer == HOLD_TIME )
	{
		data_leds.byte = 0b11111111;
		state = state_wait_release;
	}
}

static void state_data0( void )
{
	if( !button.data0 )
		state = state_stop;

	else if( ++hold_timer == HOLD_TIME )
	{
		data_leds.byte = 0;
		state = state_wait_release;
	}
}

static void state_load( void )
{
	if( !button.load )
	{
		address_leds = PC;
		data_leds.byte = RAM[PC];
		state = state_stop;
	}
	else if( button.run_stop )
	{
		data_leds.byte = 0;
		cpu_reset();
		state = state_wait_release2;
	}
	else if( button.prev )
	{
		data_leds.byte = 0;
		
		// clear RAM
		for( uint16 addr = 0; addr < 256; addr++ )
		{
			address_leds = PROGRESS_BAR( addr );
			sleep_ms( 1 );			// for visual effect only
			RAM[addr] = 0;
		}

		cpu_reset();
		state = state_wait_release2;
	}
	else if( button.next )
	{
		address_leds = data_leds.byte = 0;
		latc.led.run = 1;
		comm_load_init();
		process_fn = comm_load_process;
		state = state_comm_load;
	}
	else if( button.store )
	{
		address_leds = data_leds.byte = 0;
		monitor_active = 1;
		monitor_init();
		process_fn = monitor_process;
		state = state_monitor;
	}
	else if( button.data7 )
	{
		data_leds.byte = 0b10000000;
		file_load( 7, 0 );
		cpu_reset();
		state = state_wait_release2;
	}
	else if( button.data6 )
	{
		data_leds.byte = 0b01000000;
		file_load( 6, 0 );
		cpu_reset();
		state = state_wait_release2;
	}
	else if( button.data5 )
	{
		data_leds.byte = 0b00100000;
		file_load( 5, 0 );
		cpu_reset();
		state = state_wait_release2;
	}
	else if( button.data4 )
	{
		data_leds.byte = 0b00010000;
		file_load( 4, 0 );
		cpu_reset();
		state = state_wait_release2;
	}
	else if( button.data3 )
	{
		data_leds.byte = 0b00001000;
		file_load( 3, 0 );
		cpu_reset();
		state = state_wait_release2;
	}
	else if( button.data2 )
	{
		data_leds.byte = 0b00000100;
		file_load( 2, 0 );
		cpu_reset();
		state = state_wait_release2;
	}
	else if( button.data1 )
	{
		data_leds.byte = 0b00000010;
		file_load( 1, 0 );
		cpu_reset();
		state = state_wait_release2;
	}
	else if( button.data0 )
	{
		data_leds.byte = 0b00000001;
		file_load( 0, 0 );
		cpu_reset();
		state = state_wait_release2;
	}
	else if( ++scan_timer == SCAN_TIME )
	{
		data_leds.byte = scan[scan_index];

		if( ++scan_index == ELEMENTSOF( scan ) )
			scan_index = 0;
		
		scan_timer = 0;
	}
}

static void state_save( void )
{
	if( !button.save_ )
	{
		address_leds = PC;
		data_leds.byte = RAM[PC];
		state = state_stop;
	}
	else if( button.next )
	{
		address_leds = data_leds.byte = 0;
		latc.led.run = 1;
		comm_save_init();
		process_fn = comm_save_process;
		state = state_comm_save;
	}
	else if( button.data7 )
	{
		data_leds.byte = 0b10000000;
		file_save( 7, 0 );
		state = state_wait_release2;
	}
	else if( button.data6 )
	{
		data_leds.byte = 0b01000000;
		file_save( 6, 0 );
		state = state_wait_release2;
	}
	else if( button.data5 )
	{
		data_leds.byte = 0b00100000;
		file_save( 5, 0 );
		state = state_wait_release2;
	}
	else if( button.data4 )
	{
		data_leds.byte = 0b00010000;
		file_save( 4, 0 );
		state = state_wait_release2;
	}
	else if( button.data3 )
	{
		data_leds.byte = 0b00001000;
		file_save( 3, 0 );
		state = state_wait_release2;
	}
	else if( button.data2 )
	{
		data_leds.byte = 0b00000100;
		file_save( 2, 0 );
		state = state_wait_release2;
	}
	else if( button.data1 )
	{
		data_leds.byte = 0b00000010;
		file_save( 1, 0 );
		state = state_wait_release2;
	}
	else if( button.data0 )
	{
		data_leds.byte = 0b00000001;
		file_save( 0, 0 );
		state = state_wait_release2;
	}
	else if( ++scan_timer == SCAN_TIME )
	{
		data_leds.byte = scan[scan_index];

		if( ++scan_index == ELEMENTSOF( scan ) )
			scan_index = 0;
		
		scan_timer = 0;
	}
}

static void state_run( void )
{
	if( !button.run_stop )
		state = state_run2;

	else if( !latc.led.run )		// if CPU halted
	{
		address_leds = PC;
		data_leds.byte = RAM[PC];
		state = state_wait_release;
	}
}

static void state_run2( void )
{
	if( button.run_stop )
	{
		latc.led.run = 0;
		address_leds = PC;
		data_leds.byte = RAM[PC];
		state = state_wait_release;
	}
	else if( button.goto_ )
	{
		cpu.address_blanking ^= 1;
		state = state_run3;
	}
	else if( !latc.led.run )		// if CPU halted
	{
		address_leds = PC;
		data_leds.byte = RAM[PC];
		state = state_stop;
	}			
}

static void state_run3( void )
{
	if( !button.goto_ )
		state = state_run2;

	else if( !latc.led.run )		// if CPU halted
	{
		address_leds = PC;
		data_leds.byte = RAM[PC];
		state = state_wait_release;
	}
}

static void state_comm_load( void )
{
	if( button.word == 0 )			// if all buttons released
		state = state_comm_load2;
}

static void state_comm_load2( void )
{
	if( !latc.led.run )				// if done
	{
		process_fn = cpu_process;
		cpu_reset();
		ram.regs.status.byte = ram.regs.button = ram.regs.address_led = ram.regs.data_led = 0;		// clear registers

		LISTENING_LED = 0;

		if( data_leds.byte )		// if load error(s)
		{
			comm_load_errors = data_leds.byte;
			comm_load_timer = 0;
			state = state_comm_load3;
		}
		else						// successful load
		{
			if( end_of_file_record_address < sizeof( ram.program ) )
				PC = (uint8) end_of_file_record_address;

			address_leds = data_leds.byte = 0;
			state = state_wait_release2;
		}
	}
	else if( button.run_stop )		// if cancel
	{
		latc.led.run = 0;
		address_leds = data_leds.byte = 0;
		process_fn = cpu_process;
		state = state_wait_release2;
	}
}

static void state_comm_load3( void )
{
	if( button.data )				// if error(s) acknowledge
	{
		address_leds = data_leds.byte = 0;
		state = state_wait_release2;
	}
	else if( ++comm_load_timer == 125 )
	{
		data_leds.byte = data_leds.byte ? 0 : comm_load_errors;
		comm_load_timer = 0;
	}
}

static void state_comm_save( void )
{
	if( !latc.led.run )
	{
		address_leds = data_leds.byte = 0;
		process_fn = cpu_process;
		state = state_wait_release2;
	}
	else if( button.run_stop )		// if cancel
	{
		latc.led.run = 0;
		address_leds = data_leds.byte = 0;
		process_fn = cpu_process;
		state = state_wait_release2;
	}
}

static void state_monitor( void )
{
	if( !monitor_active || button.run_stop )
	{
		address_leds = data_leds.byte = 0;
		process_fn = cpu_process;
		state = state_wait_release2;
	}
}

static void state_wait_release( void )
{
	if( button.word == 0 )			// if all buttons released
		state = state_stop;
}

static void state_wait_release2( void )
{
	if( button.word == 0 )			// if all buttons released
	{
		address_leds = PC;
		data_leds.byte = RAM[PC];
		state = state_stop;
	}
}
