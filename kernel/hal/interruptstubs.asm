global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19

global isr32
global isr33
global isr34
global isr35
global isr36
global isr37
global isr38
global isr39

global isr46
global isr47


;These are C handlers
extern default_handler
extern timer_handler
extern keyboard_handler


extern send_EOI_master
extern send_EOI_slave

; ---------------------------------------------------------------------
; Minimal exception stubs (0-19) -> call C default_handler
; ---------------------------------------------------------------------
isr0:
	pusha
	call default_handler
	popa
	iret
isr1:
	pusha
	call default_handler
	popa
	iret
isr2:
	pusha
	call default_handler
	popa
	iret
isr3:
	pusha
	call default_handler
	popa
	iret
isr4:
	pusha
	call default_handler
	popa
	iret
isr5:
	pusha
	call default_handler
	popa
	iret
isr6:
	pusha
	call default_handler
	popa
	iret
isr7:
	pusha
	call default_handler
	popa
	iret
isr8:
	pusha
	call default_handler
	popa
	add esp, 4 ; pop error code
	iret
isr9:
	pusha
	call default_handler
	popa
	iret
isr10:
	pusha
	call default_handler
	popa
	add esp, 4 ; pop error code
	iret
isr11:
	pusha
	call default_handler
	popa
	add esp, 4 ; pop error code
	iret
isr12:
	pusha
	call default_handler
	popa
	add esp, 4 ; pop error code
	iret
isr13:
	pusha
	call default_handler
	popa
	add esp, 4 ; pop error code
	iret
isr14:
	pusha
	call default_handler
	popa
	add esp, 4 ; pop error code
	iret
isr15:
	pusha
	call default_handler
	popa
	iret
isr16:
	pusha
	call default_handler
	popa
	iret
isr17:
	pusha
	call default_handler
	popa
	add esp, 4 ; pop error code
	iret
isr18:
	pusha
	call default_handler
	popa
	iret
isr19:
	pusha
	call default_handler
	popa
	iret

isr32:
	pusha
	call timer_handler
	call send_EOI_master
	popa
	iret
isr33:
	pusha
	call keyboard_handler
	call send_EOI_master
	popa
	iret
isr34:
isr35:
isr36:
isr37:
isr38:
isr39:
	pusha
	call send_EOI_master
	popa
	iret


isr46:
isr47:
	pusha
	call send_EOI_master
	call send_EOI_slave
	popa
	iret
