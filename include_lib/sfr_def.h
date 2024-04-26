#ifndef __SFR_DEF_H
#define __SFR_DET_H


//===========================================================================//
//
//                                public
//
//===========================================================================//

    #ifdef c51
    #else
        #define c51
    #endif

    #define u8  unsigned char
    #define u16 unsigned short
    #define u32 unsigned long
    #define BIT(x) (1<<x)

//===========================================================================//
//
//                                access
//
//===========================================================================//
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

    #define XDATA_ACCESS(addr)  (*(volatile unsigned char xdata *)(addr))
    #define XSFR_ACCESS(addr)   XDATA_ACCESS(0x3F00+addr)


#endif /* __SFR_DET_H*/
