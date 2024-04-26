$NOMOD51

;------------------------------------------------------------------------------
; <o> IDATALEN: IDATA memory size <0x0-0x100>
;     <i> Note: The absolute start-address of IDATA memory is always 0
;     <i>       The IDATA space overlaps physically the DATA and BIT areas.
;------------------------------------------------------------------------------
IDATALEN        EQU     100H

CLK_CON0     	DATA    0C3H
CLK_CON1       	DATA    0C4H


                NAME    ?C_STARTUP							 


?C_C51STARTUP   SEGMENT   CODE								 
?STACK          SEGMENT   IDATA

				RSEG    ?STACK
                DS      1

                EXTRN CODE (?C_START)						 
                PUBLIC  ?C_STARTUP
															 
                CSEG    AT      0x0000						 
?C_STARTUP:     LJMP    STARTUP1							 

                RSEG    ?C_C51STARTUP						 

STARTUP1:

;时钟初始化,切换时钟到16M
				ANL     CLK_CON1,#0FCH	;CLK_CON1 &= ~0x03
				ORL     CLK_CON1,#01H	;CLK_CON1 |=  0x01 		;div 1

				ANL     CLK_CON0,#0F8H	;CLK_CON0 &= ~0x07
				ORL     CLK_CON0,#03H	;CLK_CON0 |=  0x03 		;sel rch_clk_32M

				ORL     CLK_CON0,#010H	;CLK_CON0 |=  BIT(4) 	;sel mclko

;把IDATA的RAM清0(只清0x00~0xFF)
                MOV     R0,#IDATALEN - 1
                CLR     A
IDATALOOP:      MOV     @R0,A
                DJNZ    R0,IDATALOOP
		 
				MOV     SP,#?STACK-1
				LJMP    ?C_START							 

                END
