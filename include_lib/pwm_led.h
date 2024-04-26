#ifndef _PWM_LED_H_
#define _PWM_LED_H_

#include "typedef.h"

/*
    方案一：(可用于一个IO推两个灯)
    PWM LED固定IO推灯时, PWM为高时IO输出信号为IO的OUT寄存器电平,PWM为低时IO变成高阻态,相当于PWM信号控制了IO的方向,如下图:
               _______________________________________
    IO OUT信号:                                       |_______________________________________
               __     _      _      _      _      _      _      _      _      _      _      _
    PWMLED信号:  |___| |____| |____| |____| |____| |____| |____| |____| |____| |____| |____| |
               __     _      _      _      _      _
    IO输出信号:  |___| |____| |____| |____| |____| |__________________________________________
               高-高阻-高-高阻--------------------------低-高阻-低高阻------------------------

    方案二:
    当使用OUTPUT CHANNEL时,选择控制IO的DIR,则和固定IO的输出方式是一样的;若选择控制IO的OUT信号,则PWM信号直接输出到IO上,如下图
               __     _      _      _      _      _      _      _      _      _      _      _
    PWMLED信号:  |___| |____| |____| |____| |____| |____| |____| |____| |____| |____| |____| |
               __     _      _      _      _      _      _      _      _      _      _      _
    IO输出信号:  |___| |____| |____| |____| |____| |____| |____| |____| |____| |____| |____| |
               高- 低-高- 低-高- 低 ----------------------------------------------------------
*/

enum {
    PWM_FREQ_DIV1,
    PWM_FREQ_DIV4,
    PWM_FREQ_DIV16,
    PWM_FREQ_DIV64,
    PWM_FREQ_DIV2,
    PWM_FREQ_DIV8,
    PWM_FREQ_DIV32,
    PWM_FREQ_DIV128,
    PWM_FREQ_DIV256,
    PWM_FREQ_DIV1024,
    PWM_FREQ_DIV4096,
    PWM_FREQ_DIV16384,
    PWM_FREQ_DIV512,
    PWM_FREQ_DIV2048,
    PWM_FREQ_DIV8192,
    PWM_FREQ_DIV32768,
};

enum {
    PWM_CLK_SEL_WCLK_32K,
    PWM_CLK_SEL_SRC_32M,
};

#define LED_RED			0 //红灯/橙灯
#define LED_GREEN		1 //绿灯
#define LED_WHITE		2 //白灯

#define LED_SET_BREATH	0 //设置呼吸使能
#define LED_SET_LIGHT	1 //设置常亮使能

#endif  /*  _PWM_LED_H_ */

