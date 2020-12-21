; Prime Numbers for Digirule 2U
; Olivier Lecluse and Brent Hauser
; 21 Sep 2020

cr	equ	13
lf	equ	10

prime_flag	equ	3		; we use bit 3 of status to store the prime state

;******************************************************************************
; Main code
	
	org	0

start	speed	0
	initsp

restart	copylr	init_str,outstr+1	; display init_str 
	call	outstr

	copylr	0,_sr			; variables initialisations
	copylr	5,nb			; start the search with 5

test_nb
	call	prime_test
	btstss	prime_flag,_sr
	jump	next_nb
	
	copyrr	nb,_dr			; output number to Data LEDs
	call	outdec			; output number to serial port
	copyla	' '
	comout
	
next_nb 
	incr	nb
	btstsc	_z,_sr			; if zero (256), we're done
	jump	the_end
	incr	nb			; increment nb by 2
	jump	test_nb

the_end
	copyla	cr
	comout
	copyla	lf
	comout
	
	halt
	jump	restart

;******************************************************************************
; Subroutines
	
;**************************************
; primality test
; on entry, nb = number to test
; on exit, prime_flag = 1 (prime) or 0 (not prime)
prime_test
	copylr	0,_sr			; assume number is not prime
	copyrr	nb,dv
	shiftrr	dv
	bset	0,dv			; making sure dv is odd
	
loop_div 
	copyrr	nb,r0
	div	r0,dv			; r0 = quotient, acc = remainder, carry = 1 if remainder is 0, carry = 0 if remainder is not 0
	btstsc	_c,_sr
	return				; number is NOT prime
	nop				; dummy second byte for btst
	decr	dv			; stop when dv is 3
	decr	dv
	copyra	dv
	xorla	1
	btstss	_z,_sr
	jump	loop_div

	bset	prime_flag,_sr		; number is prime
	return
	
;**************************************
; output nb in decimal to serial port
; on entry, nb = number to output
outdec	copylr	stack,stack_ptr		; initialize stack pointer
	copyrr	nb,r0

push_digits 
	div	r0,ten
	copyai	stack_ptr
	incr	stack_ptr
	copyra	r0
	btstss	_z,_sr
	jump	push_digits
	
pop_digits				; output stack to serial
	decr	stack_ptr
	copyia	stack_ptr
	orla	'0'			; convert binary to ASCII
	comout
	copyra	stack_ptr		; test if we reached top of stack
	xorla	stack
	btstss	_z,_sr
	jump	pop_digits
	return

;**************************************
; output a NUL-terminated string to the serial port
; on entry, outstr+1 = address of string
outstr	copyra	0
	btstsc	_z,_sr
	return
	nop				; dummy second byte for btst
	comout
	incr	outstr+1
	jump	outstr

;******************************************************************************
; Data
	
init_str	byte	cr,lf,'2 3 ',0
	
dv		byte	0
ten		byte	10
r0		byte	0
nb		byte	0

stack_ptr	byte	0
stack		byte	0,0,0

;******************************************************************************
; For Olivier

	org	0xA0
	
start2	speed	16
	copylr	olivier_str,outstr+1
	call	outstr
	halt
	jump	start2
	
olivier_str	byte	cr,lf,'The Digirule 2U is dedicated to our good friend and colleague Olivier Lecluse',cr,lf,0

	end	start
