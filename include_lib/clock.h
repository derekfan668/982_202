#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "typedef.h"

#define HRC_CLK_DEF         32000000UL
#define LRC_CLK_DEF			200000UL

//CLK_CON
#define SFR_CLK_KEEP()      CLK_CON0 |=  BIT(7) //keep住后,SFR不能读写
#define SFR_CLK_NO_KEEP()   CLK_CON0 &= ~BIT(7) //默认为不keep住的状态

#define BOOST_CLK_EN()      CLK_CON0 |=  BIT(5) //BOOST rc_clk_1m 使能
#define BOOST_CLK_DIS()     CLK_CON0 &= ~BIT(5) //BOOST 时钟禁能

#define SRC_CLK_SEL_4M()    CLK_CON0 &= ~BIT(4) //SRC时钟选择RC_CLK_4M
#define SRC_CLK_SEL_MCLK()  CLK_CON0 |=  BIT(4) //选主时钟

#define UART_CLK_SEL_16M()  CLK_CON0 &= ~BIT(6) //DEFAULT
#define UART_CLK_SEL_SYS()  CLK_CON0 |=  BIT(6)

#define MUX_CLK_SEL(x)      CLK_CON0 &= ~0x07; CLK_CON0 |= (x<<0)

#define SYS_CLK_DIV(x)      CLK_CON1 &= ~0x03; CLK_CON1 |= (x<<0)

#define PAT_CLK_EN()		CLK_CON1 |= BIT(2)
#define IS_PAT_CLK_EN()		(CLK_CON1 & BIT(2))

enum {
    MUX_CLK_SEL_RC_250K,
    MUX_CLK_SEL_RC_8M,
    MUX_CLK_SEL_RC_16M,
    MUX_CLK_SEL_RCH_32M,
    MUX_CLK_SEL_LRC_200K,
};

#define SRC_CLK             	  otp_info.rch_clk
#define SYS_DIV                   2 //系统时钟必须小于16M
#define SYS_CLK                   (otp_info.rch_clk / SYS_DIV)
#define W_CLK                     (otp_info.rch_clk >> 10) // /1024
#define LRC_CLK					  otp_info.lrc_clk
#define UART_SRC_CLK        	  (otp_info.rch_clk >> 1) // /2
#define UART_BAUD_SRC(x)          (((UART_SRC_CLK / x) >> 2) - 1)	 //DIV4   9600
#define UART_BAUD_DIVS3_SRC(x)	  (((UART_SRC_CLK / x) / 3) - 1)	 //DIV3	  115200
#endif  /*  _CLOCK_H_ */


