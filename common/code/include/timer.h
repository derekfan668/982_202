#ifndef _TIMER_H_
#define _TIMER_H_

#include "typedef.h"

/*
    TMR PWM应用时, PWM_EN=1时读写CNT是操作PWM占空比寄存器,PWM_EN=0时读写CNT就是操作定时计数值;
    从固定IO输出PWM信号时,需要设置 PWM0_OUTPUT_EN() 或 PWM1_OUTPUT_EN()

    TMR 分频后的时钟不能超过系统时钟!否则TMR不能正常运行
*/


#define TMR_PND         BIT(7)
#define TMR_CPND        BIT(6)
#define TMR_DIV0(x)     ((x & 0x03) << 4)
#define TMR_CLK(x)      (x << 2)
#define TMR_MODE(x)     (x << 0)

#define TMR_DIV1(x)     ((x >> 2) << 6) //only timer0
#define TMR_PWM_EN      BIT(0)
#define TMR_PWM_INV     BIT(1)

enum {
    TMR_DIV_1,
    TMR_DIV_4,
    TMR_DIV_16,
    TMR_DIV_64,
    TMR_DIV_256,    //only timer0
    TMR_DIV_1024,   //only timer0
    TMR_DIV_4096,   //only timer0
    TMR_DIV_16384,  //only timer0
};

enum {
    TMR0_SYS_CLK,   //系统时钟
    TMR0_IO_CLK,    //IO时钟
    TMR0_WCLK,      //WCLK-31.25K
    TMR0_SRC_CLK,   //系统时钟分频前的时钟
};

enum {
    TMR1_SYS_CLK,   //系统时钟
    TMR1_IO_CLK,    //IO时钟
    TMR1_RCH_CLK,   //RCH为整个芯片里面的最高时钟,中心值为28M
    TMR1_LRC_CLK,   //LRC时钟,大约100K
};

enum {
    TMR_MODE_CLOSE,
    TMR_MODE_CNT,
    TMR_MODE_RISE_CAP,
    TMR_MODE_FALL_CAP,
};

extern data u8 timer_8s_cnt;

void timer0_init(void);

#endif  /*  _TIMER_H_ */



