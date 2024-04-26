$NOMOD51
;------------------------------------------------------------------------------
; <o> IDATALEN: IDATA memory size <0x0-0x100>
;     <i> Note: The absolute start-address of IDATA memory is always 0
;     <i>       The IDATA space overlaps physically the DATA and BIT areas.
;------------------------------------------------------------------------------
IDATALEN        EQU     100H

                NAME    ?C_STARTUP							 


?C_C51STARTUP   SEGMENT   CODE								 
?STACK          SEGMENT   IDATA

				RSEG    ?STACK
                DS      1

                EXTRN CODE (?C_START)						 
                PUBLIC  ?C_STARTUP
															 
                CSEG    AT      0x4000						 
?C_STARTUP:     LJMP    STARTUP1							 

                RSEG    ?C_C51STARTUP						 

STARTUP1:
				
;把IDATA的RAM清0(只清0x00~0xFF)
                MOV     R0,#IDATALEN - 1
                CLR     A
IDATALOOP:      MOV     @R0,A
                DJNZ    R0,IDATALOOP
		 
				MOV     SP,   #?STACK-1
				LJMP    ?C_START							 

                END
