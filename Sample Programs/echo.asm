; Echo (comm loopback) for Digirule 2U
; Brent Hauser
; 21 Sep 2020

	org	0

start	copylr	0,_dr

loop	comin
	comout
	copyar	_dr
	jump	loop

	end	start
