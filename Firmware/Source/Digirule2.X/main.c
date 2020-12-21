#include "digirule2.h"

bool_t t1ms;

void (*process_fn)( void ) = cpu_process;

void main( void )
{
	OSCCONbits.IRCF = 0b111;	// Fosc to 16 MHz
	
	// initialize ports
	ANSEL	= 0b00000000;		// all analog-capable pins to digital
	ANSELH	= 0b00000000;
	
	LATA	= 0b00000000;
	TRISA	= 0b00000000;		// RA7..0 (DATA LEDs) to outputs
	
	LATB	= 0b00000000;
	TRISB	= 0b11111111;		// RB7..0 (DATA buttons) to inputs
	RBPU	= 0;
	WPUB	= 0b11111111;		// with pull-ups

	LATC	= 0b01000000;
	TRISC	= 0b10100101;		// RC7 (RXD), RC5 (LOAD button), RC2 (SAVE button) and RC0 (RUN/STOP button) to inputs, RC6 (TXD), RC1 (RUN/STOP LED) and all unused to outputs
	
	LATD	= 0b00000000;
	TRISD	= 0b00000000;		// RD7..0 (ADDRESS LEDs) to outputs
	
	LATE	= 0b00000000;
	TRISE	= 0b00001111;		// RE3 (NEXT button), RE2 (PREV button), RE1 (SAVE button) and RE0 (GOTO button) to inputs

	// initialize Timer 0 for 16 MHz (Fosc) / 4 (Fcyc) / 64 (prescale) / 65536 = 954 mHz (1.049 S period)
	T0CON	= 0b10000101;
	
	// initialize Timer 1 for 16 MHz (Fosc) / 4 (Fcyc) / 1 (prescale) / 65536 = 61.035 Hz (16.384 mS period)
	T1CON	= 0b10000001;
	
	// initialize Timer 2 for 16 MHz (Fosc) / 4 (Fcyc) / 4 (prescale) / 100 (PR2 + 1) / 10 (postscale) = 1 KHz (1 mS period)
	PR2		= 100 - 1;
	T2CON	= 0b01001101;

	// initialize UART for 9600 baud
	SPBRG	= 16000000 / 9600 / 64 - 1;
	SPEN    = 1;				// enable serial port
	TXEN    = 1;				// enable transmitter
	CREN	= 1;				// enable receiver
	RCIE	= 1;				// enable receive interrupt
	
	GIE = PEIE = 1;

	lamp_test();
	
	// main loop
	for( ;; )
	{
		t1ms = 0;

		// if timer rolled over, set millisecond event flag for one (and only one) loop iteration
		if( TMR2IF )
		{
			TMR2IF = 0;
			t1ms = 1;
		}
		
		ui_process();
		process_fn();
	}
}
