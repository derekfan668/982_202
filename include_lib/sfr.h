/***************************************************************************
 *                                - SFR.h -
 *
 * Special header for the JieLi F98 Microcontroller.
 *
 ***************************************************************************/

#ifndef __F98_H
#define __F98_H

/***************************************************************************
 *                                - KEIL - C51 - A51
 ***************************************************************************/
#ifdef __KEIL__

/**********Standard SFR*************/
#define SFR(reg, addr)  sfr reg = addr
#define SFRC(reg, addr) SFR(reg, addr)

#define SFR_BIT(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
        sfr   reg = addr;   \
        sbit bit0 = reg^0;  \
        sbit bit1 = reg^1;  \
        sbit bit2 = reg^2;  \
        sbit bit3 = reg^3;  \
        sbit bit4 = reg^4;  \
        sbit bit5 = reg^5;  \
        sbit bit6 = reg^6;  \
        sbit bit7 = reg^7

#define SFR_BITC(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
        SFR_BIT(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)

#endif	/* __KEIL__ */ 

/***************************************************************************
 *                                - IAR - ICC	
 ***************************************************************************/
#ifdef __IAR_SYSTEMS_ICC__ 
/**********Standard SFR*************/
#define SFR(reg,addr)	__sfr __no_init volatile unsigned char (reg) @ (addr)
#define SFRC(reg,addr)  SFR(reg,addr)
#define SFR_BIT(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)    \
__sfr __no_init volatile union      \
{ \
    unsigned char reg; \
    struct \
    { \
        unsigned char bit0: 1; \
        unsigned char bit1: 1; \
        unsigned char bit2: 1; \
        unsigned char bit3: 1; \
        unsigned char bit4: 1; \
        unsigned char bit5: 1; \
        unsigned char bit6: 1; \
        unsigned char bit7: 1; \
    }; \
} @ addr
#define SFR_BITC(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
SFR_BIT(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)
#ifndef _SYSTEM_BUILD
   #pragma system_include
#endif
#pragma language=extended
#endif	/* __IAR_SYSTEMS_ICC__ */

/***************************************************************************
 *                                - IAR - ASM
 ***************************************************************************/
/***************************************************************************
 *   Assembler definitions
 *
 *   The following SFRs are built in in the a8051.exe and can not be
 *   defined explicitly in this file:
 *     ACC,B,PSW,SP,DPL,DPH
 *   The PSW-bits are built-in in the a8051.exe
 *     OV,AC,F0,RS0,RS1,P
 *
 ***************************************************************************/
#ifdef __IAR_SYSTEMS_ASM__

#pragma language = default
/**********Standard SFR*************/
#define SFR(reg,addr)                reg DEFINE addr
#define SFRC(reg,addr)				 SFR(reg,addr)
// *INDENT-OFF*
SBIT_DEFM MACRO reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7
reg DEFINE addr
bit0 DEFINE addr.0
bit1 DEFINE addr.1
bit2 DEFINE addr.2
bit3 DEFINE addr.3
bit4 DEFINE addr.4
bit5 DEFINE addr.5
bit6 DEFINE addr.6
bit7 DEFINE addr.7
	ENDM
// *INDENT-ON*
#define SFR_BIT(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)  \
SBIT_DEFM reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7
#define SFR_BITC(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)

#endif /* __IAR_SYSTEMS_ASM__*/

//===========================================================================//
//
//                             standard SFR
//
//===========================================================================//
SFR(P0_OUT,   0x80);
SFRC(SP,   0x81);
SFR(DP0L,   0x82);
SFR(DP0H,   0x83);
SFR(PCNT_BSEL,   0x84);
SFR(PCNT_CON,   0x85);
SFR(PCNT_DAT,   0x86);

SFR_BIT(IE0,   0x88, ET0, ET1, EADC, ELVD, EWDT, EPORT, EPMU, EA);
SFR(IPCON0L,   0x8A);
SFR(IPCON1L,   0x8B);
SFR(CRC_DIN,   0x8C);
SFR(CRC_REG,   0x8D);
SFR(CRC_CON,   0x8E);
SFR(WDT_CON,   0x8F);

SFR(P0_IN,   0x90);
SFR(P0_DIR,   0x91);
SFR(P0_PU,   0x92);
SFR(P0_PD,   0x93);
SFR(P0_HD,   0x94);
SFR(P0_DIE,   0x95);
SFR(P0_OCEN,   0x96);

SFR(P1_OUT,   0x98);
SFR(P1_DIR,   0x99);
SFR(P1_PU,   0x9A);
SFR(P1_PD,   0x9B);
SFR(P1_HD,   0x9C);
SFR(P1_DIE,   0x9D);
SFR(P1_OCEN,   0x9E);


SFR(P1_IN,   0xA0);
SFR(WKUP_CON,   0xA1);
SFR(WKUP_FLT,   0xA2);
SFR(WKUP_PND,   0xA3);
SFR(UART0_CON0,   0xA4);
SFR(UART0_CON1,   0xA5);
SFR(UART0_BAUD,   0xA6);
SFR(UART0_BUF,   0xA7);

SFR(PPM_STRAN_SET,   0xA8);
SFR(PPM_CON0,   0xA9);
SFR(PPM_STRAN_DAT,   0xAA);
SFR(PPM_READ,   0xAB);


SFR_BIT(P2_OUT,   0xB0, P20_OUT, P21_OUT, P22_OUT, P23_OUT, P24_OUT, P25_OUT, P26_OUT, P27_OUT);
SFR(UART1_CON0,   0xB1);
SFR(UART1_CON1,   0xB2);
SFR(UART1_BAUD,   0xB3);
SFR(UART1_BUF,   0xB4);
SFR(INT_BASE,   0xB5);
SFR(LED0_DUTY,   0xB6);


SFR_BITC(P2_IN,   0xB8, P20_IN, P21_IN, P22_IN, P23_IN, P24_IN, P25_IN, P26_IN, P27_IN);
SFR(P2_DIR,   0xB9);
SFR(P2_PU,   0xBA);
SFR(P2_PD,   0xBB);
SFR(P2_HD,   0xBC);
SFR(P2_DIE,   0xBD);
SFR(P2_OCEN,   0xBE);
SFR(FSPG_CON,   0xBF);

SFR(PMU_DAT, 0xC0);
SFR(PMU_ADR, 0xC1);
SFR(PMU_WREG_RD, 0xC2);
SFR(CLK_CON0, 0xC3);
SFR(CLK_CON1, 0xC4);
SFR(MODE_CON, 0xC5);
SFR(RST_CON, 0xC6);
SFR(MEM_CON, 0xC7);

SFR(IOMC0,   0xC8);
SFR(IOMC1,   0xC9);
SFR(IOMC2,   0xCA);
SFR(IOMC3,   0xCB);
SFR(IOMC4,   0xCC);
SFR(IOMC5,   0xCD);
SFR(IOMC6,   0xCE);
SFR(IOMC7,   0xCF);


SFR_BITC(PSW,   0xD0, P, Z, OV, RS0, RS1, F1, AC, CY);
SFR(LED_CON0,   0xD1);
SFR(LED_CON1,   0xD2);
SFR(LED_PRD,   0xD3);
SFR(OTP_CMD0,   0xD4);
SFR(OTP_CMD1,   0xD5);
SFR(OTP_WDAT,   0xD6);
SFR(OTP_ADR,   0xD7);

SFR(IE1,   0xD8);
SFR(PWR_CON,   0xD9);
SFR(OTP_DATH,   0xDA);
SFR(OTP_DATL,   0xDB);
SFR(RST_SRC,   0xDC);
SFR(LVD_CON0,   0xDD);
SFR(LVD_CON1,   0xDE);
SFR(SFC_CON0,   0xDF);

SFRC(ACC,   0xE0);
SFR(ADC_CON0,   0xE1);
SFR(ADC_CON1,   0xE2);
SFR(ADC_CON2,   0xE3);
SFR(ADC_RESL,   0xE4);
SFR(ADC_RESH,   0xE5);

SFR(TMR1_CON0,   0xE8);
SFR(TMR1_CON1,   0xE9);
SFR(TMR1_CNTH,   0xEA);
SFR(TMR1_CNTL,   0xEB);
SFR(TMR1_PRH,   0xEC);
SFR(TMR1_PRL,   0xED);


SFR(BREG,   0xF0);
SFR(LED1_DUTY,   0xF1);
SFR(LED2_DUTY,   0xF2);
SFR(WPR_CON0,   0xF3);
SFR(WPR_DIV,   0xF4);
SFR(WPR_DAT_ADR,   0xF5);
SFR(WPR_DAT_CNT,   0xF6);
SFR(WPR_PRE_CNT,   0xF7);

SFR(TMR0_CON0,   0xF8);
SFR(TMR0_CON1,   0xF9);
SFR(TMR0_CNT,   0xFA);
SFR(TMR0_PR,   0xFB);
SFR(SFC_KEY0,   0xFC);
SFR(SFC_KEY1,   0xFD);
SFR(CHIP_VER,   0xFE);
SFR(CHIP_ID,   0xFF);

#endif    /*    _F98_H    */






