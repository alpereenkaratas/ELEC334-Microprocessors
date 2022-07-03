Stack_Size      EQU     0x00000400

RCC_BASE EQU         0x40021000          
RCC_IOPENR EQU      RCC_BASE   + 0x34 

GPIOB_BASE EQU       0x50000400        
GPIOB_MODER EQU     GPIOB_BASE + 0x00 
GPIOB_ODR EQU       GPIOB_BASE + 0x14 

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

    THUMB

    AREA    RESET, DATA, READONLY
    EXPORT  __Vectors

__Vectors
    DCD     __initial_sp                   ; Top of Stack
    DCD     Reset_Handler                  ; Reset Handler
    DCD     NMI_Handler                    ; NMI Handler
    DCD     HardFault_Handler              ; Hard Fault Handler

    AREA    |.text|, CODE, READONLY


; nmi handler
NMI_Handler    PROC
    EXPORT  NMI_Handler
    B .
    ENDP


; hardfault handler
HardFault_Handler    PROC
    EXPORT  HardFault_Handler
    B .
    ENDP


; entry function
Reset_Handler    PROC
    EXPORT  Reset_Handler
    ; Edit below this line
	
; processor speed -> 16Mhz=1/16000000 sec (in 1 cycle)
; subs(1 cycle)+bne(1 cycle)=2 cycle	
; so we need to repeat the same operation 8000000(0x7A1200) times to obtain the 1 second delay

	LDR R6, =GPIOB_ODR
	LDR R5, [R6]
blink
	LDR R4,=0x0000 	   ;led toggles,remember active low
	ANDS R5, R5, R4
	STR R5, [R6]	   ;in this situation light is on
	
	LDR R0, =0x7A1200 				
led_on				   ;delays 1 second with the light
	SUBS R0,R0,#1
	BNE led_on
	
	LDR R4,=0x1000 	   ;led toggles,remember active low
	ORRS R5, R5, R4
	STR R5, [R6]	   ;in this situation light went out
	
	LDR R0, =0x7A1200
led_off				   ;delays 1 second without light
	SUBS R0,R0,#1
	BNE led_off
	
	B blink

    ; Edit above this line
    B .
    ENDP

    END