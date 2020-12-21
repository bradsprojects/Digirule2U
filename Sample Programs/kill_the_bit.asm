; Kill the Bit for Digirule 2U
; Brad Slattery
; 21 Sep 2020

	org	0

start	speed	32
	copylr	1,_dr

loop	shiftrl	_dr
	copyra	_br
	xorra	_dr
	copyar	_dr
	jump	loop
	
	end	start
