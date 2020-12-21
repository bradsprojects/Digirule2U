; Base Conversion Trainer for Digirule 2U
; Olivier Lecluse
; 21 Sep 2020

; Connect a terminal to the DR2U
; Run the program
; The DR2U displays a binary number on the data LEDs
; Enter the hexadecimal equivalent and press Return
; Speed is at address 248
; Base is at address 249

	org	0

start	initsp	

restart	speed	0
	bset	_sar,_sr

; initialize game

	randa	
;	copyla	0xAA			; This is cheating
	copyar	nb2guess
	copyrr	nb2guess,_dr		; prints number to guess on data LEDs
	copylr	0,nbplayer		; user guess
	call	init_timer		; time limit

guess_nb
; Wait for user input
	comrdy	
	btstss	_z,_sr
	jump	read_nb
	call	tick_timer
	btstsc	_c,_sr
	jump	you_loose
	jump	guess_nb
read_nb
; a character is available
	comin	
	comout				; echo on console
	copyar	char
	xorla	13
	btstsc	_z,_sr
; End of input on Enter key
	jump	input_end
	copyra	char
; to upper case
	bclr	_c,_sr
	subla	'a'
	btstss	_c,_sr
	subla	'a'-'A'
	addla	'a'-1
; letter is uppercase
; test if digit or letter
	bclr	_c,_sr
	subla	'A'
	btstss	_c,_sr
	subla	7			; A -> '0' + 10
	addla	'A'-1
	subla	'0'-1			; user input (0-15) is in accumulator
	mul	nbplayer,BASE
	addra	nbplayer
	copyar	nbplayer
	jump	guess_nb
input_end
	copylr	0,_sr
	copyra	nb2guess
	xorra	nbplayer
	btstsc	_z,_sr
	jump	you_win
	jump	you_loose

you_win
	copylr	0xFF,_dr
	copylr	win_str,strPtr
	call	print_message
	call	wait_for_space
	jump	restart
you_loose
	copylr	0,_dr
	copylr	lose_str,strPtr
	call	print_message
	call	wait_for_space
	jump	restart

; Displays message string
print_message
	copyia	strPtr
	btstsc	_z,_sr
	return	
	nop	
	comout	
	incr	strPtr
	jump	print_message

wait_for_space
	copylr	space_str,strPtr
	call	print_message
	comin	
	xorla	' '
	btstss	_z,_sr
	jump	wait_for_space
	return	

init_timer
; Initialize timer
	copylr	0,counter
	copyrr	COUNT_SPEED,cs
	copylr	0xFF,cs+1
	copylr	0,_ar
	bclr	_c,_sr
	return	
tick_timer
; Non blocking timer
	decrjz	cs+1
	return	
	nop
	copylr	0xFF,cs+1
	decrjz	cs
	return	
	nop	
	copyrr	COUNT_SPEED,cs
; displays a progress bar on address LEDs
	bclr	_c,_sr
	shiftrl	counter
	incr	counter
	copyrr	counter,_ar
; The Carry is Set in case of TimeOut
	return	

; Variables declaration

cs		byte	0,0
counter		byte	0
nb2guess	byte	0
char		byte	0
nbplayer	byte	0
strPtr		byte	0
win_str		byte	13,10,'You win,',0
lose_str	byte	13,10,'You lose,',0
space_str	byte	' hit SPACE to restart',13,10

	org	248

COUNT_SPEED	byte	20
BASE		byte	16

	end	start
