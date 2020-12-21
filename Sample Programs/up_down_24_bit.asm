; Up/Down 24-bit Counter for Digirule 2U
; Brent Hauser
; 21 Sep 2020

step	equ	1		; amount to add/subtract on each pass

	org	0

start	initsp
	bset	_sar,_sr

loop	call	count

	copyrr	_br,_br
	btstsc	_z,_sr		; if any data button pressed
	jump	loop

	incr	dir		; change direction

loop2	call	count

	copyrr	_br,_br
	btstss	_z,_sr		; if all data buttons released
	jump	loop2

	jump	loop


count	bclr	_c,_sr

	btstsc	0,dir
	jump	down

up	copyra	byte0
	addla	step
	copyar	byte0

	copyra	byte1
	addla	0
	copyar	byte1

	copyra	byte2
	addla	0
	copyar	byte2

	return

down	copyra	byte0
	subla	step
	copyar	byte0

	copyra	byte1
	subla	0
	copyar	byte1

	copyra	byte2
	subla	0
	copyar	byte2

	return


	org	0xF0

dir	byte	0		; counting direction

byte0	space	1		; lower byte

	org	_ar
byte2	space	1		; address LED register used as upper byte

	org	_dr
byte1	space	1		; data LED register used as middle byte

	end	start
