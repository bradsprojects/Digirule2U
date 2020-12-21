#include <xc.inc>	

ctrl_d			equ	0x04
ctrl_e			equ	0x05
ctrl_q			equ	0x11
ctrl_r			equ	0x12
ctrl_w			equ	0x17
	
flash_erase_size	equ	64

address_leds		equ	LATD
data_leds		equ	LATA

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
			
	psect	powerup,class=CODE,delta=1,reloc=2

	global	powerup
powerup:
	nop					; recommended errata workaround

; test if flash is erased
	
	bcf	EECON1,EECON1_CFGS_POSN,a	; access flash or EEPROM, not config
	bsf	EECON1,EECON1_EEPGD_POSN,a	; access flash, not EEPROM
	
	movlw	0xFF				; read flash location 0x00FFFF
	movwf	TBLPTR,a
	movwf	TBLPTRH,a
	clrf	TBLPTRU,a
	tblrd*

	incf	TABLAT,w,a
	btfsc	STATUS,STATUS_Z_POSN,a
	bra	firmware_updater		; if flash is erased
	
; test for explicit firmware update request
	
	bcf	ANSEL,ANSEL_ANS5_POSN,a		; RE0 to digital for button test
	
	btfsc	PORTE,0,a
	bra	app_start			; if GOTO button not pressed
	
	btfsc	PORTC,5,a
	bra	app_start			; if LOAD button not pressed
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
firmware_updater:
	movlw	0b01110000			; Fosc to 16 MHz
	iorwf	OSCCON,f,a

	clrf	ANSEL,a				; all analog-capable pins to digital
	clrf	ANSELH,a
	
	clrf	LATA,a				; initialize ports
	clrf	TRISA,a
	
	clrf	LATB,a
	clrf	TRISB,a
	
	movlw	0b01000000
	movwf	LATC,a
	movlw	0b10100101
	movwf	TRISC,a
	
	clrf	LATD,a
	clrf	TRISD,a
	
	clrf	LATE,a
	movlw	0b00001111
	movwf	TRISE,a
	
	movlw	0b10000101			; initialize timer
	movwf	T0CON,a
	clrf	TMR0H,a				; order dependency
	clrf	TMR0L,a				; order dependency
	
	bsf	TXSTA,TXSTA_BRGH_POSN,a		; initialize UART
	movlw	16000000/38400/16-1
	movwf	SPBRG,a
	bsf	RCSTA,RCSTA_SPEN_POSN,a
	bsf	TXSTA,TXSTA_TXEN_POSN,a
	bsf	RCSTA,RCSTA_CREN_POSN,a

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

get_command:
	rcall	rx_wait
	movf	RCREG,w,a

	xorlw	ctrl_d
	btfsc	STATUS,STATUS_Z_POSN,a
	bra	command_done
	
	xorlw	ctrl_e^ctrl_d
	btfsc	STATUS,STATUS_Z_POSN,a
	bra	command_erase
	
	xorlw	ctrl_r^ctrl_e
	btfsc	STATUS,STATUS_Z_POSN,a
	bra	command_read
	
	xorlw	ctrl_w^ctrl_r
	btfsc	STATUS,STATUS_Z_POSN,a
	bra	command_write
	
	bra	get_command

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

command_done:
	reset
	bra	$

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
command_erase:	
	clrf	TBLPTR,a			; point to end of flash 0x010000
	clrf	TBLPTRH,a
	movlw	0x01
	movwf	TBLPTRU,a
	
ce_loop:
	movlw	flash_erase_size		; previous block
	subwf	TBLPTR,f,a
	movlw	0
	subwfb	TBLPTRH,f,a
	subwfb	TBLPTRU,f,a
	
	movff	TBLPTRH,address_leds		; display address
	rcall	blink
	
	bsf	EECON1,EECON1_WREN_POSN,a	; erase block
	bsf	EECON1,EECON1_FREE_POSN,a
	movlw	0x55
	movwf	EECON2,a
	movlw	0xAA
	movwf	EECON2,a
	bsf	EECON1,EECON1_WR_POSN,a
	bcf	EECON1,EECON1_WREN_POSN,a
	
	movlw	0x08				; check if at start of application 0x000800
	xorwf	TBLPTRH,w,a
	iorwf	TBLPTR,w,a
	btfss	STATUS,STATUS_Z_POSN,a
	bra	ce_loop

	clrf	address_leds,a			; display off
	
	rcall	tx_wait
	movlw	ctrl_q				; send x-on
	movwf	TXREG,a
	
	bra	get_command
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

command_read:	
	clrf	TBLPTR,a			; point to start of flash 0x000000
	clrf	TBLPTRH,a
	clrf	TBLPTRU,a
	
cr_loop:
	movff	TBLPTRH,address_leds		; display address
	
	tblrd*
	rcall	tx_wait
	movff	TABLAT,TXREG			; send data
	
	movff	TABLAT,data_leds		; display data
	
	movlw	1				; next byte
	addwf	TBLPTR,f,a
	movlw	0
	addwfc	TBLPTRH,f,a
	addwfc	TBLPTRU,f,a
	
	btfss	TBLPTRU,0,a			; check if end of flash 0x010000
	bra	cr_loop

	clrf	address_leds,a			; display off
	clrf	data_leds,a

	bra	get_command
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

command_write:
	clrf	TBLPTR,a			; point to start of application 0x000800
	movlw	0x08
	movwf	TBLPTRH,a
	clrf	TBLPTRU,a

cw_loop:
	movff	TBLPTRH,address_leds		; display address

	rcall	rx_wait
	movff	RCREG,TABLAT			; receive data
	tblwt*
	
	movff	TABLAT,data_leds		; display data
	
	movf	TBLPTR,w,a			; check if last byte of block
	andlw	0x3F
	xorlw	0x3F
	btfss	STATUS,STATUS_Z_POSN,a
	bra	cw_next
	
	bsf	EECON1,EECON1_WREN_POSN,a	; write block
	movlw	0x55
	movwf	EECON2,a
	movlw	0xAA
	movwf	EECON2,a
	bsf	EECON1,EECON1_WR_POSN,a
	bcf	EECON1,EECON1_WREN_POSN,a
	
	rcall	tx_wait
	movlw	ctrl_q				; send x-on
	movwf	TXREG,a
	
cw_next:
	movlw	1				; next byte
	addwf	TBLPTR,f,a
	movlw	0
	addwfc	TBLPTRH,f,a
	addwfc	TBLPTRU,f,a
	
	btfss	TBLPTRU,0,a			; check if end of flash 0x010000
	bra	cw_loop
	
	clrf	address_leds,a			; display off
	clrf	data_leds,a

	bra	get_command

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; helper subroutines

rx_wait:
	btfss	RCSTA,RCSTA_OERR_POSN,a		; check if receiver overrun error
	bra	rw_2
	
	bcf	RCSTA,RCSTA_CREN_POSN,a		; reset overrun error
	bsf	RCSTA,RCSTA_CREN_POSN,a
	movf	RCREG,w,a			; flush receiver
	movf	RCREG,w,a
	
rw_2:	
	btfsc	PIR1,PIR1_RCIF_POSN,a		; check if receive data available
	return
	
	rcall	blink
	bra	rw_2

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

tx_wait:
	btfsc	TXSTA,TXSTA_TRMT_POSN,a		; check if transmitter empty
	return
	
	rcall	blink
	bra	tx_wait

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

blink:
	movf	TMR0L,w,a			; dummy read of TMR0L updates TMR0H
	btfss	TMR0H,7,a			; copy timer MSB to RUN LED
	bra	bk_2
	
	bsf	LATC,1,a
	return
	
bk_2:
	bcf	LATC,1,a
	return

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

app_start:
	global	start
	goto	start				; launch the C application

	end
