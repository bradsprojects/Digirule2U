; Hello World for Digirule 2U
; Brent Hauser
; 21 Sep 2020

cr	equ	13
lf	equ	10

	org	0

start	speed	1

loop	copylr	hello,ptr	; point to beginning of string

loop2	copyra	0		; get character from string
	btstsc	_z,_sr
	jump	loop		; if end of string

	comout			; output character
	incr	ptr		; point to next character
	jump	loop2

ptr	equ	loop2+1		; pointer is embedded in instruction

	org	0x20

hello	byte	cr,lf
	byte	' _  _     _ _      __      __       _    _   _ ',cr,lf
	byte	'| || |___| | |___  \ \    / /__ _ _| |__| | | |',cr,lf
	byte	'| __ / -_) | / _ \  \ \/\/ / _ \ ''_| / _` | |_|',cr,lf
	byte	'|_||_\___|_|_\___/   \_/\_/\___/_| |_\__,_| (_)',cr,lf
	byte	0                                        

	end	start
