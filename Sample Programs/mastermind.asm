;****************************************************************************
; Mastermind game for Digirule 2U
; (c) Emmanuel Lazard
; License CC-BY https://creativecommons.org/licenses/by/3.0/au/
;****************************************************************************
; The purpose of the game is to guess a secret code chosen by the computer.
; A code is composed of 4 digits, each one being between 0 and 7.
; Repetitions are allowed (so that 1213 is a valid code for example).
;
; At each of your turns, you may enter 4 digits that will be your guess.
; The computer will then compare its secret code and your combination and
; will tell you the number of correct digits (same digit in the same position)
; and the number of incorrectly placed digits (digit is correct but not in the
; correct position). Each digit can only be used in one match
; (ex a correctly placed digit cannot also be considered incorrectly placed
; towards another digit in the secret code).
; You win when you have found the secret code, i.e. when the computer's answer
; is 4-0 (4 correctly placed digits).
;****************************************************************************
;
; When the program is started, the computer will randomly generate
; its secret code. It will then wait for the user to enter his first
; leftmost digit (digit number 4) by showing 00000100 (number 4 in binary)
; on the address LEDs.
; To enter a digit, the user presses a button between D0 (digit = 0) and
; D7 (digit = 7).
; The computer will then wait for digits number 3 (showing 00000011 on
; address LEDs) to digit number 1 (showing 00000001).
;
; Once the user has entered all 4 digits, the computer will compare them to
; its secret code and will display
; - Number of correct digits (and right place) on LEDs A4 to A7. As many LEDs
;   will lit as there are correct digits (0 to 4 LEDs),
; - Number of "incorrect" digits (correct digit but wrong place) on LEDs A0 to A3.
;   As many LEDs will lit as there are incorrect digits (0 to 4 LEDs)
; For example, if address LEDs display 00110011, that means there are
; 2 correct digits and 2 badly placed digits. A display of 00010000 shows only
; 1 correct digit.
; At the same time, data LEDs will show (in binary) the number of tries so far.
;
; Once the user has taken note of the result, he can press D7 button to go back
; entering his next guess (display will reverse to 00000100, asking for digit
; number 4). 
;
; A final display of 11110000 on the address LEDs (4 correct digits) means
; the user has correctly guessed the secret code. Pressing D7 will go back
; to the start, with the computer picking up a new secret code.
;****************************************************************************

	org	0

start	initsp

; Clear number of tries and show data on address LEDs

restart	copylr	4,_sr
	copylr	0,numberTries+3		; reset number of tries

; Compute four random digits by asking four times for a random number and
; keeping the three least significant bits each time. The digit is copied
; in memory

	randa				; get a random value in Acc
	andla	7			; keep only first three bits (so it's 0-7)
	copyar	code_1			; and copy into memory
	randa				; get a random value in Acc
	andla	7			; keep only first three bits (so it's 0-7)
	copyar	code_2			; and copy into memory
	randa				; get a random value in Acc
	andla	7			; keep only first three bits (so it's 0-7)
	copyar	code_3			; and copy into memory
	randa				; get a random value in Acc
	andla	7			; keep only first three bits (so it's 0-7)
	copyar	code_4			; and copy into memory

; This commented code (same length as above) is used to force a secret code.
; Useful for testing.

;	copylr	1,code_1
;	copylr	2,code_2
;	copylr	3,code_3
;	copylr	4,code_4
;	nop
;	nop
;	nop
;	nop
;	nop
;	nop
;	nop
;	nop



startNewTry
	copylr	0,_dr			;  clear number of tries display

; Copy guess into working copy
	copyrr	code_1,copy_1
	copyrr	code_2,copy_2
	copyrr	code_3,copy_3
	copyrr	code_4,copy_4

; Display digit number on address LEDs

	copylr	4,_ar

; We must now wait for the user to press a button

testButton
	copyrr	_br,tempI
	btstsc	_z,_sr
	jump	testButton

; user pressed a button, it's in Accumulator
; we must now convert bit 0 to 7 pressed
;  to value from 0 to 7. To do so, we shift right until bit found
;  and increment digit each time.

	copylr	255,tempE		; start with 255 so first 'incr' brings it to 0
value	incr	tempE
	shiftrr	tempI
	btstss	_c,_sr
	jump	value

; Now digit (between 0 and 7) is in tempE
; We now compare it to the digit in code (working copy)
; that is at the same place.
;  Prepare Address to be tested

	bclr	_c,_sr
	copyla	copy_1-1
	addra	_ar			; (is between 4 and 1)

; Now Acc 'points' to location copy_1 to _4 (copy_1-1 + _ar)
; Update location below to point to correct digit

	copyar	index+1
	copyar	index2+2
	copyra	tempE			; get digit back and compute digit - copy_i
index	xorra	255			; 255 is the placeholder for the correct address
	btstss	_z,_sr			; digits are equal?
	jump	noMatch
match	incr	correctPlace
index2	copylr	42,255			; if match put 42 (dummy value) in digit copy
	copylr	43,tempE		; and 43 (dummy value) in (future) user digit

; The dummy values are used to prevent any late match with another digit 
; once a match has already been found.
;  Prepare Address to store digit

noMatch	copyla	userDigit_1-1
	addra	_ar

; Now Acc 'points' to location userDigit_1 to _4 (userDigit_1-1 + _ar)
; Update location below to store digit

	copyar	index3+2
index3	copyrr	tempE,255		; 255 is the placeholder for the correct address

; We now have to wait until user releases the button

testButtonReleased
	copyra	_br
	btstss	_z,_sr
	jump	testButtonReleased

; And we can now go to the next digit

	decrjz	_ar
	jump	testButton

; Now we have all 4 user digits in memory
; We now have to compare them to the code for the 'incorrectly placed' digits
; To do so, we have two embedded loops. For each user digit, we loop over all 4
; computer secret digits.

incorrectlyPlaced
	copylr	4,tempE
innerLoop
	copylr	4,tempI
innerCompareLoop
	copyra	copy_1
	xorra	userDigit_1
	btstss	_z,_sr	; if zero=1, match
	jump	noMatch2
	incr	wrongPlace		; increment number of wrong placed guesses
	copylr	41,copy_1		; and change code for next digit
	copylr	44,userDigit_1		; in copy and userDigit (dummy values)
noMatch2
	incr	innerCompareLoop+1	; next digit for copy_1
	incr	innerCompareLoop+13	; next digit in userDigits_1
	decrjz	tempI
	jump	innerCompareLoop	; next inner loop iteration
	copylr	copy_1,innerCompareLoop+1 ; put back to original addresses
	copylr	copy_1,innerCompareLoop+13
; next outer loop iteration
	incr	innerCompareLoop+3
	incr	innerCompareLoop+16
	decrjz	tempE
	jump	innerLoop
	copylr	userDigit_1,innerCompareLoop+3
	copylr	userDigit_1,innerCompareLoop+16

; Now we must show the results
; First compute as many LEDs as correctly placed digits
; We just add 16 (LED 4) for each correctPlace and shift

;	copylr	0,tempE			; we know tempE==0 from above
	bclr	_c,_sr			; clear carry prior to shift
	incr	correctPlace

checkCP	decr	correctPlace
	btstsc	_z,_sr
	jump	next
	shiftrl	tempE
	copyra	tempE
	addla	16
	copyar	tempE
	jump	checkCP

; next add LEDs for incorrectly placed digits

next
;	copylr	0,correctPlace		; we know correctPlace = 0

; use correctPlace to compute number of LEDs

	incr	wrongPlace

checkWP	decr	wrongPlace
	btstsc	_z,_sr
	jump	next2
	shiftrl	correctPlace
	incr	correctPlace
	jump	checkWP

; combine the two displays

next2	copyra	tempE
	orra	correctPlace
	copyar	_ar

	copylr	0,correctPlace		; put back to 0 for next guess
;	copylr	0,wrongPlace		; wrongPlace is already null here

; Now the Address LEDs display the results
;  (correct places with LEDs 7-4 and wrong places with LEDs 3-0)

numberTries
	incr	numberTries+3
	copylr	255,_dr			; 255 is a placeholder where the real counter will be put

; Wait for button 7 to be pressed and released to start another try

waitButton
	btstss	7,_br
	jump	waitButton

waitButton2
	btstsc	7,_br
	jump	waitButton2

	xorla	0b11110000		; win?
	btstsc	_z,_sr
	jump	restart			; if yes, jump to new game
	jump	startNewTry		; if not, jump to another try

;********** End of code ********************

; These nop instructions are just for padding so that
; variables are at the end of memory (last available address = 251)

;********** Variables **********************

	org	252-16

; My memory locations
; Random combination to guess
code_1		space	1
code_2		space	1
code_3		space	1
code_4		space	1

; User combination
userDigit_1	space	1
userDigit_2	space	1
userDigit_3	space	1
userDigit_4	space	1

; Temporary locations to test digits
copy_1		space	1
copy_2		space	1
copy_3		space	1
copy_4		space	1

; counters
correctPlace	space	1
wrongPlace	space	1
tempE		space	1
tempI		space	1

		end	start
