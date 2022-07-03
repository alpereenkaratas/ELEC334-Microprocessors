Stack_Size      EQU     0x00000400

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

    LDR R1,=0x14224
    LDR R0,[R1]
    LDR R3,=0x14228
    LDR R2,[R3]
    EORS R2,R2,R0 
    ;Assume that the numbers are 5 bits
    MOVS R4,#0x5
    MOVS R5,#0 ;counter of 1's.
loop
	SUBS R4,R4,#1
	CMP R4,#0	
	BNE finish   ;if zero flag is clear,branch to finish
	LSRS R2,R2,#1
	BCC loop      ;if carry flag is clear,branch to loop
	ADDS R5,R5,#1
	B loop
finish
	ADDS R3,R3,#4
	STR R5,[R3]	  ;write the result to the R3 memory address
   
    ; Edit above this line
    B .
    ENDP

    END