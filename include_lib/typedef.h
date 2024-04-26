/******************************************************************************/
/* file		typedef.h
 * brief	类型重定义
 * author
 * date		2022-10-15
 * note		F98
 */
/******************************************************************************/
#ifndef _TYPEDEF_H
#define _TYPEDEF_H


#ifdef WIN32
typedef unsigned char	u8, uint8;
typedef char			s8, int8;
typedef unsigned short	u16, uint16;
typedef signed short	s16, int16;
typedef unsigned int	u32, uint32,func;
typedef signed int		s32, int32;
typedef unsigned long	u64, uint64;
typedef signed long		s64, int64;

#define _xdata
#define _data
#define _idata
#define _pdata
#define _bdata
#define _code
#define _bit		bool
#define _sbit		bool
#define pu16		u16
#define pu32		u32

#define	LD_WORD(ptr)		(u16)(*(u16*)(u8*)(ptr))
#define	LD_DWORD(ptr)		(u32)(*(u32*)(u8*)(ptr))
#define	ST_WORD(ptr,val)	*(u16*)(u8*)(ptr)=(u16)(val)
#define	ST_DWORD(ptr,val)	*(u32*)(u8*)(ptr)=(u32)(val)

#endif	/*	WIN32	*/


#ifdef __C51__
typedef unsigned char	u8, uint8;
typedef char			s8, int8;
typedef unsigned int	u16, uint16,func;
typedef signed int		s16, int16;
typedef unsigned long	u32, uint32;
typedef signed long		s32, int32;

#define bool		bit
#define _bool		bool
#define _xdata		xdata
#define _data		data
#define _idata		idata
#define _pdata		pdata
#define _bdata		bdata
#define _bit		bit
#define _sbit		sbit
#define _code		code
#define _small		small
#define _large      large

#define FALSE	0
#define TRUE    1

#define false	0
#define true    1

#define NULL	((void *)0)

//big_endian to little_endian
#define SW16(x)			(((u16)(x) & 0x00ffU)<<8 | ((u16)(x) & 0xff00U)>>8)
#define SW32(x)			( \
						((u32)(x) & 0x000000ffU)<<24| \
						((u32)(x) & 0x0000ff00U)<<8 | \
						((u32)(x) & 0x00ff0000U)>>8 | \
						((u32)(x) & 0xff000000U)>>24 \
						)

#define BIT(n)	            (1 << n)
#define BitSET(REG,POS)     (REG |= (1 << POS))
#define BitCLR(REG,POS)     (REG &= (~(1 << POS)))
#define BitXOR(REG,POS)     (REG ^= (~(1 << POS)))
#define BitCHK_1(REG,POS)   ((REG & (1 << POS)) == (1 << POS))
#define BitCHK_0(REG,POS)   ((REG & (1 << POS)) == 0x00)
#define testBit(REG,POS)    (REG & (1 << POS))

#define clrBit(x,y)         x &= ~(1L << y)
#define setBit(x,y)         x |= (1L << y)

#define SFR_SET(sfr, start, len, dat) (sfr = sfr & ~((~(0xffffffff << len)) << start) | ((dat & (~(0xffffffff << len))) << start))

#define INTABS(x)		((x)>0? (x):-(x))

typedef union {
    u8 byte;
    struct {
        u8 b0 : 1;
        u8 b1 : 1;
        u8 b2 : 1;
        u8 b3 : 1;
        u8 b4 : 1;
        u8 b5 : 1;
        u8 b6 : 1;
        u8 b7 : 1;
    } u;
} __bit8;

#endif	/*	__C51__	*/

#ifdef __IAR_SYSTEMS_ICC__
typedef unsigned char	u8, uint8;
typedef signed char	s8, int8;
typedef unsigned int	u16, uint16,func;
typedef signed int		s16, int16;
typedef unsigned long	u32, uint32;
typedef signed long		s32, int32;

#define _bit		__bit
#define _xdata		__xdata
#define _data		__data
#define _idata		__idata
#define _pdata		__pdata
#define _bdata		__bdata
#define _bit		__bit
#define _code		__code

#define _bool       bool

#define FALSE	0
#define TRUE    1

#define false	0
#define true    1

#define NULL	((void *)0)

//big_endian to little_endian
#define SW16(x)			(((u16)(x) & 0x00ffU)<<8 | ((u16)(x) & 0xff00U)>>8)
#define SW32(x)			( \
						((u32)(x) & 0x00ffU)<<24 | \
						((u32)(x) & 0xff00U)<<8  | \
						((u32)(x) & 0xff0000U)>>8 | \
						((u32)(x) & 0xff000000U)>>24 \
						)

#define BIT(n)	            (1 << n)
#define BitSET(REG,POS)     (REG |= (1 << POS))
#define BitCLR(REG,POS)     (REG &= (~(1 << POS)))
#define BitXOR(REG,POS)     (REG ^= (~(1 << POS)))
#define BitCHK_1(REG,POS)   ((REG & (1 << POS)) == (1 << POS))
#define BitCHK_0(REG,POS)   ((REG & (1 << POS)) == 0x00)
#define testBit(REG,POS)    (REG & (1 << POS))

#define clrBit(x,y)         x &= ~(1L << y)
#define setBit(x,y)         x |= (1L << y)

#define OVERLAY           __root

#define INTABS(x)		((x)>0? (x):-(x))

#define SFR_SET(sfr, start, len, dat) (sfr = sfr & ~((~(0xffffffff << len)) << start) | ((dat & (~(0xffffffff << len))) << start))

typedef union {
    u8 byte;
    struct {
        u8 b0 : 1;
        u8 b1 : 1;
        u8 b2 : 1;
        u8 b3 : 1;
        u8 b4 : 1;
        u8 b5 : 1;
        u8 b6 : 1;
        u8 b7 : 1;
    } u;
} __bit8;

#endif  /* __IAR_SYSTEMS_ICC__ */

#endif    /*    _TYPEDEF_H    */




